#include "sink_console.h"
#include <iostream>


using std::cout;
using std::endl;


namespace utility {
namespace logging {
namespace implementation {


CSinkConsole::CSinkConsole(TEvent::TLevel const &level)
:
    m_level(level)
{}


void CSinkConsole::log(TEvent const &event) {
    if (event.level >= m_level)
        cout << makeLine(event) << endl;
}


void CSinkConsole::initialize() {
}


void CSinkConsole::finalize() {
}


} // implementation
} // logging
} // utility
