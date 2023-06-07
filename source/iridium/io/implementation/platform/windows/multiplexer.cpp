#include "multiplexer.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/implementation/event.h"


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
            convertion::convert<std::string>(error_message_code) + " " + message); // ----->
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


std::list<IEvent::TSharedPtr> CMultiplexer::waitEvents() {
    std::list<IEvent::TSharedPtr> result;

    try {
        OVERLAPPED *overlapped                  = nullptr;
        DWORD       number_of_bytes_transfered  = 0;
        ULONG_PTR   completion_key              = 0;
    
        bool is_ok = GetQueuedCompletionStatus(m_iocp, &number_of_bytes_transfered, &completion_key, &overlapped, 1000);
        //LOGT << "GetQueuedCompletionStatus: " << is_ok << " " << number_of_bytes_transfered << " " << completion_key << " " << (overlapped != nullptr);
        if  (is_ok) {
            if (completion_key) {
                LOGT << "waitEvents: id: " << uint64_t(completion_key);

                LOCK_SCOPE();

                auto i  = m_map_id_stream.find(reinterpret_cast<IStream::TID>(completion_key));
                if  (i != m_map_id_stream.end()) {
                    auto stream = i->second;
                    // todo: unknown type
                    result.push_back(CEvent::create(stream, IEvent::TType::READ));
                    result.push_back(CEvent::create(stream, IEvent::TType::WRITE));
                } else {
                    //LOGW << "wait event got unsubscribed id: " << uint64_t(completion_key);
                }
            }
        } else {
            LOGT << "waitEvents: timeout";
            auto code  = GetLastError();
            if  (code != ERROR_TIMEOUT)
                assertOK(code, "GetQueuedCompletionStatus");
        }
    } catch (std::exception const &e) {
        throw std::runtime_error("multiplexer waiting events error: " + std::string(e.what())); // ----->
    }
    
    //LOGT << "result: size = " << result.size();
    return result; // ----->
}


void CMultiplexer::subscribe(IStream::TConstSharedPtr const &stream) {

    if (!stream) {
        //LOGW << "subscribe stream with null id";
        return; // ----->
    }

    try {
        std::const_pointer_cast<IStream>(stream)->initialize();

        if (!stream->getID())
            return; // ----->

        ULONG_PTR completion_key = reinterpret_cast<ULONG_PTR>(stream->getID());
        LOGT << "subscribe id: " << uint64_t(stream->getID());
        assertOK(m_iocp == CreateIoCompletionPort(stream->getID(), m_iocp, completion_key, 0), "CreateIoCompletionPort");
        //LOGT << "subscribe OK";

        {
            LOCK_SCOPE();
            m_map_id_stream[stream->getID()] = std::const_pointer_cast<IStream>(stream);
        }

        OVERLAPPED overlapped {};
        overlapped.hEvent = CreateEvent(nullptr, false, false, nullptr);
        assertOK(ReadFile(stream->getID(), nullptr, 0, nullptr, &overlapped), "ReadFile");
        assertOK(WaitForSingleObject(overlapped.hEvent, 1000) == WAIT_OBJECT_0, "WaitForSingleObject");

    } catch (std::exception const &e) {
        throw std::runtime_error("multiplexer subscribing error: " + std::string(e.what())); // ----->
    }
}


void CMultiplexer::unsubscribe(IStream::TConstSharedPtr const &stream) {

    if (!stream || !stream->getID()) {
        //LOGW << "unsubscribe stream with null id";
        return; // ----->
    }
    try {
        LOGT << "unsubscribe id: " << uint64_t(stream->getID());
        CancelIo(stream->getID());
        //LOGT << "unsubscribe OK";

        LOCK_SCOPE();
        m_map_id_stream.erase(stream->getID());
    } catch (std::exception const &e) {
        throw std::runtime_error("multiplexer unsubscribing error: " + std::string(e.what())); // ----->
    }
}


} // platform
} // implementation
} // io
} // iridium


#endif // WINDOWS_PLATFORM
