#include <gtest/gtest.h>

#include <iridium/io/net/url.h>
#include <iridium/io/net/dns.h>
#include <iridium/io/net/implementation/socket_server.h>
#include <iridium/logging/logger.h>
#include <iridium/convertion/convert.h>
#include <iridium/io/net/implementation/platform/windows/wsa.h>
#include <iridium/threading/thread.h>


#include <memory>


using namespace std;

using iridium::io::net::getIPv4ByHost;
using iridium::convertion::convert;
using iridium::threading::sleep;


#include "iridium/io/fs/implementation/file_stream.h"
#include <iridium/io/protocol/http/implementation/protocol_factory.h>
using iridium::io::fs::implementation::CFileStream;
//using iridium::io::implementation::CStreamProxy;
using iridium::io::protocol::http::implementation::CProtocolFactory;
using iridium::io::net::implementation::CSocketServer;


namespace iridium {
namespace io {
namespace net {
namespace socket {


TEST(net, socket_loopback) {
    logging::update(logging::config::createDefaultConsoleLoggerConfig());
    
//    {
//        auto reader = CFileStream::create("index.html", CFileStream::TOpenMode::READ);
//        auto writer = CFileStream::create("index.html.output", CFileStream::TOpenMode::WRITE);
//
//        reader->initialize();
//        writer->initialize();
//
//        IStreamProxy::TSharedPtr  proxy  = CStreamProxy::create(reader, writer);
//
//        while (proxy->transmit());
//
//        reader->finalize();
//        writer->finalize();
//
//        return;
//    }
    


//    auto protocol_factory   = CProtocolFactory::create();
//    auto socket             = CSocketServer::create(URL("http://127.0.0.1:55555"), 1, protocol_factory);

//    try {
//        socket->initialize();
//        LOGT << "begin";
//        threading::sleep(100000);
//        LOGT << "end";
//        socket->finalize();
//    } catch (std::exception const &e) {
//        FAIL() << e.what();
//    }
}


TEST(net, dns) {
    auto ipv4 = getIPv4ByHost("ya.ru");

    //cout << "ya.ru ip: "
    //    << static_cast<int>(ipv4[0]) << "."
    //    << static_cast<int>(ipv4[1]) << "."
    //    << static_cast<int>(ipv4[2]) << "."
    //    << static_cast<int>(ipv4[3]) << endl;

    ASSERT_EQ(4, ipv4.size());
    ASSERT_THROW(getIPv4ByHost("ya.rur"), std::exception);
}


TEST(net, url) {
    ASSERT_THROW(URL("")            , std::exception);
    ASSERT_THROW(URL("172.16.0.64") , std::exception);
    ASSERT_THROW(URL("55555")       , std::exception);
    ASSERT_THROW(URL(":55555")      , std::exception);
    ASSERT_THROW(URL("::")          , std::exception);

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

//    auto makeFunc() {
//        return [this](){f();};
//    }
//};


//template<typename T>
//T &f(T &t) {
//    return t;
//}

//A a;


//TEST(test, test) {
//    A a;
//    auto func = a.makeFunc();
//    func();
//}


#ifdef WINDOWS_PLATFORM
TEST(net, wsa) {
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
    sleep(10000);
}
#endif


} // socket
} // net
} // io
} // iridium
