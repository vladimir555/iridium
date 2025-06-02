// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_PORT_44EAE897_34B0_426B_BD53_4421355EEBA6
#define HEADER_STREAM_PORT_44EAE897_34B0_426B_BD53_4421355EEBA6


#include "iridium/platform.h"


/// \~english @file stream_port.h
/// \~english @brief Defines the `CStreamPort` for the Linux platform.
/// \~english This implementation currently typedefs to the Unix `CStreamPort` implementation,
/// \~english effectively using the common Unix platform's stream port logic for Linux builds.
/// \~russian @file stream_port.h
/// \~russian @brief Определяет `CStreamPort` для платформы Linux.
/// \~russian Данная реализация в настоящее время использует typedef для Unix-реализации `CStreamPort`,
/// \~russian фактически используя общую логику потокового порта платформы Unix для сборок Linux.

#ifdef LINUX_PLATFORM


#include "../unix/stream_port.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


typedef platform::unix_::CStreamPort CStreamPort;


} // platform
} // implementation
} // io
} // iridium


#endif // LINUX_PLATFORM


#endif // HEADER_STREAM_PORT_44EAE897_34B0_426B_BD53_4421355EEBA6
