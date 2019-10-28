#include "recursive_mutex.h"



namespace iridium {
namespace threading {
namespace implementation {


void CRecursiveMutex::lock() const {
    m_mutex.lock();
}


void CRecursiveMutex::unlock() const {
    m_mutex.unlock();
}


} // implementation
} // threading
} // iridium
