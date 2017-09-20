#include <gtest/gtest.h>

#include <utility/parsing/node.h>
#include <utility/parsing/implementation/node.h>
#include <utility/parsing/parser.h>
#include <utility/parsing/implementation/parser_xml.h>
#include <utility/parsing/implementation/parser_json.h>
#include <utility/parsing/implementation/parser_http_request.h>
#include <utility/assert.h>


using std::string;
using utility::parsing::INode;
using utility::parsing::implementation::CNode;
using utility::parsing::implementation::CXMLParser;
using utility::parsing::implementation::CJSONParser;
using utility::parsing::implementation::CHTTPRequestParser;


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
"        ]\n"
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
        auto item = root_node->addChild("item");
        item->addChild("", "text");
    }

    return root_node;
}


string xml_str_expects = ""
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<root>\n"
"\t<item value_1=\"value_1\" name_1=\"name_1\"/>\n"
"\t<item value_2=\"value_2\" name_2=\"name_2\">\n"
"\t\t<sub_item sub_item_attr_value_5=\"sub_item_attr_value_5\"/>\n"
"\t</item>\n"
"\t<item>text</item>\n"
"</root>\n\n";


string json_str_expects = ""
"{\n"
"    item: [\n"
"        {\n"
"            attr_name: \"name_1\",\n"
"            attr_value: \"value_1\"\n"
"        },\n"
"        {\n"
"            attr_name: \"name_2\",\n"
"            attr_value: \"value_2\",\n"
"            sub_item: {\n"
"                item: \"sub_item_attr_value_5\"\n"
"            }\n"
"        },\n"
"        {\n"
"            #text: \"text\"\n"
"        }\n"
"    ]\n"
"}\n";


string http_header = ""
"content-type: text/html; charset=windows-1251\n"
"Allow: GET, HEAD\n"
"Content-Length: 356\n"
"ALLOW: GET, OPTIONS\n"
"Content-Length:   1984";


} // unnamed


namespace utility {
namespace parsing {


TEST(parsing, parse_xml) {
    auto parser         = CXMLParser::create();
    auto node           = parser->parse(beer_xml);
    auto value_nodes    = assertComplete(node->findChilds("/Brewery/Beer/name"), "tags not found");

    ASSERT_EQ("Centennial",          (*value_nodes.begin())->getValue());
    ASSERT_EQ("Farmhouse Ale",     (*++value_nodes.begin())->getValue());
    ASSERT_EQ("Two Hearted Ale", (*++++value_nodes.begin())->getValue());
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
}


TEST(parsing, compose_json) {
    auto parser         = CJSONParser::create();
    auto node           = createTestNode();
    auto json_str       = parser->compose(node);

    ASSERT_EQ(json_str_expects, json_str);
}


TEST(parsing, parse_http_request) {
    auto parser         = CHTTPRequestParser::create();
    auto node           = parser->parse(http_header);

    std::cout << "http:\n" << convertion::convert<string>(node) << "\n";
}


} // parsing
} // utility
