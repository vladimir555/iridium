// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include <string>

#include "convert.h"


#ifdef FREEBSD_PLATFORM


using std::string;


namespace iridium::convertion::implementation {


string TConvert<string, long long>::convert(long long const &value) {
    char buffer[int_to_string_buffer_size];
    platform::itoa(static_cast<int64_t>(value), buffer, 10);
    return string(buffer);
}


} // iridium::convertion::implementation


#endif // FREEBSD_PLATFORM
