#ifndef HEADER_PROCESS_E4AE8F87_99DE_490C_BEF4_19FFA3E0038D
#define HEADER_PROCESS_E4AE8F87_99DE_490C_BEF4_19FFA3E0038D


// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


#include "../unix/process.h"


namespace iridium::system::implementation::platform {


typedef platform::unix_::CProcessStream CProcessStream;


} // iridium::system::implementation::platform


#endif // MACOS_PLATFORM


#endif // HEADER_PROCESS_E4AE8F87_99DE_490C_BEF4_19FFA3E0038D
