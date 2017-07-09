#include "thread.h"


#include "utility/platform.h"
#include "utility/convertion/convert.h"


using utility::convertion::convert;
using std::string;


namespace {


#ifdef WINDOWS_PLATFORM
#pragma warning(push)
#pragma warning(disable:4073) // initializers put in library initialization area
#pragma init_seg(lib)
#if _MSC_VER < 1900
struct VS2013_threading_fix {
    VS2013_threading_fix() {
        _Cnd_do_broadcast_at_thread_exit();
    }
} threading_fix;
#endif // _MSC_VER
#pragma warning(pop)
#endif // WINDOWS_PLATFORM


}


namespace utility {
namespace threading {


void sleep(int const &milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}


std::string getThreadID() {
    return convert<string>(std::this_thread::get_id()); // ----->
}


} // threading
} // utility
