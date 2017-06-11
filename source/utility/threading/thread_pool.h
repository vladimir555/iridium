#ifndef HEADER_THREAD_POOL_EF4A52A1_FBFD_4975_BFC8_607474576C7F
#define HEADER_THREAD_POOL_EF4A52A1_FBFD_4975_BFC8_607474576C7F


#include "utility/smart_ptr.h"


namespace utility {
namespace threading {


class IThreadPool {
public:
    DEFINE_SMART_PTR(IThreadPool)
    virtual ~IThreadPool() = default;
};


} // threading
} // utility


#endif // HEADER_THREAD_POOL_EF4A52A1_FBFD_4975_BFC8_607474576C7F
