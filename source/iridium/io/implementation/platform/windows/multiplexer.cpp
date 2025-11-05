#include "multiplexer.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/event.h"

#include <stdint.h>


#include <iridium/logging/logger.h>
namespace iridium {
namespace io {
namespace implementation {
namespace platform {


template<typename TResult>
TResult assertOK(TResult const &result, std::string const &method) {
    if (result) {
        return result; // ----->
    } else {
        DWORD error_message_code    = ::GetLastError();
        LPSTR buffer                = nullptr;

        size_t size = FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER  | 
            FORMAT_MESSAGE_FROM_SYSTEM      | 
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, error_message_code, 
            MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), 
            (LPSTR)&buffer, 0, NULL);

        std::string message(buffer, size);
        LocalFree(buffer);
        throw std::runtime_error(method + " error: " + 
            convertion::convert<std::string, uint64_t>(error_message_code) + " " + message); // ----->
    }
}


CMultiplexer::CMultiplexer()
:
    m_iocp(nullptr)
{}


void CMultiplexer::initialize() {
    LOCK_SCOPE();

    try {
        m_iocp = assertOK(CreateIoCompletionPort(INVALID_HANDLE_VALUE, nullptr, 0, 0), "CreateIoCompletionPort");
    } catch (std::exception const &e) {
        throw std::runtime_error("multiplexer initializing error: " + std::string(e.what())); // ----->
    }
}


void CMultiplexer::finalize() {
    //LOCK_SCOPE();
    //LOGT << "finalize ...";
    try {
        if (m_iocp)
            CancelIo(m_iocp);
            assertOK(CloseHandle(m_iocp), "CloseHandle");
    } catch (std::exception const &e) {
        throw std::runtime_error("multiplexer finalization error: " + std::string(e.what())); // ----->
    }
    //LOGT << "finalize OK";
}


std::list<Event::TSharedPtr> CMultiplexer::waitEvents() {
    std::list<Event::TSharedPtr> result;

    try {
        OVERLAPPED *overlapped                  = nullptr;
        DWORD       number_of_bytes_transfered  = 0;
        ULONG_PTR   completion_key              = 0;
    
        bool is_ok = GetQueuedCompletionStatus(
            m_iocp, 
            &number_of_bytes_transfered, 
            &completion_key, 
            &overlapped, 1000);
        //LOGT << "GetQueuedCompletionStatus: " << is_ok << " " << number_of_bytes_transfered << " " << completion_key << " " << (overlapped != nullptr);
        if  (is_ok) {
            if (completion_key) {
                //LOGT << "waitEvents: id: " << uint64_t(completion_key);

                LOCK_SCOPE();

                auto i  = m_map_id_stream.find(reinterpret_cast<HANDLE>(completion_key));
                if  (i != m_map_id_stream.end()) {
                    auto stream = i->second;

                    io::Event::TOperation o = static_cast<io::Event::TOperation>(
                        reinterpret_cast<intptr_t>(overlapped->Pointer));

                    //LOGT << "overlapped operation: "  << (intptr_t)overlapped->Pointer << " " << o;

                    result.push_back(
                        Event::create(
                            stream, o, 
                            Event::TStatus::END));
                } else {
                    LOGW << "wait event got unsubscribed id: " << uint64_t(completion_key);
                }
            }
        } else {
            //LOGT << "waitEvents: timeout";
            auto code  = GetLastError();
            if  (code != ERROR_TIMEOUT)
                assertOK(code, "GetQueuedCompletionStatus");
        }
    } catch (std::exception const &e) {
        throw std::runtime_error("multiplexer waiting events error: " + std::string(e.what())); // ----->
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
    //LOGT << "subscribe ...";

    if(!stream) {
        LOGW << "subscribe stream null";
        return; // ----->
    }

    auto handles = stream->getHandles();

    if (handles.empty()) {
        LOGW << "subscribe stream with empty handles";
        return; // ----->
    }

    try {
        for (auto const &handle_: handles) {
            if (!handle_)
                continue; // <---

            HANDLE      handle          = reinterpret_cast<HANDLE>      (handle_);
            ULONG_PTR   completion_key  = reinterpret_cast<ULONG_PTR>   (handle);

            //LOGT << "subscribe fd: " << uint64_t(handle);
            assertOK(m_iocp == CreateIoCompletionPort(handle, m_iocp, completion_key, 0), "CreateIoCompletionPort");
            //LOGT << "subscribe OK";

            {
                LOCK_SCOPE();
                m_map_id_stream[handle] = stream;
            }

            auto overlapped     = new OVERLAPPED{ 0 };
            overlapped->Pointer = reinterpret_cast<PVOID>(static_cast<intptr_t>(Event::TOperation::OPEN));
            overlapped->hEvent  = CreateEvent(nullptr, false, false, nullptr);

            assertOK(
                PostQueuedCompletionStatus(
                    m_iocp, 0, 
                    completion_key, 
                    overlapped),
                "send event OPEN END");

            // todo: catch -> close handle event

            //assertOK(
            //    CreateIoCompletionPort(
            //        handle, 
            //        m_iocp, 
            //        reinterpret_cast<ULONG_PTR>(overlapped), 0), 
            //    "create event OPEN END");

            //assertOK(ReadFile(handle, nullptr, 0, nullptr, &overlapped), "ReadFile");
            //assertOK(WaitForSingleObject(overlapped.hEvent, 1000) == WAIT_OBJECT_0, "WaitForSingleObject");
        }

    } catch (std::exception const &e) {
        throw std::runtime_error("multiplexer subscribing error: " + std::string(e.what())); // ----->
    }
}


void CMultiplexer::unsubscribe(IStream::TSharedPtr const &stream) {
    for (auto const &handle_ : stream->getHandles()) {
        HANDLE handle = reinterpret_cast<HANDLE>(handle_);
        if (!stream || handle) {
            //LOGW << "unsubscribe stream with null id";
            return; // ----->
        }
        try {
            //LOGT << "unsubscribe id: " << uint64_t(handle);
            if (handle)
                CancelIo(handle);
            //LOGT << "unsubscribe OK";

            LOCK_SCOPE();
            m_map_id_stream.erase(handle);
        } catch (std::exception const &e) {
            throw std::runtime_error("multiplexer unsubscribing error: " + std::string(e.what())); // ----->
        }
    }
}


void CMultiplexer::wake(Event::TSharedPtr const &) {
    // todo:
}


} // platform
} // implementation
} // io
} // iridium


#endif // WINDOWS_PLATFORM
