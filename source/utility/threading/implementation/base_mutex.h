#ifndef HEADER_BASE_MUTEX_E1B41E03_6DD3_46CE_B69D_19EDE093B88E
#define HEADER_BASE_MUTEX_E1B41E03_6DD3_46CE_B69D_19EDE093B88E


#include "utility/threading/mutex.h"


namespace utility {
namespace threading {
namespace implementation {


class CBaseMutex: public IMutex {
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
} // utility


#endif // HEADER_BASE_MUTEX_E1B41E03_6DD3_46CE_B69D_19EDE093B88E
