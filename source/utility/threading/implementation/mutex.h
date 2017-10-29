#ifndef HEADER_MUTEX_7E2413B9_1604_4ADD_9540_34FED57BFF14
#define HEADER_MUTEX_7E2413B9_1604_4ADD_9540_34FED57BFF14


#include "utility/threading/mutex.h"

#include <mutex>
#include <string>


namespace utility {
namespace threading {
namespace implementation {


class CMutex: public IMutex {
public:
    DEFINE_CREATE(CMutex)
    ///
    CMutex() = default;
    ///
    virtual ~CMutex() = default;
    ///
    virtual void lock() const override;
    ///
    virtual void unlock() const override;
protected:
    ///
    mutable std::mutex m_mutex;
};


} // implementation
} // threading
} // utility


#endif // HEADER_MUTEX_7E2413B9_1604_4ADD_9540_34FED57BFF14
