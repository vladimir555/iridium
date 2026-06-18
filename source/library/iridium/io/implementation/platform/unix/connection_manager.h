// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONNECTION_MANAGER_EE8CF126_FBCE_4377_B313_0B388B65BD2B
#define HEADER_CONNECTION_MANAGER_EE8CF126_FBCE_4377_B313_0B388B65BD2B


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/uri.h"

#include <map>


namespace iridium::io::implementation::platform::unix_ {


class CConnectionManager {
public:
    CConnectionManager() = default;
private:
};


} // iridium::io::implementation::platform::unix_


#endif // UNIX_PLATFORM


#endif // HEADER_CONNECTION_MANAGER_EE8CF126_FBCE_4377_B313_0B388B65BD2B
