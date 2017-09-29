#include "request.h"

#include <string>

#include "utility/strings.h"
#include "utility/assert.h"
#include "utility/convertion/convert.h"


using std::string;
using utility::convertion::convert;


namespace utility {
namespace networking {
namespace http {


THttp::TRequestLine convert(string const &source) {
    auto    lines = assertSize(split(source, " "), 3, "wrong http request line format: '" + source + "'");
    auto    line  = lines.begin();
    return THttp::TRequestLine( { convertion::convert<string>(*line++),  *line++, *line++ } ); // ----->
}


string convert(THttp::TRequestLine const &source) {
    return convertion::convert<string>(source.method) + " " + source.uri + " " + source.protocol; // ----->
}


} // http
} // networking
} // utility


IMPLEMENT_CONVERT(utility::networking::http::THttp::TRequestLine, std::string, utility::networking::http::convert)
IMPLEMENT_CONVERT(std::string, utility::networking::http::THttp::TRequestLine, utility::networking::http::convert)
