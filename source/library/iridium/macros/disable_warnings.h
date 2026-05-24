// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Manages compiler warning states by saving the current state and disabling specific warnings.
///
/// This file is intended to be included before sections of code that might generate
/// warnings which are known and accepted, or are difficult to avoid due to external library
/// constraints or specific C++ idioms.
///
/// **Functionality:**
/// - For Clang (`__clang__`): It uses `#pragma clang diagnostic push` to save the current
///   diagnostic state. Specific warnings can be disabled by adding
///   `#pragma clang diagnostic ignored "-W<warning-name>"` after the push directive if needed.
/// - For MSVC (`_MSC_VER`): It uses `#pragma warning(push)` to save the current warning state.
///   It then disables warning `4996`. Warning `4996` is commonly related to the use of
///   functions marked as deprecated by the compiler (e.g., older CRT functions deemed unsafe,
///   like `strcpy` or `sprintf`, for which bounds-checked alternatives like `strcpy_s` or
///   `sprintf_s` are recommended by Microsoft).
///
/// **Usage:**
/// This header should be included directly before the code block that generates the warnings
/// you wish to suppress. Immediately after that code block, `source/iridium/macros/enable_warnings.h`
/// **must** be included to restore the original warning state. Failure to do so will result in
/// the suppressed warning settings persisting for the rest of the translation unit, which is
/// generally undesirable.
///
/// Example:
/// ```cpp
/// #include "iridium/macros/disable_warnings.h"
/// // Code that might generate warning 4996 on MSVC or other temporarily suppressed warnings
/// // e.g., using an older, "unsafe" C function for specific reasons.
/// #include "iridium/macros/enable_warnings.h"
/// ```
///
/// \~russian @file
/// @brief Управляет состояниями предупреждений компилятора, сохраняя текущее состояние и отключая определенные предупреждения.
///
/// Этот файл предназначен для включения перед участками кода, которые могут генерировать
/// предупреждения, являющиеся известными и принятыми, или которые трудно избежать из-за
/// ограничений внешних библиотек или специфических идиом C++.
///
/// **Функциональность:**
/// - Для Clang (`__clang__`): Используется `#pragma clang diagnostic push` для сохранения текущего
///   состояния диагностики. Определенные предупреждения могут быть отключены добавлением
///   `#pragma clang diagnostic ignored "-W<имя-предупреждения>"` после директивы push, если это необходимо.
/// - Для MSVC (`_MSC_VER`): Используется `#pragma warning(push)` для сохранения текущего состояния предупреждений.
///   Затем отключается предупреждение `4996`. Предупреждение `4996` обычно связано с использованием
///   функций, помеченных компилятором как устаревшие (например, старые функции CRT, считающиеся небезопасными,
///   такие как `strcpy` или `sprintf`, для которых Microsoft рекомендует использовать альтернативы
///   с проверкой границ, например `strcpy_s` или `sprintf_s`).
///
/// **Использование:**
/// Этот заголовочный файл следует включать непосредственно перед блоком кода, который генерирует
/// предупреждения, которые вы хотите подавить. Сразу после этого блока кода **необходимо**
/// включить `source/iridium/macros/enable_warnings.h` для восстановления исходного состояния предупреждений.
/// Невыполнение этого требования приведет к тому, что подавленные настройки предупреждений будут
/// действовать до конца единицы трансляции, что обычно нежелательно.
///
/// Пример:
/// ```cpp
/// #include "iridium/macros/disable_warnings.h"
/// // Код, который может генерировать предупреждение 4996 в MSVC или другие временно подавляемые предупреждения
/// // например, использование старой, "небезопасной" функции C по определенным причинам.
/// #include "iridium/macros/enable_warnings.h"
/// ```

#ifdef __clang__
#pragma clang diagnostic push
#endif
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

// http://stackoverflow.com/questions/4193476/is-using-pragma-warning-push-pop-the-right-way-to-temporarily-alter-warning-lev
