#include "multiplexer.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/assert.h"
#include "iridium/items.h"
#include "iridium/io/event_.h"
#include "iridium/threading/implementation/async_queue.h"

#include <stdint.h>
#include <limits>
#include <iostream>


using iridium::threading::implementation::CAsyncQueue;


#include <iridium/logging/logger.h>
namespace iridium::io::implementation::platform {


static constexpr ULONG_PTR WAKE_COMPLETION_KEY
    = static_cast<ULONG_PTR>(-1);
// special completion keys for internal signals
static constexpr ULONG_PTR FINALIZE_COMPLETION_KEY
    = static_cast<ULONG_PTR>(-2);


// todo: mv to common place
DWORD CMultiplexer::assertOK(bool const &is_ok, std::string const &message) {
    using convertion::convert;
    using std::string;

    if (is_ok) {
        return ERROR_SUCCESS;
    } else {
        auto error_code = ::GetLastError();

        if (checkOneOf<int32_t>(
            error_code,
            ERROR_IO_INCOMPLETE,
            ERROR_BROKEN_PIPE,
            ERROR_HANDLE_EOF)
        )
            return error_code; // ----->

        LPSTR  buffer = nullptr;
        size_t size   = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, error_code,
            MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT),
            reinterpret_cast<LPSTR>(&buffer), 0, NULL);

        std::string api_message(buffer, size);
        LocalFree(buffer);

        throw std::runtime_error(
            message + ": " + api_message + "("+
            convert<string, uint64_t>(error_code, uint8_t{16}) + ")");
            // ----->
    }
}


CMultiplexer::CMultiplexer()
:
    m_iocp
        (INVALID_HANDLE_VALUE),
    m_wake_events
        (CAsyncQueue<Event::TSharedPtr>::create())
{}


void CMultiplexer::initialize() {
    // LOGT << "initialize ...";
    try {
        m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
        assertOK(m_iocp, "CreateIoCompletionPort");
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer initializing error: ") + e.what()); // ----->
    }
    // LOGT << "initialize OK";
}


void CMultiplexer::finalize() {
    // LOGT << "finalize ...";
    try {
        if (m_iocp == INVALID_HANDLE_VALUE)
            return; // ----->

        std::unordered_map<IStream::TSharedPtr, HANDLE> map_stream_wait_pid_handle;
        {
            LOCK_SCOPE();
            map_stream_wait_pid_handle = std::move(m_map_stream_wait_pid_handle);
        }

        for (auto const &stream_wait_pid_handle: map_stream_wait_pid_handle) {
            assertOK(
                UnregisterWaitEx(stream_wait_pid_handle.second, INVALID_HANDLE_VALUE),
               "UnregisterWaitEx"
            );
        }

        // post finalization signal to wake up any waiting threads
        PostQueuedCompletionStatus(m_iocp, 0, FINALIZE_COMPLETION_KEY, nullptr);
        // CancelIoEx(m_iocp, nullptr);
        // assertOK(
        //     CloseHandle(m_iocp),
        //     "CloseHandle");
        // m_iocp = INVALID_HANDLE_VALUE;

    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer finalization error: ") + e.what()); // ----->
    }
    // LOGT << "finalize OK";
}


