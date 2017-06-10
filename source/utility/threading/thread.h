#ifndef HEADER_THREAD_EC50D1AB_7469_400A_90D9_4977B01B0A71
#define HEADER_THREAD_EC50D1AB_7469_400A_90D9_4977B01B0A71


#include "utility/pattern/initializable.h"
#include "utility/smart_ptr.h"

#include <string>
#include <chrono>
#include <thread>


namespace utility {
namespace threading {


///
class IThread: 
    public pattern::IInitializable
{
public:
    DEFINE_SMART_PTR(IThread)
    ///
    virtual ~IThread() = default;
    /// for std::map caching
    virtual bool operator < (IThread const &thread) const = 0;
    ///
    virtual std::string getName() const = 0;
    ///
    static void sleep(int const &milliseconds);
};


} // threading
} // utility


#endif // HEADER_THREAD_EC50D1AB_7469_400A_90D9_4977B01B0A71
