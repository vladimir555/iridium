/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <iridium/testing/tester.h>

#include <iridium/io/net/url.h>
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
#include <iridium/io/protocol/http/implementation/request_handler.h>
#include <iridium/io/protocol/http/implementation/response_handler.h>

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
using iridium::io::protocol::http::implementation::CSimpleProtocolFactory;


#include <set>


#include "iridium/io/implementation/stream_pool.h"
#include "iridium/io/net/implementation/socket_client.h"
#include "iridium/io/net/implementation/socket_acceptor.h"
#include "iridium/io/net/implementation/socket_peer.h"
#include "iridium/io/net/implementation/client.h"
#include "iridium/io/net/socket.h"


namespace iridium {
namespace io {
namespace net {
namespace test {


class CHTTPProtocolClientHandler: public protocol::IProtocolHandler {
public:
    DEFINE_IMPLEMENTATION(CHTTPProtocolClientHandler)

    CHTTPProtocolClientHandler() {
        m_state             = 0;
        m_response          = Buffer::create();
        m_response_stream   = CStreamWriterBuffer::create(m_response);
    }

    // redirect streams
    bool redirectStreams(ITransmitterStreams::TSharedPtr const &transmitter, Event::TSharedPtr const &event) override {
        LOGT << "state = " << m_state << " event = " << event->type;

        if(checkOneOf(event->type,
            Event::TType::OPEN,
            Event::TType::WRITE
        ) && m_state == 0)

//        if(checkOneOf<Event::TType>(
//            event->type, {
//                Event::TType::OPEN,
//                Event::TType::WRITE
//            }) &&
//            m_state == 0)
        {
            string request = ""
            "GET / HTTP/1.1\r\n"
            "Host: example.com:443\r\n"
            "User-Agent: curl/7.58.0\r\n"
            "Accept: */*"
            "\r\n"
            "\r\n";

            transmitter->set(
                CStreamReaderBuffer::create(
                    Buffer::create(request)),
                    std::dynamic_pointer_cast<IStreamWriter>(event->stream));

            m_state++;

            return true;
        }

        if (event->type == Event::TType::READ && m_state == 1) {
            transmitter->set(std::dynamic_pointer_cast<IStreamReader>(event->stream), m_response_stream);

            m_state++;

            return true;
        }

        if (m_state == 2) {
            LOGT << *m_response;
        }

        return true;
    }

private:
    int                         m_state;
    IStreamWriter::TSharedPtr   m_response_stream;
    Buffer::TSharedPtr          m_response;
};


TEST(https_server) {

//    stream_pool->add();

//    // todo: https
//    logging::update(logging::config::createDefaultConsoleLoggerConfig());
//    auto protocol_factory   = CSimpleProtocolFactory<protocol::http::implementation::CResponseProtocolHandler>::create();
//    auto socket             = CSocketServer::create(URL("http://localhost:55555"), protocol_factory, 1);
//    socket->initialize();
//    LOGT << "begin";
//    threading::sleep(500000);
//    LOGT << "end";
//    socket->finalize();
}


TEST(https_client) {
//    logging::update(iridium::logging::config::createDefaultConsoleLoggerConfig());

//    IClient::TSharedPtr client =
//            implementation::CClient::create(
////                URL("https://127.0.0.1:44330"),
//                URL("https://example.com"),
//                CHTTPProtocolClientHandler::create());

//    client->initialize();

//    LOGT << "begin";
//    threading::sleep(5000);
//    LOGT << "end";

//    client->finalize();
}


//TEST(https_client) {
//    string request = ""
//    "GET / HTTP/1.1\r\n"
//    "Host: example.com:80\r\n"
//    "User-Agent: curl/7.58.0\r\n"
//    "Accept: */*"
//    "\r\n"
//    "\r\n";

//    using io::implementation::CListener;
//    using io::net::implementation::CSocket;

//    auto listener = CListener::create();
////    auto socket   = CSocket::create(URL("https://localhost:1443"), false);
//    auto socket   = CSocket::create(URL("https://example.com"), false);

//    socket->initialize();
//    listener->initialize();

//    listener->add(socket);

//    LOGT << "! 1 write";
//    size_t write_result = 0;
//    write_result = socket->write(Buffer::create(request));

//    auto is_continue = true;
//    do {
//        LOGT << "! step";
//        auto events = listener->wait();

//        if (events.empty())
//            break;

//        for (auto const &event: events) {
//            LOGT << "! event: " << event->type;
//            if (event->stream == socket) {
//                if (event->type == io::Event::TType::READ) {
//                    LOGT << "! 2 read";
//                    auto result = socket->read(64);
//                    if (result) {
//                        auto result = socket->read(64);
////                        LOGT << *result;
////                        if (result->empty())
//                            continue;
//                    }
//                }
//                if (event->type == io::Event::TType::WRITE && write_result == 0) {
//                    LOGT << "! 2 write";
//                    write_result = socket->write(Buffer::create(request));
//                    if (write_result == 0)
//                        continue;
//                }
//            }
//        }
//    } while (is_continue);

//    socket->finalize();
//    listener->finalize();
//}


//TEST(dns) {
//    auto ipv4 = getIPv4ByHost("ya.ru");

//    //cout << "ya.ru ip: "
//    //    << static_cast<int>(ipv4[0]) << "."
//    //    << static_cast<int>(ipv4[1]) << "."
//    //    << static_cast<int>(ipv4[2]) << "."
//    //    << static_cast<int>(ipv4[3]) << endl;

//    ASSERT(4, equal, ipv4.size());
//    ASSERT(getIPv4ByHost("ya.rur"), std::exception);
//}


TEST(url) {
    using iridium::io::net::URL;
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
