#include "file_parser.h"

#include "parser.h"

#include "implementation/parser_json.h"
#include "implementation/parser_xml.h"

#include "iridium/strings.h"
#include "iridium/io/fs/files.h"

#include <stdexcept>


using std::string;

using iridium::io::fs::readFile;
using iridium::io::fs::writeFile;

using iridium::io::fs::extractFileNameExtension;

using iridium::parsing::implementation::CJSONParser;
using iridium::parsing::implementation::CXMLParser;


namespace iridium {
namespace parsing {


IParser::TSharedPtr createParserByExtension(string const &file_name) {
    auto extension = upperCase(extractFileNameExtension(file_name));

    if (extension == "JSON")
        return CJSONParser::create(); // ----->

    if (extension == "XML")
        return CXMLParser::create(); // ----->

    throw std::runtime_error("not have parser for extension '" + extension + "'"); // ----->
}


INode::TSharedPtr parseFile(string const &file_name) {
    auto parser = createParserByExtension(file_name);
    auto text   = readFile(file_name);

    return parser->parse(text); // ----->
}


void composeFile(string const &file_name, INode::TSharedPtr const &root_node) {
    auto parser = createParserByExtension(file_name);
    auto text   = parser->compose(root_node);

    writeFile(file_name, text);
}


} // parsing
} // iridium
