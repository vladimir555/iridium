#include <gtest/gtest.h>
#include <iostream>

#include <utility/pasring/parser.h>
#include <utility/pasring/implementation/xml_parser.h>
#include <utility/pasring/implementation/json_parser.h>
#include <utility/assert.h>


using std::string;
using utility::parsing::implementation::CXMLParser;
using utility::parsing::implementation::CJSONParser;


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
}


namespace utility {
namespace parsing {


TEST(parsing, parse_xml) {
    IParser::TSharedPtr parser = CXMLParser::create();

    parser->parse(beer_xml);

    auto value_nodes = assertComplete(parser->getRootNode()->findChilds("/Brewery/Beer/name"), "tags not found");
    ASSERT_EQ("Centennial", (*value_nodes.begin())->getValue());
}


TEST(parsing, compose_xml) {

}


TEST(parsing, parse_json) {

}


TEST(parsing, compose_json) {

}


} // parsing
} // utility
