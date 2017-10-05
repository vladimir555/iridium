#include <gtest/gtest.h>


#include <iostream>
using namespace std;


#include "utility/networking/http/request.h"
#include "utility/networking/http/response.h"
#include "utility/parsing/implementation/parser_http.h"
#include "utility/parsing/implementation/parser_json.h"
#include "utility/parsing/implementation/parser_xml.h"
#include "utility/networking/server/implementation/http.h"
#include "utility/networking/server/implementation/http_fs_mapper.h"
#include "utility/logging/logger.h"


using utility::parsing::implementation::CHTTPParser;
using utility::parsing::implementation::CJSONParser;
using utility::parsing::implementation::CXMLParser;
using utility::convertion::convert;


namespace utility {
namespace networking {
namespace http {


string const request_example = ""
"GET / HTTP/1.1\n"
"host: www.google.com\n"
"user-agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.9.2) Gecko/20100115 Firefox/3.6 GTB7.1\n"
"accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
"accept-Language: ru,en-us;q=0.7,en;q=0.3\n"
"accept-Encoding: gzip,deflate\n"
"accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7\n"
"keep-alive: 115\n"
"connection: keep-alive\n"
"cookie: PREF=ID=2578ccacb1ecf2aa:U=3a0a80ae418769c4:TM=1264579766:LM=1284111039:DV=sTIUytipGDoC:S=N0XC7wB0v7c6CCnH\n";


string const responce_example = ""
"HTTP/1.1 200 OK\n"
"date: Mon, 27 Jul 2009 12:28:53 GMT\n"
"server: Apache/2.2.14 (Win32)\n"
"last-modified: Wed, 22 Jul 2009 19:15:56 GMT\n"
"content-length: 88\n"
"content-type: text/html\n"
"connection: Closed\n\n\n"
"<html>\n"
"<body>\n"
"<h1>Hello, World!::</h1>\n"
"</body>\n"
"</html>\n";


TEST(networking, http_request) {
    auto parser = CHTTPParser::create();
    auto node   = parser->parse(request_example);

    try {
//        cout << "node: " << convert<string>(node) << endl;

//        {
//            auto json = CJSONParser::create();
//            auto xml  = CXMLParser::create();
//            cout << "node: " << endl << convert<string>(node) << endl;
//            cout << "http: " << endl << parser->compose(node) << endl;
//            cout << "json: " << endl << json->compose(node) << endl;
//            cout << "xml : " << endl <<  xml->compose(node) << endl;
//        }


        request::THttp http(node);
//        ASSERT_EQ(request_example, parser->compose(http.getNode()));

//        cout << "http message method    : " << convert<string>(http.Message.get().method) << endl;
//        cout << "http message uri       : " << http.Message.get().uri << endl;
//        cout << "http message protocol  : " << http.Message.get().protocol << endl;

//        cout << "http header host       : " << http.Headers.Host.get() << endl;

//        for (auto const &accept: http.Headers.Accept)
//            cout << "accept: " << accept.get() << endl;

    } catch (std::exception const &e) {
        cout << e.what() << endl;
        FAIL();
    }
}


TEST(networking, http_response) {
    auto parser = CHTTPParser::create();
    auto node   = parser->parse(responce_example);

//    cout << "node:" << endl << convert<string>(node) << endl;

    response::THttp http(node);

    auto node_  = http.getNode();
    auto str    = parser->compose(node_);

//    cout << "response:" << endl << str << endl;

    ASSERT_EQ(responce_example, str);
}


//class HTTPHandler: public server::IHTTPHandler {
//public:
//    DEFINE_CREATE(HTTPHandler)
//    virtual ~HTTPHandler() = default;

//    ISocket::TPacket handle(TRequest const &request) override {
//        i++;
//        return  convert<ISocket::TPacket>
//            ("<html><body>Hello! uri = " + request.uri +
//            " " + convert<string>(i) + " " + "</body></html>");
//    }
//private:
//    int i = 0;
//};


TEST(networking, http_server) {
    logging::update(true);
    LOGT << "start";

    server::IHTTP::THTTPHandlers    handlers;
//    handlers.push_back(HTTPHandler::create());
//    handlers.push_back(HTTPHandler::create());
    handlers.push_back(server::implementation::CHTTPFSMapper::create("html"));
//    handlers.push_back(server::implementation::CHTTPFSMapper::create("html"));
    server::IHTTP::TSharedPtr       http_server = server::implementation::CHTTP::create(URL("http://127.0.0.1:55555"), handlers);

    http_server->initialize();

    sleep(50);

    http_server->finalize();
}


} // socket
} // networking
} // utility
