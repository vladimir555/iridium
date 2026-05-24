// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_EVENT_PROVIDER_5371F671_E3F9_4164_8E88_8EEFC1583640
#define HEADER_EVENT_PROVIDER_5371F671_E3F9_4164_8E88_8EEFC1583640


#include "iridium/platform.h"


/// \~english @file multiplexer.h
/// \~english @brief Defines the multiplexer implementation for the macOS platform.
/// \~english This implementation includes and uses the FreeBSD `CMultiplexer` (kqueue-based)
/// \~english as macOS shares kqueue compatibility with FreeBSD.
/// \~russian @file multiplexer.h
/// \~russian @brief Определяет реализацию мультиплексора для платформы macOS.
/// \~russian Данная реализация включает и использует `CMultiplexer` для FreeBSD (на основе kqueue),
/// \~russian поскольку macOS разделяет совместимость kqueue с FreeBSD.

#ifdef MACOS_PLATFORM


#include "../freebsd/multiplexer.h"


#endif // MACOS_PLATFORM


#endif // HEADER_EVENT_PROVIDER_5371F671_E3F9_4164_8E88_8EEFC1583640
