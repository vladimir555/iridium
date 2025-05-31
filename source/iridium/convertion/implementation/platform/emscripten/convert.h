// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Platform-specific header for Emscripten to include appropriate conversion implementations.
///     This file ensures that when compiling for the Emscripten platform, the necessary
///     conversion logic (typically sourced from Unix implementations) is included.
/// \~russian @brief Платформо-специфичный заголовочный файл для Emscripten для подключения соответствующих реализаций преобразования.
///     Этот файл гарантирует, что при компиляции для платформы Emscripten подключается необходимая
///     логика преобразования (обычно взятая из реализаций для Unix).
#ifndef HEADER_CONVERT_5FFD2726_9832_4923_A481_117BB9635596
#define HEADER_CONVERT_5FFD2726_9832_4923_A481_117BB9635596


#include "iridium/platform.h"


/// \~english @brief Checks if the current compilation target is the Emscripten platform.
///     If defined, it includes Unix-specific conversion implementations, as they are often
///     compatible or serve as a base for Emscripten.
/// \~russian @brief Проверяет, является ли текущая цель компиляции платформой Emscripten.
///     Если макрос определен, подключаются специфичные для Unix реализации преобразования,
///     поскольку они часто совместимы или служат основой для Emscripten.
#ifdef EMSCRIPTEN_PLATFORM


/// \~english @brief Includes the Unix-specific conversion implementation header.
///     For the Emscripten platform, Iridium's conversion logic reuses or bases itself on the
///     implementations provided for Unix-like systems.
/// \~russian @brief Подключает заголовочный файл с реализациями преобразований для Unix.
///     Для платформы Emscripten логика преобразования Iridium повторно использует или основывается
///     на реализациях, предоставленных для Unix-подобных систем.
#include "../unix/convert.h"


#endif // EMSCRIPTEN_PLATFORM


#endif // HEADER_CONVERT_5FFD2726_9832_4923_A481_117BB9635596
