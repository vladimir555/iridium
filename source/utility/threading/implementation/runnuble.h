#ifndef HEADER_RUNNUBLE_D8593C58_69DF_402C_AD3B_47FCD0EED7E8
#define HEADER_RUNNUBLE_D8593C58_69DF_402C_AD3B_47FCD0EED7E8


#include "utility/threading/runnable.h"

#include <atomic>


namespace utility {
namespace threading {
namespace implementation {


class CRunnuble: public IRunnable {
public:
    DEFINE_CREATE(CRunnuble)
    ///
    CRunnuble();
    ///
    virtual ~CRunnuble() = default;
    ///
    virtual void stop() override;

protected:
    ///
    std::atomic<bool> m_is_running;
};


} // implementation
} // threading
} // utility


#endif // HEADER_RUNNUBLE_D8593C58_69DF_402C_AD3B_47FCD0EED7E8
