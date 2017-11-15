#include "recursive_mutex.h"



namespace utility {
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
} // utility
