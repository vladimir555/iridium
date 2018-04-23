#include <gtest/gtest.h>

#include <utility/networking/client/socket.h>
#include <utility/networking/client/implementation/socket.h>
#include <utility/networking/server/socket.h>
#include <utility/networking/server/implementation/socket.h>
#include <utility/networking/dns.h>

#include <utility/threading/worker_pool.h>
#include <utility/threading/implementation/thread.h>
#include <utility/threading/implementation/runnuble.h>
#include <utility/threading/implementation/worker_pool.h>
#include <utility/threading/implementation/worker.h>

#include <utility/convertion/convert.h>


using namespace std;

using utility::networking::getIPv4ByHost;

using utility::threading::IWorkerPool;
using utility::threading::IWorkerHandler;
using utility::threading::IAsyncQueue;
using utility::threading::IThread;
using utility::threading::getThreadID;
using utility::threading::sleep;
using utility::threading::implementation::CAsyncQueue;
using utility::threading::implementation::CThread;
using utility::threading::implementation::CWorker;
using utility::threading::implementation::CWorkerPool;
using utility::threading::implementation::CRunnuble;

using utility::convertion::convert;


namespace utility {
namespace networking {
namespace socket {


class SocketAcceptorA: public IWorkerHandler<ISocketStream::TSharedPtr> {
public:
    DEFINE_CREATE(SocketAcceptorA)
    SocketAcceptorA() = default;
   ~SocketAcceptorA() = default;

    TItems handle(TItems const &items) override {
        cout << "item" << endl;
        for (auto const &i: items) {
            try {
                auto packet = i->read();
//                cout << "packet size " << packet.size() << endl;
//                cout << "packet: " << convert<string>(packet) << endl;

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
            } catch (std::exception const &e) {
                cout << "socket error: " << e.what() << endl;
            }
        }
        return TItems();
    }

    void initialize() override {
        //cout << "socket acceptor pool thread " << getThreadID() << " start" << endl;
    }

    void finalize() override {
        //cout << "socket acceptor pool thread " << getThreadID() << " stop" << endl;
    }
};


TEST(networking, socket) {
//    URL url("tcp://127.0.0.1:55555");

//    server::ISocket::TSocketStreamsHandlers handlers;
//    handlers.push_back(SocketAcceptorA::create());
//    server::ISocket::TSharedPtr server_socket = server::implementation::CSocket::create(URL(url), handlers);

//    server_socket->initialize();

//    client::ISocket::TSharedPtr client_socket = client::implementation::CSocket::create(url);
//    //sleep(5000);

//    // todo: tests
//    for (int i = 0; i < 5; i++)
//    try {
//        client_socket->initialize();
//        client_socket->write(convert<client::ISocket::TPacket>(string("test\n")));
//        client_socket->finalize();
//        break;
//    } catch (std::exception const &e) {
//        cout << "error: " << e.what() << endl;
//    }

//    server_socket->finalize();
}


//TEST(networking, dns) {
//    auto ipv4 = getIPv4ByHost("ya.ru");
//
//    cout << "ya.ru ip: "
//         << static_cast<int>(ipv4[0]) << "."
//         << static_cast<int>(ipv4[1]) << "."
//         << static_cast<int>(ipv4[2]) << "."
//         << static_cast<int>(ipv4[3]) << endl;
//
//    ASSERT_THROW(getIPv4ByHost("ya.rur"), std::exception);
//}


// -----


//class IProtocol {
//public:
//    DEFINE_SMART_PTR(IProtocol)
//    IProtocol() = default;
//    virtual ~IProtocol() = default;

//    class IPacket {
//    public:
//        DEFINE_SMART_PTR(IPacket)
//        IPacket() = default;
//        virtual ~IPacket() = default;

//        ISocket::TPacket get(size_t const &lpos, size_t const &rpos);
//    };

//    IPacket::TSharedPtr exchange(IPacket::TSharedPtr const &request);
//};


//class ISocketAction {
//public:
//    DEFINE_INTERFACE(ISocketAction)
//    virtual void doAction() = 0;
//};


//class SocketActionRead {
//public:
//    DEFINE_CREATE(SocketActionRead)
//    void doAction();
//};


//class SocketActionWrite {
//public:
//    DEFINE_CREATE(SocketActionRead)
//    void doAction();
//};


//TEST(networking, socket_action) {

//}


} // socket
} // networking
} // utility
