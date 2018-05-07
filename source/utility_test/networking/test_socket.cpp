#include <gtest/gtest.h>

#include <utility/networking/client/implementation/socket.h>
#include <utility/networking/server/implementation/socket.h>
#include <utility/networking/url.h>
#include <utility/networking/dns.h>
#include <utility/protocol/http/implementation/protocol.h>
#include <utility/protocol/http/implementation/uri_fs_mapper.h>
#include <utility/logging/logger.h>
#include <utility/convertion/convert.h>
#include <utility/networking/implementation/platform/windows/wsa.h>
#include <utility/threading/thread.h>


using namespace std;

using utility::networking::getIPv4ByHost;
using utility::convertion::convert;
using utility::threading::sleep;


namespace utility {
namespace networking {
namespace socket {


TEST(networking, socket_loopback) {
    logging::update(logging::config::createDefaultConsoleLoggerConfig());

    protocol::http::IHTTPHandler::TSharedPtr    http_handler = 
        protocol::http::implementation::CURIFSMapper::create(".");
    protocol::IProtocol::TSharedPtr             protocol = 
        protocol::http::implementation::CProtocol::create(http_handler);
    server::ISocket::TSharedPtr                 socket = 
        server::implementation::CSocket::create(URL("http://127.0.0.1:55555"), protocol, 2);

    return;
    try {
        socket->initialize();
        LOGT << "begin";
        //threading::sleep(1000);
        LOGT << "end";
        socket->finalize();
    } catch (std::exception const &e) {
        FAIL() << e.what();
    }
}


TEST(networking, dns) {
    auto ipv4 = getIPv4ByHost("ya.ru");

    //cout << "ya.ru ip: "
    //    << static_cast<int>(ipv4[0]) << "."
    //    << static_cast<int>(ipv4[1]) << "."
    //    << static_cast<int>(ipv4[2]) << "."
    //    << static_cast<int>(ipv4[3]) << endl;

    ASSERT_EQ(4, ipv4.size());
    ASSERT_THROW(getIPv4ByHost("ya.rur"), std::exception);
}


TEST(networking, url) {
    ASSERT_THROW(URL(""), std::exception);
    ASSERT_THROW(URL("172.16.0.64"), std::exception);
    ASSERT_THROW(URL("55555"), std::exception);
    ASSERT_THROW(URL(":55555"), std::exception);
    ASSERT_THROW(URL("::"), std::exception);

    URL url("https://172.16.0.64:55555");

    ASSERT_TRUE(static_cast<bool>(url.getHost()));
    ASSERT_TRUE(static_cast<bool>(url.getIPv4()));
    ASSERT_TRUE(static_cast<bool>(url.getPort()));
    ASSERT_TRUE(static_cast<bool>(url.getProtocol()));

    ASSERT_EQ(URL::TProtocol::HTTPS, url.getProtocol());
    ASSERT_EQ(std::vector<uint8_t>({ 172, 16, 0, 64 }), *url.getIPv4());
    ASSERT_EQ("172.16.0.64", url.getIPv4AsString());
    ASSERT_EQ(55555, *url.getPort());

    url = convert<URL>(string("http://hostname.ru"));
    ASSERT_TRUE(static_cast<bool>(url.getHost()));
    ASSERT_EQ("hostname.ru", *url.getHost());

    // test dns resolver
    ASSERT_TRUE(static_cast<bool>(URL("http://ya.ru").getIPv4()));
    ASSERT_LE(static_cast<size_t>(7), URL("http://ya.ru").getIPv4AsString().size());
    ASSERT_THROW(URL("http://ya.rur").getIPv4AsString(), std::exception);
}


//class A {
//public:
//    A() {
//        cout << "create" << endl;
//        i = 5;
//    }
//    ~A() {
//        cout << "destroy" << endl;
//        i = 0;
//    }
//    A(A const &a) {
//        cout << "copy" << endl;
//    }
//    A(A const &&a) {
//        cout << "move" << endl;
//    }
//    A(A &a) {
//        cout << "copy 2" << endl;
//    }
//    A(A &&a) {
//        cout << "move 2" << endl;
//    }
//    void f() const {
//        cout << "func " << i << endl;
//        i++;
//    }
//    mutable int i = 5;
//};
//
//
//template<typename T>
//T &f(T &t) {
//    return t;
//}


#ifdef WINDOWS_PLATFORM
TEST(networking, wsa) {
    return;
    //{
    //    A const a;
    //    cout << "begin" << endl;
    //    {
    //        f(a).f();
    //        //aa.f();
    //    }
    //    cout << "end" << endl;
    //}
    //
    //return;

    logging::update(logging::config::createDefaultConsoleLoggerConfig());

    using implementation::platform::WSA;

    try {
        auto listen_socket = WSA::instance().listen(URL("127.0.0.1:55555"));

        LOGT << "listen_socket " << listen_socket;
    } catch (std::exception const &e) {
        FAIL() << e.what();
    }

    //auto accept_socket = WSA::instance().accept(listen_socket);

    //LOGT << "accept_socket " << accept_socket;
    sleep(1000);
}
#endif


} // socket
} // networking
} // utility
