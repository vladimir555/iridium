#include "multiplexer.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/assert.h"
#include "iridium/items.h"
#include "iridium/io/event.h"
#include "iridium/threading/implementation/async_queue.h"

#include <stdint.h>
#include <limits>


using iridium::threading::implementation::CAsyncQueue;


#include <iridium/logging/logger.h>
namespace iridium::io::implementation::platform {


static constexpr ULONG_PTR WAKE_COMPLETION_KEY = static_cast<ULONG_PTR>(-1);


// todo: mv to common place
DWORD CMultiplexer::checkResult(bool const &is_ok, std::string const &message) {
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
    m_iocp(nullptr),
    m_wake_events(CAsyncQueue<Event::TSharedPtr>::create())
{}


void CMultiplexer::initialize() {
    // LOGT << "initialize ...";
    try {
        m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0);
        checkResult(m_iocp, "CreateIoCompletionPort");
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer initializing error: ") + e.what()); // ----->
    }
    // LOGT << "initialize OK";
}


void CMultiplexer::finalize() {
    // LOGT << "finalize ...";
    try {
        if (m_iocp)
            CancelIo(m_iocp);
            checkResult(
                CloseHandle(m_iocp),
               "CloseHandle");
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer finalization error: ") + e.what()); // ----->
    }
    // LOGT << "finalize OK";
}


std::list<Event::TSharedPtr> CMultiplexer::waitEvents() {
    std::list<Event::TSharedPtr> result;

    try {
        assertExists(m_iocp, "iocp is not initialized");

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


        // LOGT << "\nGetQueuedCompletionStatus\n  is_ok: "  << is_ok
        //     << "\n  number_of_bytes_transfered: " << (uint32_t)number_of_bytes_transfered
        //     << "\n  completion_key: " << completion_key
        //     << "\n  overlapped is null: " << (overlapped == nullptr)
        //     << "\n  pointer: " << (overlapped ? (uint64_t)overlapped->Pointer : 0);

        if  (is_ok) {
            // special wake signal
            if (completion_key == WAKE_COMPLETION_KEY) {
                // LOGT << "wake signal";
                if (m_wake_events)
                    result.splice(result.end(), m_wake_events->pop(false));
            } else
            if (completion_key) {
                // LOGT << "waitEvents, fd: " << uint64_t(completion_key);

                LOCK_SCOPE();

                auto i  = m_map_id_stream.find(reinterpret_cast<HANDLE>(completion_key));
                if  (i != m_map_id_stream.end()) {
                    auto stream = i->second;

                    auto raw_o  = reinterpret_cast<ULONG_PTR>(overlapped->Pointer);

                    if (raw_o > std::numeric_limits<int>::max()) {
                        throw std::runtime_error(
                            "invalid overlapped operation code: " +
                            convertion::convert<std::string>(raw_o));
                    }

                    io::Event::TOperation o = static_cast<io::Event::TOperation>
                        (raw_o & std::numeric_limits<int>::max());

                    // LOGT << "overlapped operation: " << (intptr_t)overlapped->Pointer << " " << o;

                    result.push_back(
                        Event::create(
                            stream, o,
                            Event::TStatus::END));
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
                checkResult(code, "GetQueuedCompletionStatus");
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
        assertExists(m_iocp, "iocp is not initialized");
        assertExists(stream, "stream is null");

        for (auto const &handle_ : stream->getHandles()) {
            if (!handle_)
                continue;

            auto handle          = reinterpret_cast<HANDLE>      (handle_);
            auto completion_key  = reinterpret_cast<ULONG_PTR>   (handle);

            // LOGT << "subscribe handle: " << handle_;
            checkResult(
                CreateIoCompletionPort(handle, m_iocp, completion_key, 0),
               "CreateIoCompletionPort"
            );

            {
                LOCK_SCOPE();
                m_map_id_stream[handle] = stream;
            }

            // Post artificial OPEN event
            auto overlapped     = new OVERLAPPED{};
            overlapped->Pointer = reinterpret_cast<PVOID>(
                static_cast<intptr_t>(Event::TOperation::OPEN)
            );
            // hEvent is not used for PostQueuedCompletionStatus — leave it zero

            checkResult(
                PostQueuedCompletionStatus(m_iocp, 0, completion_key, overlapped),
               "PostQueuedCompletionStatus"
            );
        }
    } catch (std::exception const &e) {
        throw std::runtime_error("multiplexer subscribing error: " + std::string(e.what()));
    }
}


void CMultiplexer::unsubscribe(IStream::TSharedPtr const &stream) {
    try {
        assertExists(m_iocp, "iocp is not initialized");

        for (auto const &handle: assertExists(stream, "stream is null")->getHandles()) {
            auto handle_ = reinterpret_cast<HANDLE>(handle);

            if (handle_) {
                checkResult(
                    CancelIo(handle_),
                   "CancelIo");

                LOCK_SCOPE();
                m_map_id_stream.erase(handle_);
            }
        }
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer unsubscribing error: ") + e.what()); // ----->
    }
}


void CMultiplexer::wake(Event::TSharedPtr const &event) {
    try {
        assertExists(m_iocp, "iocp is not initialized");

        if (event && event->stream) {
            m_wake_events->push(event);

            checkResult(
                PostQueuedCompletionStatus(m_iocp, 0, WAKE_COMPLETION_KEY, nullptr),
               "PostQueuedCompletionStatus");
        }
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer wake error: ") + e.what()); // ----->
    }
}


void CMultiplexer::wake(std::list<Event::TSharedPtr> const &events) {
    try {
        assertExists(m_iocp, "iocp is not initialized");

        if (!events.empty()) {
            m_wake_events->push(events);

            checkResult(
                PostQueuedCompletionStatus(m_iocp, 0, WAKE_COMPLETION_KEY, nullptr),
               "PostQueuedCompletionStatus");
        }
    } catch (std::exception const &e) {
        throw std::runtime_error(std::string("multiplexer wake error: ") + e.what()); // ----->
    }
}


} // iridium::io::implementation::platform


#endif // WINDOWS_PLATFORM
