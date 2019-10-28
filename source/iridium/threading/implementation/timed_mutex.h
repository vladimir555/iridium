#ifndef HEADER_TIMED_MUTEX_D7B87D44_698A_4699_A45E_4941FD101097
#define HEADER_TIMED_MUTEX_D7B87D44_698A_4699_A45E_4941FD101097


#include "base_mutex.h"

#include <mutex>


namespace iridium {
namespace threading {
namespace implementation {


class CTimedMutex : public CBaseMutex {
public:
    DEFINE_IMPLEMENTATION(CTimedMutex)
    ///
    void lock() const override;
    ///
    void unlock() const override;
protected:
    ///
    mutable std::recursive_timed_mutex m_mutex;
};


} // implementation
} // threading
} // iridium


#endif // HEADER_TIMED_MUTEX_D7B87D44_698A_4699_A45E_4941FD101097
