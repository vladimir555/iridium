/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "sink_console.h"
#include <iostream>


using std::cout;
using std::endl;


namespace iridium {
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
} // iridium
