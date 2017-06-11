#ifndef HEADER_SOCKET_47A3CECD_0FF4_40FF_AF19_67FC1ED75B78
#define HEADER_SOCKET_47A3CECD_0FF4_40FF_AF19_67FC1ED75B78


#include "utility/networking/url.h"
#include "utility/networking/server/socket.h"

#include "utility/networking//socket.h"

#include "utility/threading/implementation/runnuble.h"
#include "utility/threading/thread.h"
#include "utility/threading/async_queue.h"


namespace utility {
namespace networking {
namespace server {
namespace implementation {


class CSocket: public ISocket {
public:
    typedef threading::IAsyncQueue<ISocketStream::TSharedPtr> TSocketStreamQueue;
    CSocket(URL const &url, TSocketStreamQueue::TSharedPtr const &socket_stream_queue);
    DEFINE_CREATE(CSocket)
    virtual ~CSocket() = default;

    void initialize() override;
    void finalize() override;

private:
    class Acceptor: public threading::implementation::CRunnuble {
    public:
        DEFINE_CREATE(Acceptor)
        Acceptor(URL const &url, TSocketStreamQueue::TSharedPtr const &socket_stream_queue);
        virtual ~Acceptor() = default;

        void run() override;
        void stop() override;
    private:
        networking::ISocket::TSharedPtr m_socket;
        TSocketStreamQueue::TSharedPtr  m_socket_stream_queue;

    };

    threading::IRunnable::TSharedPtr    m_runnuble;
    threading::IThread::TSharedPtr      m_thread;
};


} // implementation
} // server
} // networking
} // utility


#endif // HEADER_SOCKET_47A3CECD_0FF4_40FF_AF19_67FC1ED75B78
