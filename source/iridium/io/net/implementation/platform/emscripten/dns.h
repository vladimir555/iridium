// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_DNS_AB0D7512_3A21_4861_B95E_8430A460F8A3
#define HEADER_DNS_AB0D7512_3A21_4861_B95E_8430A460F8A3


#include "iridium/platform.h"


/// \~english @file dns.h
/// \~english @brief Provides platform-specific DNS resolution for Emscripten.
/// \~english This implementation includes and uses the Unix platform's DNS resolution functionality,
/// \~english as Emscripten often relies on POSIX-like features for networking.
/// \~russian @file dns.h
/// \~russian @brief Предоставляет платформо-специфичное разрешение DNS для Emscripten.
/// \~russian Данная реализация включает и использует функциональность разрешения DNS платформы Unix,
/// \~russian поскольку Emscripten часто полагается на POSIX-подобные функции для работы с сетью.

#ifdef EMSCRIPTEN_PLATFORM


#include "../unix/dns.h"


#endif // EMSCRIPTEN_PLATFORM


#endif // HEADER_DNS_AB0D7512_3A21_4861_B95E_8430A460F8A3
