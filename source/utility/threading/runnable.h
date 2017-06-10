#ifndef HEADER_RUNNUBLE_82187C6C_2498_4042_A14B_902FECFE4570
#define HEADER_RUNNUBLE_82187C6C_2498_4042_A14B_902FECFE4570


#include "utility/smart_ptr.h"

#include <string>


namespace utility {
namespace threading {


///
class IRunnable {
public:
    DEFINE_SMART_PTR(IRunnable)
    ///
    virtual ~IRunnable() = default;
    ///
    virtual void run() = 0;
    ///
    virtual void stop() = 0;
};


} // threading
} // utility


#endif // HEADER_RUNNUBLE_82187C6C_2498_4042_A14B_902FECFE4570
