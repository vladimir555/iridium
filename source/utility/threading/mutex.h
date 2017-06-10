#ifndef HEADER_SYNCHRONIZABLE_0DFC683E_069B_40C2_B135_2A2CB9FD8D49
#define HEADER_SYNCHRONIZABLE_0DFC683E_069B_40C2_B135_2A2CB9FD8D49


#include <string>
#include "utility/smart_ptr.h"


namespace utility {
namespace threading {


///
class IMutex {
public:
    DEFINE_SMART_PTR(IMutex)
    ///
    virtual ~IMutex() = default;
    ///
    virtual void lock() const = 0;
    ///
    virtual void unlock() const = 0;
    /// todo: move to base mutex
    std::string m_scope_name;
};


} // threading
} // utility


#endif // HEADER_SYNCHRONIZABLE_0DFC683E_069B_40C2_B135_2A2CB9FD8D49
