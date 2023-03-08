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


class CCondition: public ICondition, public CMutex {
public:
    DEFINE_IMPLEMENTATION(CCondition)
    ///
    CCondition() = default;
    ///
//    CCondition(IMutex::TSharedPtr const &mutex);
    ///
    void wait() const override;
    ///
    bool wait(std::chrono::nanoseconds const &timeout) const override;
    ///
    void notifyOne() const override;
    ///
    void notifyAll() const override;
//    ///
//    void lock() const override;
//    ///
//    void unlock() const override;
//    ///
//    void setScopeName(std::string const &scope_name) const override;
//    ///
//    std::string getScopeName() const override;

private:
    ///
    mutable std::condition_variable m_condition;
};


} // implementation
} // threading
} // iridium


#endif // HEADER_CONDITION_D45DDC63_D8F3_4F41_900C_FB8C895935B4
