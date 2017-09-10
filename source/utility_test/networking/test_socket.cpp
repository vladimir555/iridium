#include <gtest/gtest.h>

#include <utility/networking/socket.h>
#include <utility/networking/implementation/socket.h>

#include <utility/networking/client/socket.h>
#include <utility/networking/client/implementation/socket.h>

#include <utility/convertion/convert.h>

#include <utility/threading/implementation/thread.h>
#include <utility/threading/implementation/runnuble.h>

#include <utility/threading/worker_pool.h>
#include <utility/threading/implementation/worker_pool.h>
#include <utility/threading/implementation/worker.h>

#include <utility/networking/dns.h>


using namespace std;
using utility::convertion::convert;
using utility::networking::implementation::CSocket;
using utility::threading::implementation::CThread;
using utility::threading::implementation::CWorker;
using utility::threading::implementation::CWorkerPool;
using utility::threading::implementation::CRunnuble;
using utility::threading::sleep;
using utility::threading::getThreadID;
using utility::threading::IWorkerPool;
using utility::threading::IWorkerHandler;
using utility::threading::IAsyncQueue;
using utility::threading::IThread;
using utility::threading::implementation::CAsyncQueue;
using utility::networking::getIPv4ByHost;


namespace utility {
namespace networking {
namespace socket {


class SocketAcceptorA: public IWorkerHandler<ISocketStream::TSharedPtr> {
public:
    DEFINE_CREATE(SocketAcceptorA)
    SocketAcceptorA() = default;
   ~SocketAcceptorA() = default;

    void handleItems(TItems const &items) override {
        for (auto const &i: items) {
            auto read_packet = i->read();
            string message = "<html><head><title>utility</title></head>"
                             "<body>answer from thread " + getThreadID() + "</body></html>\n";
            //sleep(5000);
            auto write_packet = convert<ISocket::TPacket>(string(
                "HTTP/1.1 200 OK\n"
                "Server: utility/1.0.0\n"
                "Date: Sat, 08 Mar 2014 22:53:46 GMT\n"
                "Content-Type: text/html\n"
                "Content-Length: " + convert<string>(message.size()) + "\n"
                "Last-Modified: Sat, 08 Mar 2014 22:53:30 GMT\n"
                "Connection: keep-alive\n"
                "Accept-Ranges: bytes\n\n\n"
                ) + message);

            i->write(write_packet);
            i->close();
        }
    }

    void handleStart() override {
        //cout << "socket acceptor pool thread " << getThreadID() << " start" << endl;
    }

    void handleStop() override {
        //cout << "socket acceptor pool thread " << getThreadID() << " stop" << endl;
    }
};


class SocketServer: public CRunnuble {
public:
    DEFINE_CREATE(SocketServer)
    SocketServer() {
        m_socket_server         = CSocket::create(URL("127.0.0.1:55555"));
        list<IWorkerHandler<ISocketStream::TSharedPtr>::TSharedPtr> acceptors;
        for (size_t i = 0; i < 10; i++)
            acceptors.push_back(SocketAcceptorA::create());
        m_socket_acceptor_pool  = CWorkerPool<ISocketStream::TSharedPtr>::create(
            acceptors, "socket_acceptor_pool");;
    }
    virtual ~SocketServer() = default;

    void run() {
        m_socket_acceptor_pool->initialize();
        try {
            m_socket_server->open();

            m_socket_server->listen();
            while(true) {
                auto s = m_socket_server->accept();
                m_socket_acceptor_pool->push(s);
            }
        } catch (std::exception const &e) {
            if (m_is_running)
                throw;
            else
                LOGT << "interrupted";
        }
    }

    void stop() {
        CRunnuble::stop();

        m_socket_acceptor_pool->finalize();

        LOGT << "interrupt";
        m_socket_server->interrupt();
        LOGT << "close";
        m_socket_server->close();
    }

    ISocket::TSharedPtr m_socket_server;
    IWorkerPool<ISocketStream::TSharedPtr>::TSharedPtr m_socket_acceptor_pool;
};


TEST(networking, socket) {
    {
        utility::parsing::INode::TSharedPtr root =
            utility::parsing::implementation::CNode::create("logger");
        root->addChild("console-sink");
        utility::logging::Logger::instance().update(utility::logging::config::TLogger(root));

//        testKEvent();
//        return;
    }

//    SocketServer::TSharedPtr socket_server = SocketServer::create();
//    IThread::TSharedPtr socket_server_thread = CThread::create(socket_server, "socket server");

//    socket_server_thread->initialize();

//    LOGT << "waiting begin";
//    sleep(50000);
//    LOGT << "waiting end";

//    socket_server_thread->finalize();

    ISocket::TSharedPtr s = CSocket::create(URL("tcp://127.0.0.1:55555"));
    s->open();
    s->listen();
    while(true) {
        LOGT << "sleep\n";
        sleep(10000);
        auto a = s->accept();
        LOGT << "accepted count " << a.size();
        for (auto &i: a) {
            LOGT << 11;
            auto p = static_cast<ISocketStream::TSharedPtr>(i)->read();
            //LOGT << "packet " << p;
            string message = "<html><head><title>utility</title></head>"
                             "<body>answer from thread " + getThreadID() + " !</body></html>\n";
            p = convert<ISocket::TPacket>(string(
                "HTTP/1.1 200 OK\n"
                "Server: utility/1.0.0\n"
                "Date: Sat, 08 Mar 2014 22:53:46 GMT\n"
                "Content-Type: text/html\n"
                "Content-Length: " + convert<string>(message.size()) + "\n"
                "Last-Modified: Sat, 08 Mar 2014 22:53:30 GMT\n"
                "Connection: keep-alive\n"
                "Accept-Ranges: bytes\n\n\n"
                ) + message);
            static_cast<ISocketStream::TSharedPtr>(i)->write(p);
            //static_cast<ISocketStream::TSharedPtr>(i)->close();
            //LOGT << "packet " << p;
        }
    }

    LOGT << "end";

}


TEST(networking, dns) {

    auto ipv4 = getIPv4ByHost("ya.ru");

    cout << "ya.ru ip: "
         << static_cast<int>(ipv4[0]) << "."
         << static_cast<int>(ipv4[1]) << "."
         << static_cast<int>(ipv4[2]) << "."
         << static_cast<int>(ipv4[3]) << endl;

    ASSERT_THROW(getIPv4ByHost("ya.rur"), std::exception);
}


} // socket
} // networking
} // utility
