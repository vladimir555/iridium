#include <iridium/testing/tester.h>

#include <iridium/parsing/node.h>
#include <iridium/parsing/implementation/node.h>
#include <iridium/parsing/parser.h>
#include <iridium/parsing/implementation/parser_xml.h>
#include <iridium/parsing/implementation/parser_json.h>
#include <iridium/parsing/implementation/parser_http.h>
#include <iridium/assert.h>


#include <iostream>
using namespace std;


using std::string;
using iridium::parsing::INode;
using iridium::parsing::implementation::CNode;
using iridium::parsing::implementation::CXMLParser;
using iridium::parsing::implementation::CJSONParser;
using iridium::parsing::implementation::CHTTPParser;


namespace {


string const beer_xml = ""
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<MyBeerJournal>\n"
"    <Brewery location=\"Grand Rapids, MI\" name=\"Founders Brewing Company\">\n"
"        <Beer dateSampled=\"01/02/2011\" description=\"IPA\" name=\"Centennial\" rating=\"A+\">\n"
"            \"What an excellent IPA. This is the most delicious beer I have ever tasted!\"\n"
"        </Beer>\n"
"    </Brewery>\n"
"    <Brewery location=\"Grand Rapids, MI\" name=\"Brewery Vivant\">\n"
"        <Beer dateSampled=\"02/07/2015\" description=\"Belgian Ale\" name=\"Farmhouse Ale\" rating=\"B\">\n"
"            This beer is not so good... but I am not that big of a fan of english style ales.\n"
"        </Beer>\n"
"    </Brewery>\n"
"    <Brewery location=\"Kalamazoo, MI\" name=\"Bells Brewery\">\n"
"        <Beer dateSampled=\"03/15/2012\" description=\"IPA\" name=\"Two Hearted Ale\" rating=\"A\">\n"
"            Another execllent brew. Two Hearted gives Founders Centennial a run for it's money.\n"
"        </Beer>\n"
"    </Brewery>\n"
"    <array>5</array>\n"
"    <array>4</array>\n"
"    <array>3</array>\n"
"    <array>2</array>\n"
"    <array>1</array>\n"
"</MyBeerJournal>\n\n";


string const beer_json = ""
"{\n"
"    \"MyBeerJournal\": {\n"
"        \"Brewery\": [\n"
"            {\n"
"                \"Beer\": {\n"
"                    \"#text\": \"\n            \\\"What an excellent IPA. This is the most delicious beer I have ever tasted!\\\"\n        \",\n"
"                    \"dateSampled\": \"01/02/2011\",\n"
"                    \"description\": \"IPA\",\n"
"                    \"name\": \"Centennial\",\n"
"                    \"rating\": \"A+\"\n"
"                },\n"
"                \"location\": \"Grand Rapids, MI\",\n"
"                \"name\": \"Founders Brewing Company\"\n"
"            },\n"
"            {\n"
"                \"Beer\": {\n"
"                    \"#text\": \"\n            This beer is not so good... but I am not that big of a fan of english style ales.\n        \",\n"
"                    \"dateSampled\": \"02/07/2015\",\n"
"                    \"description\": \"Belgian Ale\",\n"
"                    \"name\": \"Farmhouse Ale\",\n"
"                    \"rating\": \"B\"\n"
"                },\n"
"                \"location\": \"Grand Rapids, MI\",\n"
"                \"name\": \"Brewery Vivant\"\n"
"            },\n"
"            {\n"
"                \"Beer\": {\n"
"                    \"#text\": \"\n            Another execllent brew. Two Hearted gives Founders Centennial a run for it's money.\n        \",\n"
"                    \"dateSampled\": \"03/15/2012\",\n"
"                    \"description\": \"IPA\",\n"
"                    \"name\": \"Two Hearted Ale\",\n"
"                    \"rating\": \"A\"\n"
"                },\n"
"                \"location\": \"Kalamazoo, MI\",\n"
"                \"name\": \"Bells Brewery\"\n"
"            }\n"
"        ],\n"
"        \"array\": [\n"
"            \"5\",\n"
"            \"4\",\n"
"            \"3\",\n"
"            \"2\",\n"
"            \"1\"\n"
"        ]\n"
"    }\n"
"}\n";


INode::TSharedPtr createTestNode() {
    auto root_node = CNode::create("root");

    {
        auto item = root_node->addChild("item");
        item->addChild("attr_value", "value_1");
        item->addChild("attr_name", "name_1");
    }

    {
        auto item = root_node->addChild("item");
        item->addChild("attr_value", "value_2");
        item->addChild("attr_name", "name_2");
        auto sub_item = item->addChild("sub_item");
        sub_item->addChild("item", "sub_item_attr_value_5");
    }

    {
        root_node->addChild("array", "5");
        root_node->addChild("array", "4");
        root_node->addChild("array", "3");
        root_node->addChild("array", "2");
        root_node->addChild("array", "1");
    }

    {
        auto item = root_node->addChild("item");
        item->addChild("", "text");
    }

    return root_node;
}


string const xml_str_expects = ""
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<root>\n"
"    <item attr_value=\"value_1\" attr_name=\"name_1\"/>\n"
"    <item attr_value=\"value_2\" attr_name=\"name_2\">\n"
"        <sub_item item=\"sub_item_attr_value_5\"/>\n"
"    </item>\n"
"    <array>5</array>\n"
"    <array>4</array>\n"
"    <array>3</array>\n"
"    <array>2</array>\n"
"    <array>1</array>\n"
"    <item>text</item>\n"
"</root>\n\n";


string const json_str_expects = ""
"{\n"
"    \"root\": {\n"
"        \"item\": [\n"
"            {\n"
"                \"attr_value\": \"value_1\",\n"
"                \"attr_name\": \"name_1\"\n"
"            },\n"
"            {\n"
"                \"attr_value\": \"value_2\",\n"
"                \"attr_name\": \"name_2\",\n"
"                \"sub_item\": {\n"
"                    \"item\": \"sub_item_attr_value_5\"\n"
"                }\n"
"            },\n"
"            {\n"
"                \"#text\": \"text\"\n"
"            }\n"
"        ],\n"
"        \"array\": [\n"
"            \"5\",\n"
"            \"4\",\n"
"            \"3\",\n"
"            \"2\",\n"
"            \"1\"\n"
"        ]\n"
"    }\n"
"}\n";


string const http_header = ""
"GET / HTTP/1.1\n"
"content-type: text/html; charset=windows-1251\n"
"Allow: GET, HEAD\n"
"Content-Length: 356\n"
"ALLOW: GET, OPTIONS\n"
"Content-Length:   1984";


string const http_header_node = "\n"
"'http' = ''\n"
"  'message' = 'GET / HTTP/1.1'\n"
"  'headers' = ''\n"
"    'content-type' = 'text/html; charset=windows-1251'\n"
"    'allow' = 'GET'\n"
"    'allow' = 'HEAD'\n"
"    'allow' = 'OPTIONS'\n"
"    'content-length' = '1984'\n"
"  'body' = ''";


string const http_header_composed = ""
"GET / HTTP/1.1\n"
"content-type: text/html; charset=windows-1251\n"
"allow: GET, HEAD, OPTIONS\n"
"content-length: 1984\n\n";


} // unnamed


