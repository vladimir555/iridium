// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Platform-specific header for FreeBSD to include appropriate conversion implementations.
///     This file ensures that when compiling for the FreeBSD platform, the necessary
///     conversion logic (typically sourced from common Unix implementations) is included.
/// \~russian @brief Платформо-специфичный заголовочный файл для FreeBSD для подключения соответствующих реализаций преобразования.
///     Этот файл гарантирует, что при компиляции для платформы FreeBSD подключается необходимая
///     логика преобразования (обычно взятая из общих реализаций для Unix).
#ifndef HEADER_CONVERT_BCE38770_C6BD_46D5_9485_3BC1F32BC3EF
#define HEADER_CONVERT_BCE38770_C6BD_46D5_9485_3BC1F32BC3EF


#include "iridium/platform.h"


/// \~english @brief Checks if the current compilation target is the FreeBSD platform.
///     If defined, it includes Unix-specific conversion implementations, as these are generally
///     applicable to FreeBSD or serve as a base for any FreeBSD-specific needs.
/// \~russian @brief Проверяет, является ли текущая цель компиляции платформой FreeBSD.
///     Если макрос определен, подключаются специфичные для Unix реализации преобразования,
///     поскольку они обычно применимы к FreeBSD или служат основой для любых специфичных нужд FreeBSD.
#ifdef FREEBSD_PLATFORM


/// \~english @brief Includes the Unix-specific conversion implementation header.
///     For the FreeBSD platform, Iridium's conversion logic reuses or bases itself on the
///     implementations provided for Unix-like systems.
/// \~russian @brief Подключает заголовочный файл с реализациями преобразований для Unix.
///     Для платформы FreeBSD логика преобразования Iridium повторно использует или основывается
///     на реализациях, предоставленных для Unix-подобных систем.
#include "../unix/convert.h"


#endif // FREEBSD_PLATFORM


#endif // HEADER_CONVERT_BCE38770_C6BD_46D5_9485_3BC1F32BC3EF
