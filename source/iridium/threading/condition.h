// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONDITIONABLE_69F9FE49_C7A4_41D6_A7FD_CA07B36B351B
#define HEADER_CONDITIONABLE_69F9FE49_C7A4_41D6_A7FD_CA07B36B351B


#include "mutex.h"
#include "iridium/smart_ptr.h"

#include <chrono>


namespace iridium {
namespace threading {


///
class ICondition {
public:
    DEFINE_INTERFACE(ICondition)
    ///
    virtual void wait() const = 0;
    ///
    virtual bool wait(std::chrono::nanoseconds const &timeout) const = 0;
    ///
    virtual void notifyOne() const = 0;
    ///
    virtual void notifyAll() const = 0;
    ///
    virtual IMutex::TSharedPtr getMutex() = 0;
};


} // threading
} // iridium


#endif // HEADER_CONDITIONABLE_69F9FE49_C7A4_41D6_A7FD_CA07B36B351B
