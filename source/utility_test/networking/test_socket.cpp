#include <gtest/gtest.h>
#include <iostream>

#include <utility/networking/socket.h>
#include <utility/networking/implementation/socket.h>
#include <utility/convertion/convert.h>
#include <utility/threading/implementation/thread.h>


using namespace std;
using utility::convertion::convert;
using utility::networking::implementation::CSocket;
using utility::threading::implementation::CThread;
using utility::threading::IThread;


namespace utility {
namespace networking {
namespace socket {


namespace test {


class SocketServer: public threading::IRunnable {
public:
    SocketServer() = default;
    //virtual ˜SocketServer() = default;

    void run() {
        ISocket::TSharedPtr socket_server = CSocket::create(URL("127.0.0.1:55555"));
        socket_server->open();

        auto s = socket_server->accept();
        auto p = s->read();

        socket_server->close();
    }

    void stop() {

    }
};


} // test


TEST(networking, socket) {
    test::SocketServer  socket_server;
    //IThread::TSharedPtr socket_server_thread = CThread::create(socket_server, "");
    ISocket::TSharedPtr socket_client = CSocket::create(URL("127.0.0.1:55555"));

    //socket_server_thread->initialize();

    IThread::sleep(10000);

//    socket_client->initialize();

    //socket_server_thread->finalize();


}


} // socket
} // networking
} // utility
