#include "sink.h"

#include "utility/convertion/convert.h"
#include "utility/threading/thread.h"

#include <chrono>


IMPLEMENT_ENUM(utility::logging::ISink::TType)


using utility::convertion::convert;
using std::string;
using std::chrono::system_clock;
using utility::threading::getThreadID;


namespace utility {
namespace logging {


std::string ISink::makeLine(TEvent const &e) {
    return
        convert<string>(system_clock::now()).substr(11, string::npos) + " " +
        convert<string>(e.level).substr(0, 1) + " " + e.line; // ----->
}


} // logger
} // utility
