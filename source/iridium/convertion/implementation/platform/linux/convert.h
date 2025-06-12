// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Platform-specific header for Linux to include appropriate conversion implementations.
///     This file ensures that when compiling for the Linux platform, the necessary
///     conversion logic (typically sourced from common Unix implementations) is included.
/// \~russian @brief Платформо-специфичный заголовочный файл для Linux для подключения соответствующих реализаций преобразования.
///     Этот файл гарантирует, что при компиляции для платформы Linux подключается необходимая
///     логика преобразования (обычно взятая из общих реализаций для Unix).
#ifndef HEADER_CONVERT_A8F9AEBC_6239_4771_BA93_C86AA9D34245
#define HEADER_CONVERT_A8F9AEBC_6239_4771_BA93_C86AA9D34245


#include "iridium/platform.h"


/// \~english @brief Checks if the current compilation target is the Linux platform.
///     If defined, it includes Unix-specific conversion implementations, as these are generally
///     applicable to Linux or serve as a base for any Linux-specific needs.
/// \~russian @brief Проверяет, является ли текущая цель компиляции платформой Linux.
///     Если макрос определен, подключаются специфичные для Unix реализации преобразования,
///     поскольку они обычно применимы к Linux или служат основой для любых специфичных нужд Linux.
#ifdef LINUX_PLATFORM


/// \~english @brief Includes the Unix-specific conversion implementation header.
///     For the Linux platform, Iridium's conversion logic reuses or bases itself on the
///     implementations provided for Unix-like systems.
/// \~russian @brief Подключает заголовочный файл с реализациями преобразований для Unix.
///     Для платформы Linux логика преобразования Iridium повторно использует или основывается
///     на реализациях, предоставленных для Unix-подобных систем.
#include "../unix/convert.h"


#endif // LINUX_PLATFORM


#endif // HEADER_CONVERT_A8F9AEBC_6239_4771_BA93_C86AA9D34245
