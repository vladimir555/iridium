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


CSinkConsole::CSinkWorker::CSinkWorker()
:
    CWorker<TEvent>("ConsoleSink")
{}


CSinkConsole::CSinkConsole(TEvent::TLevel const &level)
:
    CSink(level, CSinkWorker::create())
{}


void CSinkConsole::CSinkWorker::handleItems(TItems const &events) {
    for (auto const &e: events)
        cout << makeLine(e) << endl;
}


} // implementation
} // logging
} // utility
