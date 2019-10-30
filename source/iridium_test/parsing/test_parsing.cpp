/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <gtest/gtest.h>

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
"<?xml version=\"1.0\" encoding=\"utf-8\"?>"
"<MyBeerJournal>"
"    <Brewery name=\"Founders Brewing Company\" location=\"Grand Rapids, MI\">"
"        <Beer name=\"Centennial\" description=\"IPA\" rating=\"A+\" dateSampled=\"01/02/2011\">"
"            \"What an excellent IPA. This is the most delicious beer I have ever tasted!\""
"        </Beer>"
"    </Brewery>"
"    <Brewery name=\"Brewery Vivant\" location=\"Grand Rapids, MI\">"
"        <Beer name=\"Farmhouse Ale\" description=\"Belgian Ale\" rating=\"B\" dateSampled=\"02/07/2015\">"
"            This beer is not so good... but I am not that big of a fan of english style ales."
"        </Beer>"
"    </Brewery>"
"    <Brewery name=\"Bells Brewery\" location=\"Kalamazoo, MI\">"
"        <Beer name=\"Two Hearted Ale\" description=\"IPA\" rating=\"A\" dateSampled=\"03/15/2012\">"
"            Another execllent brew. Two Hearted gives Founders Centennial a run for it's money."
"        </Beer>"
"    </Brewery>"
"    <array>5</array>"
"    <array>4</array>"
"    <array>3</array>"
"    <array>2</array>"
"    <array>1</array>"
"</MyBeerJournal>";


string const beer_json = ""
"{\n"
"    \"MyBeerJournal\": {"
"        \"Brewery\" : [\n"
"            {\n"
"                \"Beer\" : {\n"
"                    \"#text\" : \"            \\\"What an excellent IPA. This is the most delicious beer I have ever tasted!\\\"        \",\n"
"                    \"dateSampled\" : \"01/02/2011\",\n"
"                    \"description\" : \"IPA\",\n"
"                    \"name\" : \"Centennial\",\n"
"                    \"rating\" : \"A+\"\n"
"                },\n"
"                \"location\" : \"Grand Rapids, MI\",\n"
"                \"name\" : \"Founders Brewing Company\"\n"
"            },\n"
"            {\n"
"                \"Beer\" : {\n"
"                    \"#text\" : \"            This beer is not so good... but I am not that big of a fan of english style ales.        \",\n"
"                    \"dateSampled\" : \"02/07/2015\",\n"
"                    \"description\" : \"Belgian Ale\",\n"
"                    \"name\" : \"Farmhouse Ale\",\n"
"                    \"rating\" : \"B\"\n"
"                },\n"
"                \"location\" : \"Grand Rapids, MI\",\n"
"                \"name\" : \"Brewery Vivant\"\n"
"            },\n"
"            {\n"
"                \"Beer\" : {\n"
"                    \"#text\" : \"            Another execllent brew. Two Hearted gives Founders Centennial a run for it's money.        \",\n"
"                    \"dateSampled\" : \"03/15/2012\",\n"
"                    \"description\" : \"IPA\",\n"
"                    \"name\" : \"Two Hearted Ale\",\n"
"                    \"rating\" : \"A\"\n"
"                },\n"
"                \"location\" : \"Kalamazoo, MI\",\n"
"                \"name\" : \"Bells Brewery\"\n"
"            }\n"
"        ],\n"
"        array: [5, 4, 3, 2, 1]\n"
"    }\n"
"}\n"
"";


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
"\t<item attr_value=\"value_1\" attr_name=\"name_1\"/>\n"
"\t<item attr_value=\"value_2\" attr_name=\"name_2\">\n"
"\t\t<sub_item item=\"sub_item_attr_value_5\"/>\n"
"\t</item>\n"
"\t<array>5</array>\n"
"\t<array>4</array>\n"
"\t<array>3</array>\n"
"\t<array>2</array>\n"
"\t<array>1</array>\n"
"\t<item>text</item>\n"
"</root>\n\n";


string const json_str_expects = ""
"{\n"
"    root: {\n"
"        item: [\n"
"            {\n"
"                attr_value: \"value_1\",\n"
"                attr_name: \"name_1\"\n"
"            },\n"
"            {\n"
"                attr_value: \"value_2\",\n"
"                attr_name: \"name_2\",\n"
"                sub_item: {\n"
"                    item: \"sub_item_attr_value_5\"\n"
"                }\n"
"            },\n"
"            {\n"
"                #text: \"text\"\n"
"            }\n"
"        ],\n"
"        array: [\n"
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
"allow: HEAD, OPTIONS, GET\n"
"content-length: 1984\n\n";


} // unnamed


namespace iridium {
namespace parsing {


TEST(parsing, parse_xml) {
    auto parser         = CXMLParser::create();
    auto node           = parser->parse(beer_xml);
    auto value_nodes    = assertComplete(node->findChilds("/Brewery/Beer/name"), "tags not found");

    ASSERT_EQ("Centennial",          (*value_nodes.begin())->getValue());
    ASSERT_EQ("Farmhouse Ale",     (*++value_nodes.begin())->getValue());
    ASSERT_EQ("Two Hearted Ale", (*++++value_nodes.begin())->getValue());

    string array;
    for (auto const &i: *node)
        if (i->getName() == "array")
            array += i->getValue();

    ASSERT_EQ("54321", array);
}


TEST(parsing, compose_xml) {
    auto parser         = CXMLParser::create();
    auto node           = createTestNode();
    auto xml_str        = parser->compose(node);

    ASSERT_EQ(xml_str_expects, xml_str);
}


TEST(parsing, parse_json) {
    auto parser         = CJSONParser::create();
    auto node           = parser->parse(beer_json);
    auto value_nodes    = assertComplete(node->findChilds("/Brewery/Beer/name"), "tags not found");

    ASSERT_EQ("Centennial",          (*value_nodes.begin())->getValue());
    ASSERT_EQ("Farmhouse Ale",     (*++value_nodes.begin())->getValue());
    ASSERT_EQ("Two Hearted Ale", (*++++value_nodes.begin())->getValue());

    string array;
    for (auto const &i: *node)
        if (i->getName() == "array")
            array += i->getValue();

    ASSERT_EQ("54321", array);
}


TEST(parsing, compose_json) {
    auto parser         = CJSONParser::create();
    auto node           = createTestNode();
    auto json_str       = parser->compose(node);

    ASSERT_EQ(json_str_expects, json_str);
}

// todo: differ for windows and linux
//TEST(parsing, parse_http_request) {
//    auto parser         = CHTTPParser::create();
//    auto node           = parser->parse(http_header);
//
//    ASSERT_EQ(http_header_node, convertion::convert<string>(node));
//}


TEST(parsing, compose_http_request) {
    auto parser         = CHTTPParser::create();
    auto node           = parser->parse(http_header);
    auto http_header_   = parser->compose(node);

    ASSERT_EQ(http_header_composed, http_header_);
}


} // parsing
} // iridium
