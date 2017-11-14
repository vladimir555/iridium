#ifndef HEADER_TIMED_MUTEX_D7B87D44_698A_4699_A45E_4941FD101097
#define HEADER_TIMED_MUTEX_D7B87D44_698A_4699_A45E_4941FD101097


#include "base_mutex.h"

#include <mutex>
#include <string>


namespace utility {
namespace threading {
namespace implementation {


class CTimedMutex : public CBaseMutex {
public:
    ///
    CTimedMutex() = default;
    ///
    virtual ~CTimedMutex() = default;
    ///
    void lock() const override;
    ///
    void unlock() const override;
private:
    ///
    mutable std::timed_mutex m_timed_mutex;
};


} // implementation
} // threading
} // utility


#endif // HEADER_TIMED_MUTEX_D7B87D44_698A_4699_A45E_4941FD101097
