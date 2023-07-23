#include <iridium/testing/tester.h>

#include <iridium/convertion/convert.h>
#include <iridium/io/net/dns.h>
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


} // test
} // net
} // io
} // iridium
