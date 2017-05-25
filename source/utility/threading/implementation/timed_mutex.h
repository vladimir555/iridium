#ifndef HEADER_TIMED_MUTEX_D7B87D44_698A_4699_A45E_4941FD101097
#define HEADER_TIMED_MUTEX_D7B87D44_698A_4699_A45E_4941FD101097


#include "utility/threading/mutex.h"

#include <mutex>
#include <string>


namespace utility {
namespace threading {
namespace implementation {


class CTimedMutex : public IMutex {
public:
    ///
    CTimedMutex() = default;
    ///
    virtual ~CTimedMutex() = default;
    ///
    virtual void lock() const override;
    ///
    virtual void unlock() const override;
private:
    ///
    mutable std::timed_mutex m_timed_mutex;
};


} // implementation
} // threading
} // utility


#endif // HEADER_TIMED_MUTEX_D7B87D44_698A_4699_A45E_4941FD101097
