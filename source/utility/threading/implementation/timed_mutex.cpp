#include "timed_mutex.h"

#include <chrono>

#include "utility/convertion/convert.h"


using std::string;
using utility::convertion::convert;


namespace {


static auto const DEFAULT_LOCK_TIMEOUT = std::chrono::seconds(5);


}


namespace utility {
namespace threading {
namespace implementation {


void CTimedMutex::lock() const {
    if (!m_timed_mutex.try_lock_for(DEFAULT_LOCK_TIMEOUT))
        throw std::runtime_error("timed mutex lock error: timeout " + convert<string>(DEFAULT_LOCK_TIMEOUT.count()) + " sec"); // ----->
}


void CTimedMutex::unlock() const {
    m_timed_mutex.unlock();
}


} // implementation
} // threading
} // utility
