#include "request.h"

#include <string>

#include "utility/strings.h"
#include "utility/assert.h"
#include "utility/convertion/convert.h"


using std::string;
using utility::convertion::convert;


namespace utility {
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
} // utility


IMPLEMENT_CONVERT(utility::io::protocol::http::request::THttp::TMessageLine, std::string, utility::io::protocol::http::request::convert)
IMPLEMENT_CONVERT(std::string, utility::io::protocol::http::request::THttp::TMessageLine, utility::io::protocol::http::request::convert)
