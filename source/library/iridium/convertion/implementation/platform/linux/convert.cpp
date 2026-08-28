// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "convert.h"


#ifdef LINUX_PLATFORM


namespace iridium::convertion::implementation {


std::string TConvert<std::string, long long>::convert(long long const &value) {
    return std::to_string(value);
}


} // iridium::convertion::implementation


#endif // LINUX_PLATFORM
