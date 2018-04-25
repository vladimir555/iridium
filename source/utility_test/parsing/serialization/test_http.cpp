#include <gtest/gtest.h>

#include <iostream>
using namespace std;

#include "utility/protocol/http/request.h"
#include "utility/protocol/http/response.h"
#include "utility/protocol/http/implementation/protocol.h"
#include "utility/parsing/implementation/parser_http.h"
#include "utility/parsing/implementation/parser_json.h"
#include "utility/parsing/implementation/parser_xml.h"

#include "utility/networking/server/implementation/socket.h"
#include "utility/networking/implementation/socket_handler.h"


using utility::networking::server::implementation::CSocket;
using utility::protocol::http::implementation::CProtocol;
using utility::parsing::implementation::CHTTPParser;
using utility::parsing::implementation::CJSONParser;
using utility::parsing::implementation::CXMLParser;
using utility::convertion::convert;
using utility::threading::sleep;
using std::string;


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
"content-type: text/html\n\n\n"
"<html>\n"
"<body>\n"
"<h1>Hello, World!::</h1>\n"
"</body>\n"
"</html>\n";


TEST(serialization, http_request) {
    auto parser = CHTTPParser::create();
    auto node   = parser->parse(request_example);

    try {
//        LOGT << "node: " << convert<string>(node);

//        {
//            auto json = CJSONParser::create();
//            auto xml  = CXMLParser::create();
//            LOGT << "node: " << endl << convert<string>(node);
//            LOGT << "http: " << endl << parser->compose(node);
//            LOGT << "json: " << endl << json->compose(node);
//            LOGT << "xml : " << endl <<  xml->compose(node);
//        }


        protocol::http::request::THttp http(node);
//        ASSERT_EQ(request_example, parser->compose(http.getNode()));

//        LOGT << "http message method    : " << convert<string>(http.Message.get().method);
//        LOGT << "http message uri       : " << http.Message.get().uri;
//        LOGT << "http message protocol  : " << http.Message.get().protocol;

//        LOGT << "http header host       : " << http.Headers.Host.get();

//        for (auto const &accept: http.Headers.Accept)
//            LOGT << "accept: " << accept.get();

    } catch (std::exception const &e) {
        FAIL() << e.what();
    }
}


TEST(serialization, http_response) {
    auto parser = CHTTPParser::create();
    auto node   = parser->parse(responce_example);

//    LOGT << "node:" << endl << convert<string>(node);

    protocol::http::response::THttp http(node);

    auto node_  = http.getNode();
    auto str    = parser->compose(node_);

//    LOGT << "response:" << endl << str;

    ASSERT_EQ(responce_example, str);
}


} // socket
} // networking
} // utility
