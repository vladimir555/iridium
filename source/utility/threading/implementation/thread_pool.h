#ifndef HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47
#define HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47


#include "utility/threading/thread_pool.h"


namespace utility {
namespace threading {
namespace implementation {


class CThreadPool: public IThreadPool {
public:
    DEFINE_CREATE(CThreadPool)
    virtual ~CThreadPool() = default;

    CThreadPool(size_t const &max_thread_count);
};


} // implementation
} // threading
} // utility


#endif // HEADER_THREAD_POOL_B90973E0_2CF8_4067_99BC_8460E3B83D47
