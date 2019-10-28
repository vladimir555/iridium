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
