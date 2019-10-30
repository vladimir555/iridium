/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "mutex.h"



namespace iridium {
namespace threading {
namespace implementation {


void CMutex::lock() const {
    m_mutex.lock();
}


void CMutex::unlock() const {
    m_mutex.unlock();
}


} // implementation
} // threading
} // iridium
