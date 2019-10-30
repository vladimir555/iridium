/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "timed_mutex.h"

#include <chrono>

#include "iridium/convertion/convert.h"


using std::string;
using iridium::convertion::convert;


namespace {


static auto const DEFAULT_LOCK_TIMEOUT = std::chrono::seconds(5);


}


namespace iridium {
namespace threading {
namespace implementation {


void CTimedMutex::lock() const {
    if (!m_mutex.try_lock_for(DEFAULT_LOCK_TIMEOUT))
        throw std::runtime_error("timed mutex lock error: timeout " + convert<string>(DEFAULT_LOCK_TIMEOUT.count()) + " sec"); // ----->
}


void CTimedMutex::unlock() const {
    m_mutex.unlock();
}


} // implementation
} // threading
} // iridium
