#ifndef HEADER_CONDITIONABLE_69F9FE49_C7A4_41D6_A7FD_CA07B36B351B
#define HEADER_CONDITIONABLE_69F9FE49_C7A4_41D6_A7FD_CA07B36B351B


#include "mutex.h"
#include "iridium/smart_ptr.h"


namespace iridium {
namespace threading {


///
class ICondition {
public:
    DEFINE_SMART_PTR(ICondition)
    ///
    virtual ~ICondition() = default;
    ///
    virtual void wait() const = 0;
    ///
    virtual void wait(int const &timeout_sec) const = 0;
    ///
    virtual void notifyOne() const = 0;
    ///
    virtual void notifyAll() const = 0;
};


} // threading
} // iridium


#endif // HEADER_CONDITIONABLE_69F9FE49_C7A4_41D6_A7FD_CA07B36B351B