std::list<Event::TSharedPtr> CMultiplexer::waitEvents() {
    std::list<Event::TSharedPtr> result;

    try {
        if (m_iocp == INVALID_HANDLE_VALUE)
            return {}; // ----->
            // throw std::runtime_error("iocp is not initialized");

        OVERLAPPED *overlapped                  = nullptr;
        DWORD       number_of_bytes_transfered  = 0;
        ULONG_PTR   completion_key              = 0;

        // LOGT << "wait ...";
        bool is_ok = GetQueuedCompletionStatus(
            m_iocp,
            &number_of_bytes_transfered,
            &completion_key,
            &overlapped, 1000);
        // LOGT << "wait, is_ok: " << is_ok;

        // if (is_ok)
        // LOGT << "\nGetQueuedCompletionStatus\n  is_ok: "  << is_ok
        //     << "\n  number_of_bytes_transfered: " << (uint32_t)number_of_bytes_transfered
        //     << "\n  completion_key: " << completion_key
        //     << "\n  overlapped is null: " << (overlapped == nullptr)
        //     << "\n  pointer: " << (overlapped ? (uint64_t)overlapped->Pointer : 0);

        // threading::sleep(1000);

        if  (is_ok) {
            // finalization signal: wake up the multiplexer thread to exit
            if (completion_key == FINALIZE_COMPLETION_KEY) {
                // LOGT << "finalization signal received";
                assertOK(
                    CloseHandle(m_iocp),
                   "CloseHandle");

                m_iocp = INVALID_HANDLE_VALUE;

                return {};  // return empty list to trigger thread exit check
            }

            // special wake signal
            if (completion_key == WAKE_COMPLETION_KEY) {
                // LOGT << "wake signal";
                if (m_wake_events)
                    result.splice(result.end(), m_wake_events->pop(false));
            }

            else

            if (completion_key) {
                // LOGT << "waitEvents, fd: " << uint64_t(completion_key);

                auto i  = m_map_id_stream.find(reinterpret_cast<HANDLE>(completion_key));
                if  (i != m_map_id_stream.end()) {
                    auto stream = i->second;
                    auto raw_o  = reinterpret_cast<ULONG_PTR>(overlapped->Pointer);

                    if (raw_o > std::numeric_limits<int>::max()) {
                        throw std::runtime_error(
                            "invalid overlapped operation code: " +
                            std::to_string(raw_o));
                    }

                    io::Event::TOperation o = static_cast<io::Event::TOperation>
                        (raw_o & std::numeric_limits<int>::max());

                    // LOGT << "overlapped operation: " << (intptr_t)overlapped->Pointer << " " << o;

                    result.push_back(
                        Event::create(
                            stream, o,
                            Event::TStatus::BEGIN));
                } else {
                    LOGW << "wait event got unsubscribed id: " << uint64_t(completion_key);
                }
            } else {
                // LOGT << "no completion key";
            }
        } else {
            // LOGT << "waitEvents: timeout";
            auto code  = GetLastError();
            if  (code != ERROR_TIMEOUT)
                assertOK(code, "GetQueuedCompletionStatus");
        }
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer waiting events error: ") + e.what()); // ----->
    }

    //{
    //    std::string events;
    //    for (auto const &ev : result) {
    //        events += "\n"
    //            + convert<std::string>(ev->operation) + " "
    //            + convert<std::string>(ev->status) + " "
    //            + convert<std::string>(ev->stream->getHandles().front());
    //    }
    //    LOGT << "events:" << events;

    //}
    //LOGT << "result: size = " << result.size();
    return result; // ----->
}


void CMultiplexer::subscribe(IStream::TSharedPtr const &stream) {
    // LOGT << "subscribe: " << stream->getHandles();
    try {
        if (m_iocp == INVALID_HANDLE_VALUE)
            throw std::runtime_error("iocp is not initialized"); // ----->

        assertExists(stream, "stream is null"); // ----->

        bool is_added = false;

        for (auto const &handle_ : stream->getHandles()) {

            if (handle_.first == IStream::THandleType::PID) {
                HANDLE hProc = reinterpret_cast<HANDLE>(handle_.second);
                HANDLE pid_handle_copy = nullptr;

                assertOK(
                    DuplicateHandle(
                        GetCurrentProcess(),
                        hProc,
                        GetCurrentProcess(),
                       &pid_handle_copy,
                        SYNCHRONIZE,
                        FALSE,
                        DUPLICATE_SAME_ACCESS),
                   "DuplicateHandle");

                struct TContext {
                    CMultiplexer
                       *multiplexer;
                    IStream::TSharedPtr
                        stream;
                    HANDLE
                        pid_handle_copy;
                };

                auto *context_ = new TContext {
                    this,
                    stream,
                    pid_handle_copy
                };

                HANDLE wait_pid_handle = nullptr;

                assertOK(
                    RegisterWaitForSingleObject(
                        &wait_pid_handle, pid_handle_copy,
                        [] (PVOID p, BOOLEAN b) {
                            //if (!b)
                            //    return;
                            auto *context = static_cast<TContext *>(p);
                            if (!context || !context->multiplexer || context->multiplexer->m_iocp == INVALID_HANDLE_VALUE)
                                return;
                            context->multiplexer->wake(
                                Event::create(context->stream, Event::TOperation::CLOSE, Event::TStatus::BEGIN));
                            if (context->pid_handle_copy)
                                CloseHandle(context->pid_handle_copy);
                            delete context;
                        },
                        context_,
                        INFINITE,
                        WT_EXECUTEONLYONCE),
                   "RegisterWaitForSingleObject");

                m_map_stream_wait_pid_handle[stream] = wait_pid_handle;
                continue; // <---
            }

            auto operation = Event::TOperation::UNKNOWN;
            auto handle_type = static_cast<IStream::THandleType>(handle_.first);

            if (handle_type == IStream::THandleType::READER)
                operation = Event::TOperation::READ;

            if (handle_type == IStream::THandleType::WRITER)
                operation = Event::TOperation::WRITE;

            // skip pid handle type
            if (operation == Event::TOperation::UNKNOWN)
                continue; // <---

            auto handle          = reinterpret_cast<HANDLE>      (handle_.second);
            auto completion_key  = reinterpret_cast<ULONG_PTR>   (handle);

            // LOGT << "subscribe handle: " << handle_;
            assertOK(
                CreateIoCompletionPort(handle, m_iocp, completion_key, 0),
               "CreateIoCompletionPort"
            );

            {
                //LOCK_SCOPE();
                m_map_id_stream[handle] = stream;
                is_added = true;
            }

            // Post artificial OPEN event
            auto overlapped     = new OVERLAPPED{};
            overlapped->Pointer = reinterpret_cast<PVOID>(
                static_cast<intptr_t>(operation)
            );
            // hEvent is not used for PostQueuedCompletionStatus — leave it zero

            assertOK(
                PostQueuedCompletionStatus(m_iocp, 0, completion_key, overlapped),
               "PostQueuedCompletionStatus"
            );
        }

        if (is_added) {
            m_wake_events->push(
                Event::create(
                    stream,
                    Event::TOperation::OPEN,
                    Event::TStatus::END));
        }

    } catch (std::exception const &e) {
        throw std::runtime_error("multiplexer subscribing error: " + std::string(e.what()));
    }
}


