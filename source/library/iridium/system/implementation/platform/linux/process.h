#ifndef HEADER_PROCESS_B0266DAC_1BFC_4DEC_89F4_A3042BE3BEDD
#define HEADER_PROCESS_B0266DAC_1BFC_4DEC_89F4_A3042BE3BEDD


// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "../unix/process.h"


namespace iridium::system::implementation::platform {


typedef platform::unix_::CProcessStream CProcessStream;


} // iridium::system::implementation::platform


#endif // LINUX_PLATFORM


#endif // HEADER_PROCESS_B0266DAC_1BFC_4DEC_89F4_A3042BE3BEDD
