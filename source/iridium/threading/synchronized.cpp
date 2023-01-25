/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#ifndef HEADER_PROTOCOL_FACTORY_9A7C3633_1E55_40E8_A2EA_EB216B70D563
#define HEADER_PROTOCOL_FACTORY_9A7C3633_1E55_40E8_A2EA_EB216B70D563


#include "synchronized.h"
#include "iridium/assert.h"

#include "implementation/mutex.h"


#include <iostream>
namespace iridium {
namespace threading {


Synchronized::Synchronized(IMutex::TSharedPtr const &mutex)
:
    m_mutex(mutex)
{}


Synchronized::Synchronized()
:
    m_mutex(implementation::CMutex::create())
{}


IMutex::TSharedPtr Synchronized::getMutex() const {
    return m_mutex; // ----->
}


} // threading
} // iridium


#endif // HEADER_PROTOCOL_FACTORY_9A7C3633_1E55_40E8_A2EA_EB216B70D563
