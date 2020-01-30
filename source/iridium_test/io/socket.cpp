/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <iridium/testing/tester.h>

#include <iridium/io/net/url.h>
#include <iridium/io/net/dns.h>
#include <iridium/io/net/implementation/socket_server.h>
#include <iridium/logging/logger.h>
#include <iridium/convertion/convert.h>
#include <iridium/io/net/implementation/platform/windows/wsa.h>
#include <iridium/threading/thread.h>

#include <iridium/io/fs/implementation/file_stream_reader.h>

#include <memory>


using namespace std;

using iridium::io::net::getIPv4ByHost;
using iridium::convertion::convert;
using iridium::threading::sleep;


#include <iridium/io/fs/implementation/file_stream.h>
#include <iridium/io/protocol/http/implementation/protocol_factory.h>
using iridium::io::fs::implementation::CFileStream;
//using iridium::io::implementation::CStreamProxy;
using iridium::io::protocol::http::implementation::CProtocolFactory;
using iridium::io::net::implementation::CSocketServer;


#include <set>

namespace iridium {
namespace io {
namespace net {
namespace socket {


TEST(socket_loopback) {

    //std::set<int> s1{ 1, 2 };
    //std::set<int> s2{ 2, 1 };
    //std::set<IStream::TSharedPtr> s;
    //std::map< std::set<int>, int > m;

    logging::update(logging::config::createDefaultConsoleLoggerConfig());

//    IStreamReader::TSharedPtr reader = fs::implementation::CFileStreamReader::create("html/index.html");
//    reader->initialize();
//    LOGT << "read size = " << reader->read(8192).size();
//    reader->finalize();
//    return;

    //LOGT << "set: '" << (bool)(s1 == s2) << "'";
    //m[s1] = 1;
    //LOGT << m[s1];
    //m[s2] = 2;
    //LOGT << m[s1];
    //LOGT << m.size();
    //return;
    
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
    


    auto protocol_factory   = CProtocolFactory::create();
    auto socket             = CSocketServer::create(URL("http://127.0.0.1:55555"), protocol_factory, 10);

    socket->initialize();
    LOGT << "begin";
    threading::sleep(500000);
    LOGT << "end";
    socket->finalize();
}


TEST(dns) {
    auto ipv4 = getIPv4ByHost("ya.ru");

    //cout << "ya.ru ip: "
    //    << static_cast<int>(ipv4[0]) << "."
    //    << static_cast<int>(ipv4[1]) << "."
    //    << static_cast<int>(ipv4[2]) << "."
    //    << static_cast<int>(ipv4[3]) << endl;

    ASSERT(4, equal, ipv4.size());
    ASSERT(getIPv4ByHost("ya.rur"), std::exception);
}


TEST(url) {
    ASSERT(URL("")            , std::exception);
    ASSERT(URL("172.16.0.64") , std::exception);
    ASSERT(URL("55555")       , std::exception);
    ASSERT(URL(":55555")      , std::exception);
    ASSERT(URL("::")          , std::exception);

    URL url("https://172.16.0.64:55555");

    ASSERT(static_cast<bool>(url.getHost()));
    ASSERT(static_cast<bool>(url.getIPv4()));
    ASSERT(static_cast<bool>(url.getPort()));
    ASSERT(static_cast<bool>(url.getProtocol()));

    ASSERT(URL::TProtocol::HTTPS, equal, url.getProtocol());
    ASSERT(std::vector<uint8_t>({ 172, 16, 0, 64 }), equal, *url.getIPv4());
    ASSERT("172.16.0.64", equal, url.getIPv4AsString());
    ASSERT(55555, equal, *url.getPort());

    url = convert<URL>(string("http://hostname.ru"));
    ASSERT(static_cast<bool>(url.getHost()));
    ASSERT("hostname.ru", equal, *url.getHost());

    // test dns resolver
    ASSERT(static_cast<bool>(URL("http://ya.ru").getIPv4()));
    ASSERT(static_cast<size_t>(7), less, URL("http://ya.ru").getIPv4AsString().size());
    ASSERT(URL("http://ya.rur").getIPv4AsString(), std::exception);
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

    auto listen_socket = WSA::instance().listen(URL("127.0.0.1:55555"));

    LOGT << "listen_socket " << listen_socket;

    //auto accept_socket = WSA::instance().accept(listen_socket);

    //LOGT << "accept_socket " << accept_socket;
    sleep(10000);
}
#endif


} // socket
} // net
} // io
} // iridium
