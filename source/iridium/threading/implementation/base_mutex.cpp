/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "base_mutex.h"



namespace iridium {
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
} // iridium
