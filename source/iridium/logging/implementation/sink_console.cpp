#include "sink_console.h"
#include <iostream>


using std::cout;
using std::endl;


namespace iridium::logging::implementation {


CSinkConsole::CSinkConsole(TEvent::TLevel const &level)
:
    m_level(level)
{}


void CSinkConsole::log(TEvent::TConstSharedPtr const &event) {
    if (event->level >= m_level)
        cout << makeLine(event) << endl;
}


void CSinkConsole::flush() {
    cout << std::flush;
}


void CSinkConsole::initialize() {}


void CSinkConsole::finalize() {}


} // iridium::logging::implementation
