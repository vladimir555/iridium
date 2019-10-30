/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "request.h"

#include <string>

#include "iridium/strings.h"
#include "iridium/assert.h"
#include "iridium/convertion/convert.h"


using std::string;
using iridium::convertion::convert;


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace request {


THttp::TMessageLine convert(string const &source) {
    auto    lines = assertSize(split(source, " "), 3, "wrong http request line format: '" + source + "'");
    auto    line  = lines.begin();
    return THttp::TMessageLine( { convertion::convert<string>(*line++),  *line++, *line++ } ); // ----->
}


string convert(THttp::TMessageLine const &source) {
    return convertion::convert<string>(source.method) + " " + source.uri + " " + source.protocol; // ----->
}


} // request
} // http
} // protocol
} // io
} // iridium


IMPLEMENT_CONVERT(iridium::io::protocol::http::request::THttp::TMessageLine, std::string, iridium::io::protocol::http::request::convert)
IMPLEMENT_CONVERT(std::string, iridium::io::protocol::http::request::THttp::TMessageLine, iridium::io::protocol::http::request::convert)
