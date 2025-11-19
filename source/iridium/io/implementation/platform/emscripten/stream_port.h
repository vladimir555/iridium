// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_PORT_810D3421_E75E_49EB_A765_C12A13401FDF
#define HEADER_STREAM_PORT_810D3421_E75E_49EB_A765_C12A13401FDF


#include "iridium/platform.h"


/// \~english @file stream_port.h
/// \~english @brief Defines the `CStreamPort` for the Emscripten platform.
/// \~english This implementation currently typedefs to the Unix `CStreamPort` implementation,
/// \~english effectively using the Unix platform's stream port logic for Emscripten builds.
/// \~russian @file stream_port.h
/// \~russian @brief Определяет `CStreamPort` для платформы Emscripten.
/// \~russian Данная реализация в настоящее время использует typedef для Unix-реализации `CStreamPort`,
/// \~russian фактически используя логику потокового порта платформы Unix для сборок Emscripten.

#ifdef EMSCRIPTEN_PLATFORM


#include "../unix/stream_port.h"


namespace iridium::io::implementation::platform {


typedef platform::unix_::CStreamPort CStreamPort;


} // iridium::io::implementation::platform


#endif // EMSCRIPTEN_PLATFORM


#endif // HEADER_STREAM_PORT_810D3421_E75E_49EB_A765_C12A13401FDF
