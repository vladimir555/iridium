/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "sink.h"

#include "iridium/convertion/convert.h"
#include "iridium/threading/thread.h"

#include <chrono>


IMPLEMENT_ENUM(iridium::logging::ISink::TType)


using iridium::convertion::convert;
using std::string;
using std::chrono::system_clock;
using iridium::threading::getThreadID;


namespace iridium {
namespace logging {


std::string ISink::makeLine(TEvent const &e) {
    return
        convert<string>(system_clock::now()).substr(11, string::npos) + " " +
        convert<string>(e.level).substr(0, 1) + " " + e.line; // ----->
}


} // logger
} // iridium
