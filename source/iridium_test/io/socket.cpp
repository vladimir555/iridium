/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <iridium/testing/tester.h>

#include <iridium/io/url.h>
#include <iridium/io/net/dns.h>
#include <iridium/io/net/implementation/socket_server.h>
#include <iridium/io/net/implementation/socket_client.h>
#include <iridium/logging/logger.h>
#include <iridium/convertion/convert.h>
#include <iridium/io/net/implementation/platform/windows/wsa.h>
#include <iridium/threading/thread.h>

#include <iridium/io/fs/implementation/file_stream_reader.h>
#include <iridium/io/implementation/stream_buffer.h>
#include <iridium/io/protocol/http/implementation/protocol_factory.h>

#include <iridium/io/protocol/http/implementation/peer_session.h>
#include <iridium/io/protocol/http/implementation/client_session.h>
#include <iridium/io/protocol/http/implementation/acceptor_session.h>


#include "iridium/items.h"


using namespace std;
using iridium::io::net::getIPv4ByHost;
using iridium::convertion::convert;
using iridium::threading::sleep;


#include <iridium/io/fs/implementation/file_stream.h>
#include <iridium/io/implementation/listener.h>
#include <iridium/io/net/implementation/socket.h>
using iridium::io::fs::implementation::CFileStream;
//using iridium::io::implementation::CStreamProxy;
//using iridium::io::net::implementation::CSocketServer;
//using iridium::io::net::implementation::CSocketClient;
using iridium::io::implementation::CStreamReaderBuffer;
using iridium::io::implementation::CStreamWriterBuffer;
//using iridium::io::protocol::http::implementation::CSimpleProtocolFactory;
//using iridium::io::protocol::http::implementation::CAcceptorSession;
using iridium::io::protocol::http::implementation::CClientSession;
using iridium::io::protocol::http::implementation::CPeerSession;


#include <set>


#include "iridium/io/implementation/stream_pool.h"
#include "iridium/io/net/implementation/socket_client.h"
#include "iridium/io/net/implementation/socket_acceptor.h"
#include "iridium/io/net/implementation/socket_peer.h"
#include "iridium/io/net/implementation/client.h"
#include "iridium/io/net/implementation/server.h"
#include "iridium/io/net/socket.h"


namespace iridium {
namespace io {
namespace net {
namespace test {


class CHTTPPeerFactory: public IPeerSessionFactory {
public:
    DEFINE_IMPLEMENTATION(CHTTPPeerFactory)
    CHTTPPeerFactory() = default;
    protocol::ISession::TSharedPtr createSession(IStreamPort::TSharedPtr const &peer) override {
        return protocol::http::implementation::CPeerSession::create(peer);
    }
};


TEST(https_server) {
//    auto server = net::implementation::CServer::create(URL("http://127.0.0.1:55555"), CHTTPPeerFactory::create());

//    server->initialize();

//    LOGT << "begin";
//    threading::sleep(500000);
//    LOGT << "end";

//    server->finalize();
}


TEST(https_client) {
//    auto stream_pool = io::implementation::CStreamPool::create();

//    stream_pool->initialize();

//    auto session = protocol::http::implementation::CClientSession::create(URL("https://example.com"));

//    stream_pool->add(session);
//    threading::sleep(10000);
//    stream_pool->del(session);

//    stream_pool->finalize();


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


#ifdef WINDOWS_PLATFORM
TEST(wsa) {
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


} // test
} // net
} // io
} // iridium
