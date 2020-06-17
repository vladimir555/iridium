/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "file_parser.h"

#include "parser.h"

#include "implementation/parser_json.h"
#include "implementation/parser_xml.h"

#include "iridium/strings.h"
#include "iridium/io/fs/files.h"
#include "iridium/io/fs/implementation/file_stream_writer.h"


using std::string;

using iridium::io::fs::readTextFile;
using iridium::io::fs::extractFileNameExtension;
using iridium::io::fs::checkFileExistence;
using iridium::io::fs::implementation::CFileStreamWriter;
using iridium::io::Buffer;

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
    auto parser = createParserByExtension(file_name);
    auto writer = CFileStreamWriter::create(file_name);
    auto line   = parser->compose(root_node);

    writer->initialize();
    writer->write(Buffer::create(line.begin(), line.end()));
    writer->finalize();
}


} // parsing
} // iridium
