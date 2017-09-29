#include <gtest/gtest.h>


#include <iostream>
using namespace std;


#include "utility/networking/http/request.h"
#include "utility/parsing/implementation/parser_http.h"
#include "utility/parsing/implementation/parser_json.h"
#include "utility/parsing/implementation/parser_xml.h"


using utility::parsing::implementation::CHTTPParser;
using utility::parsing::implementation::CJSONParser;
using utility::parsing::implementation::CXMLParser;
using utility::convertion::convert;


namespace utility {
namespace networking {
namespace http {


string const request_example = ""
"GET / HTTP/1.1\n"
"Host: www.google.com\n"
"User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.9.2) Gecko/20100115 Firefox/3.6 GTB7.1\n"
"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\n"
"Accept-Language: ru,en-us;q=0.7,en;q=0.3\n"
"Accept-Encoding: gzip,deflate\n"
"Accept-Charset: windows-1251,utf-8;q=0.7,*;q=0.7\n"
"Keep-Alive: 115\n"
"Connection: keep-alive\n"
"Cookie: PREF=ID=2578ccacb1ecf2aa:U=3a0a80ae418769c4:TM=1264579766:LM=1284111039:DV=sTIUytipGDoC:S=N0XC7wB0v7c6CCnH\n";


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


        THttp http(node);
//        cout << "http message method    : " << convert<string>(http.Message.get().method) << endl;
//        cout << "http message uri       : " << http.Message.get().uri << endl;
//        cout << "http message protocol  : " << http.Message.get().protocol << endl;

//        cout << "http header host       : " << http.Headers.Host.get() << endl;

//        for (auto const &accept: http.Headers.Accept)
//            cout << "accept: " << accept.get() << endl;

    } catch (std::exception const &e) {
        FAIL();
    }
}


} // socket
} // networking
} // utility
