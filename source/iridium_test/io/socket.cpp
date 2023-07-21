#include <iridium/testing/tester.h>

#include <iridium/convertion/convert.h>
#include <iridium/io/net/dns.h>
#include <iridium/io/uri.h>
#include <iridium/io/protocol/http/implementation/client_session.h>


using namespace std;
using iridium::io::net::getIPv4ByHost;
//using iridium::io::net::implementation::CServer;
using iridium::convertion::convert;


namespace iridium {
namespace io {
namespace net {
namespace test {


//TEST(https_server) {
//    IServer::TSharedPtr server = CServer::create(URI("http://127.0.0.1:55555"), CHTTPPeerSessionFactory::create());
//    server->initialize();
//    threading::sleep(500000);
//    server->finalize();
//}


//TEST(https_client) {
//    IClient::TSharedPtr client = CClient::create(CClientSession::create(URI("https://example.com")));
//    client->initialize();
//    threading::sleep(10000);
//    client->finalize();
//}


TEST(dns) {
    auto ipv4 = getIPv4ByHost("ya.ru");

    ASSERT(4, equal, ipv4.size());
    ASSERT(getIPv4ByHost("ya.rur"), std::exception);
}


TEST(uri) {
    using iridium::io::URI;
    ASSERT("localhost", equal, URI("localhost").getPath());

    ASSERT(URI("")                  , std::exception);
//    ASSERT(URI("172.16.0.64") , std::exception); // default type is file
//    ASSERT(URI("55555")       , std::exception);
    ASSERT(URI("htp://:55555")      , std::exception);
    ASSERT(URI("htp://::")          , std::exception);

    URI uri("https://172.16.0.64:55555");

    ASSERT(!uri.getHost().empty());
    ASSERT( static_cast<bool>(uri.getIPv4()));
    ASSERT( uri.getPort());

    ASSERT(URI::TProtocol::HTTPS, equal, uri.getProtocol());
//    ASSERT({ 172, 16, 0, 64 }, equal, uri.getIPv4());
//    LOGT << uri.getIPv4();
    ASSERT("172.16.0.64", equal, convert<string>(*uri.getIPv4()));
    ASSERT(55555, equal, uri.getPort());

    uri = convert<URI>(string("http://hostname.ru"));
    ASSERT(!uri.getHost().empty());
    ASSERT("hostname.ru", equal, uri.getHost());

    auto path = URI("https://www.bitmex.com/api/v1").getPath();
    ASSERT(!path.empty());
    ASSERT("/api/v1", equal, path);

    // test dns resolver
//    auto ya_ru_ipv4             = URI("http://ya.ru").getIPv4();
//    auto ya_ru_ipv4_expected    = URI("http://77.88.55.242").getIPv4();

    auto ya_ru_ipv4             = URI("http://example.com").getIPv4();
    auto ya_ru_ipv4_expected    = URI("http://93.184.216.34").getIPv4();

    ASSERT(static_cast<bool>(ya_ru_ipv4));
    ASSERT(static_cast<bool>(ya_ru_ipv4_expected));
    ASSERT(*ya_ru_ipv4_expected, equal, *ya_ru_ipv4);
    ASSERT( URI("http://ya.rur").getIPv4(), std::exception);
}


} // test
} // net
} // io
} // iridium