void CMultiplexer::unsubscribe(IStream::TSharedPtr const &stream) {
    try {
        if (m_iocp == INVALID_HANDLE_VALUE)
            throw std::runtime_error("iocp is not initialized"); // ----->

        assertExists(stream, "stream is null"); // ----->

        for (auto const &handle: assertExists(stream, "stream is null")->getHandles()) {
            auto handle_ = reinterpret_cast<HANDLE>(handle.second);

            if (static_cast<IStream::THandleType>(handle.first) == IStream::THandleType::PID) {
                LOGT << "unregister pid: " << handle.first;

                HANDLE pid_handle;
                {
                    LOCK_SCOPE();
                    pid_handle = m_map_stream_wait_pid_handle[stream];
                    m_map_stream_wait_pid_handle.erase(stream);
                }
                // assertOK(
                    UnregisterWaitEx(
                        pid_handle,
                        INVALID_HANDLE_VALUE);
                //    "UnregisterWaitEx");

                continue; // <---
            }

            {
                LOCK_SCOPE();
                auto id_stream = m_map_id_stream.find(handle_);
                if (id_stream == m_map_id_stream.end())
                    continue; // <---
                else
                    m_map_id_stream.erase(id_stream);
            }

            // LOGT << "cancelIO fd: " << reinterpret_cast<uintptr_t>(handle_);
            // assertOK(
                CancelIoEx(handle_, nullptr);
                // "CancelIo(" + convert<std::string>(reinterpret_cast<uintptr_t>(handle_)) + ")");
        }
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer unsubscribing error: ") + e.what()); // ----->
    } catch (...) {
        throw std::runtime_error(std::string("multiplexer unsubscribing unknown error")); // ----->
    }
    wake(Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::END));
}


void CMultiplexer::wake(Event::TSharedPtr const& event) {
    try {
        if (m_iocp == INVALID_HANDLE_VALUE)
            return; // ----->

        m_wake_events->push(event);

        assertOK(
            PostQueuedCompletionStatus(m_iocp, 0, WAKE_COMPLETION_KEY, nullptr),
           "PostQueuedCompletionStatus");
    }
    catch (std::exception const& e) {
        throw std::runtime_error(std::string("multiplexer wake error: ") + e.what());
    }
}


void CMultiplexer::wake(std::list<Event::TSharedPtr> const &events) {
    try {
        if (events.empty() || m_iocp == INVALID_HANDLE_VALUE)
            return; // ----->

        m_wake_events->push(events);

        assertOK(
            PostQueuedCompletionStatus(m_iocp, 0, WAKE_COMPLETION_KEY, nullptr),
           "PostQueuedCompletionStatus");
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer wake error: ") + e.what()); // ----->
    }
}


} // iridium::io::implementation::platform


#endif // WINDOWS_PLATFORM
