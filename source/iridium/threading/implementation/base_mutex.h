// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_BASE_MUTEX_E1B41E03_6DD3_46CE_B69D_19EDE093B88E
#define HEADER_BASE_MUTEX_E1B41E03_6DD3_46CE_B69D_19EDE093B88E


#include "iridium/threading/mutex.h"


namespace iridium {
namespace threading {
namespace implementation {


class CBaseMutex: virtual public IMutex {
public:
    CBaseMutex() = default;
    virtual ~CBaseMutex() = default;

    void setScopeName(std::string const &scope_name) const override;
    std::string getScopeName() const override;
    // todo: maybe unlock m_scope_name.clear();
private:
    mutable std::string m_scope_name;
};


} // implementation
} // threading
} // iridium


#endif // HEADER_BASE_MUTEX_E1B41E03_6DD3_46CE_B69D_19EDE093B88E
