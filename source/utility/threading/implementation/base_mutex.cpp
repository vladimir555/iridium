#include "base_mutex.h"



namespace utility {
namespace threading {
namespace implementation {


void CBaseMutex::setScopeName(std::string const &scope_name) const {
    m_scope_name = scope_name;
}


std::string CBaseMutex::getScopeName() const {
    return m_scope_name;
}


} // implementation
} // threading
} // utility
