/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <iridium/testing/tester.h>

#include <iridium/convertion/convert.h>
#include <iridium/io/net/dns.h>
#include "iridium/io/net/implementation/server.h"
#include <iridium/io/protocol/http/implementation/peer_session.h>
#include <iridium/io/protocol/http/implementation/peer_session_factory.h>
#include <iridium/io/protocol/http/implementation/client_session.h>
#include "iridium/io/implementation/client.h"


using namespace std;
using iridium::io::net::getIPv4ByHost;
using iridium::io::protocol::http::implementation::CClientSession;
using iridium::io::protocol::http::implementation::CPeerSession;
using iridium::io::protocol::http::implementation::CHTTPPeerSessionFactory;
using iridium::io::implementation::CClient;
using iridium::io::net::implementation::CServer;
using iridium::convertion::convert;


namespace iridium {
namespace io {
namespace net {
namespace test {


TEST(https_server) {
    IServer::TSharedPtr server = CServer::create(URL("http://127.0.0.1:55555"), CHTTPPeerSessionFactory::create());
    server->initialize();
    threading::sleep(500000);
    server->finalize();
}


TEST(https_client) {
    IClient::TSharedPtr client = CClient::create(CClientSession::create(URL("https://example.com")));
    client->initialize();
    threading::sleep(10000);
    client->finalize();
}


TEST(dns) {
    auto ipv4 = getIPv4ByHost("ya.ru");

    ASSERT(4, equal, ipv4.size());
    ASSERT(getIPv4ByHost("ya.rur"), std::exception);
}


TEST(url) {
    using iridium::io::URL;
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

    auto path = URL("https://www.bitmex.com/api/v1").getPath();
    ASSERT(static_cast<bool>(path));
    ASSERT("/api/v1", equal, *path);
}


//#ifdef WINDOWS_PLATFORM
//TEST(wsa) {
//    logging::update(logging::config::createDefaultConsoleLoggerConfig());
//    using implementation::platform::WSA;
//    auto listen_socket = WSA::instance().listen(URL("127.0.0.1:55555"));
//    LOGT << "listen_socket " << listen_socket;
//    //auto accept_socket = WSA::instance().accept(listen_socket);
//    //LOGT << "accept_socket " << accept_socket;
//    sleep(10000);
//}
//#endif


} // test
} // net
} // io
} // iridium
