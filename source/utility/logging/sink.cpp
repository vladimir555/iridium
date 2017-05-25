#include "sink.h"

#include "utility/convertion/convert.h"

#include <chrono>


IMPLEMENT_ENUM(utility::logging::ISink::TType)


using utility::convertion::convert;
using std::string;
using std::chrono::system_clock;


namespace utility {
namespace logging {


std::string ISink::makeLine(TEvent const &e) {
    return
        convert<string>(system_clock::now()).substr(11, string::npos) + " " + 
        convert<string>(std::this_thread::get_id()) + " " +
        convert<string>(e.level)[0]                 + " " + e.line; // ----->
}


} // logger
} // utility