namespace iridium::parsing {


TEST(parse_xml) {
    auto parser         = CXMLParser::create();
    auto node           = parser->parse(beer_xml);

    ASSERT(node->hasChilds());
    ASSERT("MyBeerJournal", equal, node->getName());

    auto value_nodes    = assertComplete(node->slice("/Brewery/Beer/name"), "slice is empty");

    ASSERT( 3               , equal, value_nodes.size());
    ASSERT("Centennial"     , equal, std::next(value_nodes.begin(), 0)->get()->getValue());
    ASSERT("Farmhouse Ale"  , equal, std::next(value_nodes.begin(), 1)->get()->getValue());
    ASSERT("Two Hearted Ale", equal, std::next(value_nodes.begin(), 2)->get()->getValue());

    string array;
    for (auto const &i: *node)
        if (i->getName() == "array")
            array += i->getValue();

    ASSERT("54321", equal, array);
}


TEST(compose_xml) {
    auto parser         = CXMLParser::create();
    auto node           = createTestNode();
    auto xml_str        = parser->compose(node);

    ASSERT(xml_str_expects, equal, xml_str);
}


TEST(parse_json) {
    auto parser = CJSONParser::create();

    ASSERT(std::string("null\n"), equal, parser->parse("{  \"value\":\"null\n\"}  ")->getValue());

    auto node   = parser->parse(beer_json);

    ASSERT(8, equal, node->size());
    ASSERT("MyBeerJournal", equal, node->getName());

    auto value_nodes = assertComplete(node->slice("/Brewery/Beer/name"), "slice is empty");
    ASSERT( 3               , equal, value_nodes.size());
    ASSERT("Centennial"     , equal, std::next(value_nodes.begin(), 0)->get()->getValue());
    ASSERT("Farmhouse Ale"  , equal, std::next(value_nodes.begin(), 1)->get()->getValue());
    ASSERT("Two Hearted Ale", equal, std::next(value_nodes.begin(), 2)->get()->getValue());

    string array;
    for (auto const &i: *node)
        if (i->getName() == "array")
            array += i->getValue();

    ASSERT("54321"          , equal, array);
    ASSERT("true"           , equal, parser->parse("{ \"value\":true }")->getValue());
    ASSERT("false"          , equal, parser->parse("{ \"value\":false }")->getValue());
    ASSERT("null"           , equal, parser->parse("{ \"value\":null }")->getValue());
    ASSERT("0.123456789"    , equal, parser->parse("{ \"value\":0.123456789 }")->getValue());
    ASSERT("null0"          , equal, parser->parse("{ \"value\":\"null0\" }")->getValue());
    ASSERT(parser->parse("{ \"value\":null0 }"), std::exception);

    {
        auto slice = parser->parse("{ array: [ {\"value\": 1}, {\"value\": 2} ] }")->slice("/array/value");

        ASSERT( 2       , equal, slice.size());

        ASSERT("value"  , equal, slice.front()->getName());
        ASSERT("1"      , equal, slice.front()->getValue());

        ASSERT("value"  , equal, slice.back()->getName());
        ASSERT("2"      , equal, slice.back()->getValue());
    }
    {
        auto slice = parser->parse("{ value: [ \"1\", \"2\" ] }")->slice("/value");

        ASSERT( 2       , equal, slice.size());

        ASSERT("value"  , equal, slice.front()->getName());
        ASSERT("1"      , equal, slice.front()->getValue());

        ASSERT("value"  , equal, slice.back()->getName());
        ASSERT("2"      , equal, slice.back()->getValue());
    }
    {
        auto slice = parser->parse("[ {\"value\": 1}, {\"value\": 2} ]")->slice("/array/value");

        ASSERT( 2       , equal, slice.size());

        ASSERT("value"  , equal, slice.front()->getName());
        ASSERT("1"      , equal, slice.front()->getValue());

        ASSERT("value"  , equal, slice.back()->getName());
        ASSERT("2"      , equal, slice.back()->getValue());
    }
    {
        auto slice = parser->parse("[ \"1\", \"2\" ]")->slice("/array");

        ASSERT( 2       , equal, slice.size());

        ASSERT("array"  , equal, slice.front()->getName());
        ASSERT("1"      , equal, slice.front()->getValue());

        ASSERT("array"  , equal, slice.back()->getName());
        ASSERT("2"      , equal, slice.back()->getValue());
    }
    {
        auto slice = parser->parse("[ 1, 2 ]")->slice("/array");

        ASSERT( 2       , equal, slice.size());

        ASSERT("array"  , equal, slice.front()->getName());
        ASSERT("1"      , equal, slice.front()->getValue());

        ASSERT("array"  , equal, slice.back()->getName());
        ASSERT("2"      , equal, slice.back()->getValue());
    }

    ASSERT(parser->parse("[ {\"value\": 1}, {\"value\": 2 ]"), std::exception);
    ASSERT(parser->parse("[ {\"value\": 1}, {\"value\": 2 }"), std::exception);
    ASSERT(parser->parse("[ {\"value\": 1}, {\"value\": 2"),   std::exception);
    ASSERT(parser->parse("[ {\"value\": 1,  {\"value\": 2 }]"),std::exception);
//    ASSERT(parser->parse("{\"value\" 1 }"),                    std::exception);
//    LOGT << "BEGIN";
    ASSERT(parser->parse("{\"value\": 1,}"),                   std::exception);
//    LOGT << "END";
    node = parser->parse(
"{\n"
"      \"result\": {\n"
"          \"tests\": [\n"
"              {\n"
"                  \"path\": \"/parsing/serialization/http.cpp/http_request\",\n"
"                  \"error\": \"\",\n"
"                  \"output\": \"\"\n"
"              },\n"
"              {\n"
"                  \"path\": \"/parsing/serialization/http.cpp/http_response\",\n"
"                  \"error\": \"\",\n"
"                  \"output\": \"\"\n"
"              }\n"
"          ]\n"
"      }\n"
"}\n");

    node = parser->parse(
"{ \"value\":"
"2110\t"
"}");

}


TEST(compose_json) {
    auto parser         = CJSONParser::create();
    auto node           = createTestNode();
    auto json_str       = parser->compose(node);

    ASSERT(json_str_expects, equal, json_str);
}


// todo: differ in windows and linux
//TEST(parsing, parse_http_request) {
//    auto parser         = CHTTPParser::create();
//    auto node           = parser->parse(http_header);
//
//    ASSERT_EQ(http_header_node, convertion::convert<string>(node));
//}


TEST(compose_http_request) {
    auto parser         = CHTTPParser::create();
    auto node           = parser->parse(http_header);
    auto http_header_   = parser->compose(node);

    ASSERT(http_header_composed, equal, http_header_);
}


TEST(parse_compose) {
    auto parser_json    = CJSONParser::create();
    auto parser_xml     = CXMLParser::create();

    auto node           = parser_json->parse(beer_json);
    auto xml_str        = parser_xml->compose(node);

    ASSERT(beer_xml, equal, xml_str);

    auto json_str       =  parser_json->compose(node);

    ASSERT(beer_json, equal, json_str);
}


} // namespace iridium::parsing
