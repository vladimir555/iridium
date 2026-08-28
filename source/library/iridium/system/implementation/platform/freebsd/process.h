#ifndef HEADER_PROCESS_D31F6948_E1CC_45E7_89EC_9BB8382D94B1
#define HEADER_PROCESS_D31F6948_E1CC_45E7_89EC_9BB8382D94B1


// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "iridium/platform.h"


#ifdef FREEBSD_PLATFORM


#include "../unix/process.h"


namespace iridium::system::implementation::platform {


typedef platform::unix_::CProcessStream CProcessStream;


} // iridium::system::implementation::platform


#endif // LINUX_PLATFORM


#endif // HEADER_PROCESS_D31F6948_E1CC_45E7_89EC_9BB8382D94B1
