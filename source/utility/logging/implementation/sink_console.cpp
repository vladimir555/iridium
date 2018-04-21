#include "sink_console.h"
#include <iostream>


using std::cout;
using std::endl;


namespace utility {
namespace logging {
namespace implementation {


CSinkConsole::CSinkConsole(TEvent::TLevel const &level)
:
    CSink(level, CWorkerHandler::create())
{}


CSinkConsole::CWorkerHandler::TItems CSinkConsole::CWorkerHandler::handle(TItems const &events) {
    for (auto const &e: events)
        cout << makeLine(e) << endl;
    return TItems(); // ----->
}


void CSinkConsole::CWorkerHandler::initialize() {
}


void CSinkConsole::CWorkerHandler::finalize() {
}


} // implementation
} // logging
} // utility
