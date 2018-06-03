#include "file_parser.h"

#include "parser.h"

#include "implementation/parser_json.h"
#include "implementation/parser_xml.h"

#include "utility/strings.h"
#include "utility/fs/files.h"
#include "utility/fs/implementation/fast_text_writer.h"


using std::string;

using utility::fs::readTextFile;
using utility::fs::extractFileNameExtension;
using utility::fs::checkFileExistence;
using utility::fs::implementation::CFileWriter;

using utility::parsing::implementation::CJSONParser;
using utility::parsing::implementation::CXMLParser;


namespace utility {
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
    if (!checkFileExistence(file_name))
        throw std::runtime_error("file '" + file_name + "' does not exists"); // ----->

    auto    parser     = createParserByExtension(file_name);
    auto    lines      = readTextFile(file_name);
    string  text;

    for (auto const &line : lines)
        text += line;

    return     parser->parse(text); // ----->
}


void composeFile(string const &file_name, INode::TSharedPtr const &root_node) {
    auto parser         = createParserByExtension(file_name);
    auto text_writer    = CFileWriter::create(file_name);
    auto line           = parser->compose(root_node);

    text_writer->write(io::TBuffer(line.begin(), line.end()));
}


} // parsing
} // utility
