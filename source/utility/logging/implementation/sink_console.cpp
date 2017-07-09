#include "sink_console.h"

#include <iostream>

#include "utility/convertion/convert.h"


using std::cout;
using std::endl;
using std::string;

using utility::convertion::convert;
using utility::threading::implementation::CWorker;


namespace utility {
namespace logging {
namespace implementation {


CSinkConsole::CSinkConsole(TEvent::TLevel const &level)
:
    CSink(level, getSharedThis(this))
{}


void CSinkConsole::handleItems(TItems const &events) {
    for (auto const &e: events)
        cout << makeLine(e) << endl;
}


void CSinkConsole::handleStart() {
}


void CSinkConsole::handleStop() {
}


} // implementation
} // logging
} // utility
