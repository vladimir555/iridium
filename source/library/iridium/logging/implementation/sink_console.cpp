// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

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


void CSinkConsole::finalize() {
    cout << std::flush;
}


} // iridium::logging::implementation
