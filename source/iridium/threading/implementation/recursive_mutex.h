#ifndef HEADER_RECURSIVE_MUTEX_3CFC03BD_CF41_4162_9850_23A598835F85
#define HEADER_RECURSIVE_MUTEX_3CFC03BD_CF41_4162_9850_23A598835F85


#include "base_mutex.h"

#include <mutex>


namespace iridium {
namespace threading {
namespace implementation {


class CRecursiveMutex: public CBaseMutex {
public:
    DEFINE_CREATE(CRecursiveMutex)
    ///
    CRecursiveMutex() = default;
    ///
    virtual ~CRecursiveMutex() = default;
    ///
    void lock() const override;
    ///
    void unlock() const override;
protected:
    ///
    mutable std::recursive_mutex m_mutex;
};


} // implementation
} // threading
} // iridium



#endif // HEADER_RECURSIVE_MUTEX_3CFC03BD_CF41_4162_9850_23A598835F85
