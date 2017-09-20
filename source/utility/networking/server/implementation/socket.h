#ifndef HEADER_SOCKET_47A3CECD_0FF4_40FF_AF19_67FC1ED75B78
#define HEADER_SOCKET_47A3CECD_0FF4_40FF_AF19_67FC1ED75B78


#include "utility/networking/server/socket.h"
#include "utility/networking/url.h"
#include "utility/networking//socket.h"

#include "utility/threading/implementation/runnuble.h"
#include "utility/threading/thread.h"
#include "utility/threading/worker_pool.h"

#include <list>


namespace utility {
namespace networking {
namespace server {
namespace implementation {


class CSocket: public ISocket {
public:
    CSocket(URL const &url, TSocketStreamsHandlers const &socket_stream_handlers);
    DEFINE_CREATE(CSocket)
    virtual ~CSocket() = default;

    void initialize() override;
    void finalize() override;

private:
    class Acceptor: public threading::implementation::CRunnuble {
    public:
        DEFINE_CREATE(Acceptor)
        Acceptor(URL const &url, TSocketStreamsHandlers const &socket_stream_handlers);
        virtual ~Acceptor() = default;

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
