// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONDITION_D45DDC63_D8F3_4F41_900C_FB8C895935B4
#define HEADER_CONDITION_D45DDC63_D8F3_4F41_900C_FB8C895935B4


#include <condition_variable>
#include <atomic>

#include "iridium/smart_ptr.h"
#include "iridium/threading/condition.h"
#include "mutex.h"


namespace iridium {
namespace threading {
namespace implementation {


class CCondition : protected CMutex, public ICondition {
public:
    DEFINE_IMPLEMENTATION(CCondition)
    ///
    CCondition() = default;
    ///
    virtual void wait() const override;
    ///
    virtual void wait(int const &timeout_sec) const override;
    ///
    virtual void notifyOne() const override;
    ///
    virtual void notifyAll() const override;

private:
    ///
    mutable std::condition_variable m_condition;
};


} // implementation
} // threading
} // iridium


#endif // HEADER_CONDITION_D45DDC63_D8F3_4F41_900C_FB8C895935B4
