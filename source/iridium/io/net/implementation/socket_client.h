#ifndef HEADER_SOCKET_CLIENT_EB673EE6_B48F_42B2_827B_C63CD492DE0F
#define HEADER_SOCKET_CLIENT_EB673EE6_B48F_42B2_827B_C63CD492DE0F


#include "iridium/io/stream.h"
#include "iridium/io/net/url.h"
#include "iridium/io/net/socket.h"
#include "iridium/io/listener.h"
#include "iridium/threading/async_queue.h"
#include "iridium/threading/worker.h"
#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"
#include "iridium/threading/synchronized.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {


class CStreamPortSynchronized: public IStreamPort, public threading::Synchronized {
public:
    DEFINE_IMPLEMENTATION(CStreamPortSynchronized)
    CStreamPortSynchronized(IStreamPort::TSharedPtr const &stream);

    Buffer::TSharedPtr read(size_t const &size) override;
    size_t write(Buffer::TSharedPtr const &buffer) override;
    int getID() const override;

    void initialize() override;
    void finalize() override;

private:
    IStreamPort::TSharedPtr m_stream;
};


class CSocketClient: public IStreamPort {
public:
    DEFINE_IMPLEMENTATION(CSocketClient)
    CSocketClient(URL const &url);

    Buffer::TSharedPtr read(size_t const &size) override;
    size_t write(Buffer::TSharedPtr const &buffer) override;

    void initialize() override;
    void finalize() override;

    int getID() const override;

private:
    struct Command {
        Event::TSharedPtr   event;
        Buffer::TSharedPtr  write_buffer;
        size_t              read_size = {};
    };

    typedef threading::IAsyncQueue<Buffer::TSharedPtr>::TSharedPtr  TBufferQueuePtr;
    typedef threading::IWorker<Command>::TSharedPtr                 TSocketWorkerPtr;

    class CSocketHandler: public threading::IWorkerHandler<Command> {
    public:
        DEFINE_IMPLEMENTATION(CSocketHandler)
        CSocketHandler(
            IStreamPort::TSharedPtr const &socket,
            TBufferQueuePtr const &read_buffers
        );

        void    initialize()    override;
        void    finalize()      override;

        TItems  handle(TItems const &commands) override;
    private:
        IStreamPort::TSharedPtr m_socket;
        TBufferQueuePtr     m_read_buffers;
        TBufferQueuePtr     m_write_buffers;
    };

    class CListenerHandler: public threading::IRunnable {
    public:
        DEFINE_IMPLEMENTATION(CListenerHandler)

        CListenerHandler(
            IStreamPort::TSharedPtr const &socket,
            TSocketWorkerPtr        const &socket_worker);

        void run(std::atomic<bool> &is_running) override;

        void initialize() override;
        void finalize() override;

    private:
        IStreamPort::TSharedPtr m_socket;
        IListener::TSharedPtr   m_listener;
        TSocketWorkerPtr        m_socket_worker;
    };

    IStreamPort::TSharedPtr m_socket;
    TBufferQueuePtr     m_read_buffers;
    TSocketWorkerPtr    m_socket_worker;
    threading::IThread::TSharedPtr m_listener_thread;
};


} // implementation
} // net
} // io
} // iridium


#endif // HEADER_SOCKET_CLIENT_EB673EE6_B48F_42B2_827B_C63CD492DE0F
