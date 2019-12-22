/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "response.h"

#include <string>
#include <vector>

#include "iridium/strings.h"
#include "iridium/assert.h"
#include "iridium/items.h"
#include "iridium/convertion/convert.h"


using std::string;
using std::vector;
using iridium::convertion::convert;


namespace {


static vector<string> const months  = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
static vector<string> const days    = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};


}


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace response {


std::string const THttp::DEFAULT_SERVER_NAME = "iridium";


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
            break; // --->
        }
    if (month_number == 0)
        throw std::runtime_error("parsing http date string error: '" + source + "'");

    return THttp::THeaders::THTTPDate( {
        convertion::convert<std::chrono::system_clock::time_point>(
            items[3] + "-" + rjust(convertion::convert<string>(month_number), 2, '0') + "-" + items[1] + " " + items[4]
    ) } ); // ----->
}


string convert(THttp::THeaders::THTTPDate const &source) {
    using iridium::convertion::convert;
    string date = convert<string>(source.date);
    // 2009-07-27 12:28:53
    // 0123456789012345678
    auto day    = days  [convert<unsigned int>(date.substr(8, 2)) % 7];
    auto month  = months[convert<unsigned int>(date.substr(5, 2)) % 12];
    return day + " " + month + " " + date.substr(0, 4) + " " + date.substr(11, 8) + " GMT"; // ----->
}


} // response
} // http
} // protocol
} // io
} // iridium


IMPLEMENT_CONVERT(iridium::io::protocol::http::response::THttp::TMessageLine, std::string, iridium::io::protocol::http::response::convertToMessageLine)
IMPLEMENT_CONVERT(std::string, iridium::io::protocol::http::response::THttp::TMessageLine, iridium::io::protocol::http::response::convert)


IMPLEMENT_CONVERT(iridium::io::protocol::http::response::THttp::THeaders::THTTPDate, std::string, iridium::io::protocol::http::response::convertToHTTPDate)
IMPLEMENT_CONVERT(std::string, iridium::io::protocol::http::response::THttp::THeaders::THTTPDate, iridium::io::protocol::http::response::convert)
