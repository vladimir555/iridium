#include <gtest/gtest.h>
#include <iostream>

#include <utility/networking/socket.h>
#include <utility/networking/implementation/socket.h>

#include <utility/networking/client/socket.h>
#include <utility/networking/client/implementation/socket.h>

#include <utility/convertion/convert.h>

#include <utility/threading/implementation/thread.h>
#include <utility/threading/implementation/runnuble.h>


using namespace std;
using utility::convertion::convert;
using utility::networking::implementation::CSocket;
using utility::threading::implementation::CThread;
using utility::threading::implementation::CRunnuble;
using utility::threading::IThread;


namespace utility {
namespace networking {
namespace socket {


class SocketServer: public CRunnuble {
public:
    DEFINE_CREATE(SocketServer)
    SocketServer() {
        socket_server = CSocket::create(URL("127.0.0.1:55555"));
    }
    virtual ~SocketServer() = default;

    void run() {
        try {
            socket_server->open();

            socket_server->listen();
            auto s = socket_server->accept();
            auto p = s->read();

            cout << "read: " << endl << convert<string>(p) << endl;

            socket_server->close();
        } catch (std::exception &e) {
            if (m_is_running)
                throw;
            else
                cout << "interrupted" << endl;
        }
    }

    void stop() {
        CRunnuble::stop();
        cout << "interrupt" << endl;
        socket_server->interrupt();
        cout << "close" << endl;
        socket_server->close();
    }

    ISocket::TSharedPtr socket_server;
};


TEST(networking, socket) {
//    SocketServer::TSharedPtr  socket_server = SocketServer::create();
//    IThread::TSharedPtr socket_server_thread = CThread::create(socket_server, "");
//    client::ISocket::TSharedPtr socket_client = client::implementation::CSocket::create(URL("127.0.0.1:55555"));

//    socket_server_thread->initialize();

//    cout << "waiting begin" << endl;
//    //IThread::sleep(1000);
//    socket_client->initialize();
//    //auto p = convert<ISocket::packet_t>(string("hello!"));
//    //socket_client->write(p);
//    IThread::sleep(1000);

////    IThread::sleep(1000);
//    cout << "waiting end" << endl;

//    socket_server_thread->finalize();

//    IThread::sleep(1000);
//    socket_client->finalize();
}


} // socket
} // networking
} // utility
