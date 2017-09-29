#include "response.h"

#include <string>
#include <vector>

#include "utility/strings.h"
#include "utility/assert.h"
#include "utility/items.h"
#include "utility/convertion/convert.h"


using std::string;
using std::vector;
using utility::convertion::convert;


namespace {


static vector<string> const months  = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
static vector<string> const days    = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};


}


namespace utility {
namespace networking {
namespace http {
namespace response {


THttp::TMessageLine convertToMessageLine(string const &source) {
    auto    lines = assertSize(split(source, " "), 3, "wrong http request line format: '" + source + "'");
    auto    line  = lines.begin();
    return THttp::TMessageLine( { *line++,  convertion::convert<int>(*line++), *line++ } ); // ----->
}


string convert(THttp::TMessageLine const &source) {
    return source.protocol + " " + convertion::convert<string>(source.code) + " " + source.reason; // ----->
}


THttp::THeaders::THTTPDate convertToHTTPDate(string const &source) {
    // Mon, 27 Jul 2009 12:28:53 GMT
    auto items  = assign(assertSize(split(source, " "), 6, "parsing http date string error: '" + source + "'"));

    size_t month_number = 0;
    for (size_t i = 0; i < months.size(); i++)
        if (items[2] == months[i]) {
            month_number = i + 1;
            break;
        }
    if (month_number == 0)
        throw std::runtime_error("parsing http date string error: '" + source + "'");

    return THttp::THeaders::THTTPDate( {
        convertion::convert<std::chrono::high_resolution_clock::time_point>(
            items[3] + "-" + rjust(convertion::convert<string>(month_number), 2, '0') + "-" + items[1] + " " + items[4]
    ) } ); // ----->
}


string convert(THttp::THeaders::THTTPDate const &source) {
    string date = convertion::convert<string>(source.date);
    // 2009-07-27 12:28:53
    return days[0] + ", " + date.substr(8, 2) + " " + months[convertion::convert<uint64_t>(date.substr(5, 2))] + " " + date.substr(0, 4) + " " +
        date.substr(11, 8); // ----->
}


} // response
} // http
} // networking
} // utility


IMPLEMENT_CONVERT(utility::networking::http::response::THttp::TMessageLine, std::string, utility::networking::http::response::convertToMessageLine)
IMPLEMENT_CONVERT(std::string, utility::networking::http::response::THttp::TMessageLine, utility::networking::http::response::convert)


IMPLEMENT_CONVERT(utility::networking::http::response::THttp::THeaders::THTTPDate, std::string, utility::networking::http::response::convertToHTTPDate)
IMPLEMENT_CONVERT(std::string, utility::networking::http::response::THttp::THeaders::THTTPDate, utility::networking::http::response::convert)
