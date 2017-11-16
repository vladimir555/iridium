#ifndef HEADER_SOCKET_47A3CECD_0FF4_40FF_AF19_67FC1ED75B78
#define HEADER_SOCKET_47A3CECD_0FF4_40FF_AF19_67FC1ED75B78


#include "utility/networking/server/socket.h"
#include "utility/networking/url.h"
#include "utility/networking//socket.h"

#include "utility/threading/implementation/runnuble.h"
#include "utility/threading/thread.h"
#include "utility/threading/worker_pool.h"
#include "utility/threading/implementation/mutex.h"

#include <list>
#include <map>
#include <memory>
#include <chrono>


namespace utility {
namespace networking {
namespace server {
namespace implementation {


class CSocket: public ISocket, public std::enable_shared_from_this<CSocket> {
public:
    CSocket(URL const &url, TSocketStreamsHandlers const &socket_stream_handlers);
    DEFINE_SMART_PTR(CSocket)
    DEFINE_CREATE(CSocket)
    virtual ~CSocket() = default;

    void initialize() override;
    void finalize() override;

private:
    class CCachedSocketStream: public ISocketStream {
    public:
        CCachedSocketStream(ISocketStream::TSharedPtr const &source_socket);
        virtual ~CCachedSocketStream() = default;
        DEFINE_CREATE(CCachedSocketStream)

        void    write(TPacket const &packet) override;
        TPacket read() override;
        void    close() override;
        URL     getURL() const override;

    private:
        TPacket                             m_cache;
        ISocketStream::TSharedPtr           m_source_socket;
        std::chrono::high_resolution_clock::time_point m_last_read_time;
    };

    class Acceptor: public threading::implementation::CRunnuble {
    public:
        Acceptor(URL const &url, TSocketStreamsHandlers const &socket_stream_handlers);
        virtual ~Acceptor() = default;
        DEFINE_CREATE(Acceptor)

        void run() override;
        void stop() override;

    private:
        networking::ISocket::TSharedPtr         m_socket;
        TSocketStreamsWorkerPool::TSharedPtr    m_worker_pool;
    };

    threading::IRunnable::TSharedPtr    m_runnuble;
    threading::IThread::TSharedPtr      m_thread;
};


} // implementation
} // server
} // networking
} // utility


#endif // HEADER_SOCKET_47A3CECD_0FF4_40FF_AF19_67FC1ED75B78
