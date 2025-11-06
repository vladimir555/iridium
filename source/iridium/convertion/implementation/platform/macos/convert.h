// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Platform-specific header for macOS to include appropriate conversion implementations.
///     This file ensures that when compiling for the macOS platform, the necessary
///     conversion logic (typically sourced from common Unix implementations) is included.
/// \~russian @brief Платформо-специфичный заголовочный файл для macOS для подключения соответствующих реализаций преобразования.
///     Этот файл гарантирует, что при компиляции для платформы macOS подключается необходимая
///     логика преобразования (обычно взятая из общих реализаций для Unix).
#ifndef HEADER_CONVERT_73407719_A933_45DE_9A89_8E2FE63B0DD6
#define HEADER_CONVERT_73407719_A933_45DE_9A89_8E2FE63B0DD6


#include "iridium/platform.h"


/// \~english @brief Checks if the current compilation target is the macOS platform.
///     If defined, it includes Unix-specific conversion implementations, as these are generally
///     applicable to macOS (which is a Unix-based system) or serve as a base for any macOS-specific needs.
/// \~russian @brief Проверяет, является ли текущая цель компиляции платформой macOS.
///     Если макрос определен, подключаются специфичные для Unix реализации преобразования,
///     поскольку они обычно применимы к macOS (которая является Unix-подобной системой) или служат основой для любых специфичных нужд macOS.
#ifdef MACOS_PLATFORM


/// \~english @brief Includes the Unix-specific conversion implementation header.
///     For the macOS platform, Iridium's conversion logic reuses or bases itself on the
///     implementations provided for Unix-like systems, leveraging its Unix underpinnings.
/// \~russian @brief Подключает заголовочный файл с реализациями преобразований для Unix.
///     Для платформы macOS логика преобразования Iridium повторно использует или основывается
///     на реализациях, предоставленных для Unix-подобных систем, используя ее Unix-основу.
#include "../unix/convert.h"

#include "iridium/convertion/implementation/common.h"


namespace iridium::convertion::implementation {


template<>
struct TConvert<std::string, long> {
    static std::string convert(long const &value);
};


template<>
struct TConvert<std::string, unsigned long> {
    static std::string convert(unsigned long const &value);
};


} // iridium::convertion::implementation


#endif // MACOS_PLATFORM


#endif // HEADER_CONVERT_73407719_A933_45DE_9A89_8E2FE63B0DD6
