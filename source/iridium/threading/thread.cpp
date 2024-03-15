#include "thread.h"

#include "iridium/platform.h"
#include "iridium/convertion/convert.h"

#include <stdexcept>


using iridium::convertion::convert;
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


namespace iridium {
namespace threading {


thread_local std::string IThread::thread_name_static;


void sleep(int const &milliseconds) {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
}


std::string getThreadID() {
    return convert<string>(std::this_thread::get_id()); // ----->
}


std::string const &IThread::getNameStatic() {
    if (thread_name_static.empty())
        thread_name_static = convert<string>(std::this_thread::get_id());
    return thread_name_static; // ----->
}


void IThread::setNameStatic(std::string const &name) {
//    if (thread_name_static.empty())
    thread_name_static = convert<string>(std::this_thread::get_id()) + ":" + name;
//    else
//        throw std::runtime_error("static tread name setting error: twice set '" +
//            thread_name_static + "' = '" + name + "'"); // ----->
}


//std::__thread_id IThread::initMainThreadID() {
//    return std::this_thread::get_id();
//}
//std::__thread_id const IThread::thread_main_id = IThread::initMainThreadID();


} // threading
} // iridium
