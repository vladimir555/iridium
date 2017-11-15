#ifndef HEADER_MUTEX_7E2413B9_1604_4ADD_9540_34FED57BFF14
#define HEADER_MUTEX_7E2413B9_1604_4ADD_9540_34FED57BFF14


#include "base_mutex.h"

#include <mutex>


namespace utility {
namespace threading {
namespace implementation {


class CMutex: public CBaseMutex {
public:
    DEFINE_CREATE(CMutex)
    ///
    CMutex() = default;
    ///
    virtual ~CMutex() = default;
    ///
    void lock() const override;
    ///
    void unlock() const override;
protected:
    ///
    mutable std::mutex m_mutex;
};


} // implementation
} // threading
} // utility


#endif // HEADER_MUTEX_7E2413B9_1604_4ADD_9540_34FED57BFF14
