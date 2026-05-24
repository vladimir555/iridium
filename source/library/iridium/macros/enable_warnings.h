// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Restores a previously saved compiler warning state.
///
/// This file is intended to be used in conjunction with `source/iridium/macros/disable_warnings.h`.
/// After including `disable_warnings.h` to temporarily suppress certain compiler warnings
/// before a specific block of code, this file (`enable_warnings.h`) **must** be included
/// immediately after that code block to restore the original warning settings.
///
/// **Functionality:**
/// - For Clang (`__clang__`): It uses `#pragma clang diagnostic pop` to restore the diagnostic
///   state that was previously saved by `#pragma clang diagnostic push` (in `disable_warnings.h`).
/// - For MSVC (`_MSC_VER`): It uses `#pragma warning(pop)` to restore the warning state
///   that was previously saved by `#pragma warning(push)` (in `disable_warnings.h`).
///
/// **Usage:**
/// This header should be included directly after the code block for which warnings were
/// temporarily suppressed by `disable_warnings.h`. Refer to the documentation in
/// `source/iridium/macros/disable_warnings.h` for a complete usage example.
///
/// Example (conceptual, see `disable_warnings.h` for the full example):
/// ```cpp
/// // (In some_file.cpp)
/// #include "iridium/macros/disable_warnings.h"
/// // ... code that might generate warnings ...
/// #include "iridium/macros/enable_warnings.h" // Restores warnings here
/// ```
///
/// Failure to include this file after `disable_warnings.h` will result in the modified
/// warning settings persisting for the remainder of the translation unit, which can hide
/// potentially useful warnings and is generally not recommended.
///
/// \~russian @file
/// @brief Восстанавливает ранее сохраненное состояние предупреждений компилятора.
///
/// Этот файл предназначен для использования совместно с `source/iridium/macros/disable_warnings.h`.
/// После включения `disable_warnings.h` для временного подавления определенных предупреждений
/// компилятора перед конкретным блоком кода, этот файл (`enable_warnings.h`) **необходимо**
/// включить сразу после этого блока кода для восстановления исходных настроек предупреждений.
///
/// **Функциональность:**
/// - Для Clang (`__clang__`): Используется `#pragma clang diagnostic pop` для восстановления состояния
///   диагностики, которое ранее было сохранено директивой `#pragma clang diagnostic push` (в `disable_warnings.h`).
/// - Для MSVC (`_MSC_VER`): Используется `#pragma warning(pop)` для восстановления состояния
///   предупреждений, которое ранее было сохранено директивой `#pragma warning(push)` (в `disable_warnings.h`).
///
/// **Использование:**
/// Этот заголовочный файл следует включать непосредственно после блока кода, для которого
/// предупреждения были временно подавлены с помощью `disable_warnings.h`. Обратитесь
/// к документации в `source/iridium/macros/disable_warnings.h` для полного примера использования.
///
/// Пример (концептуальный, полный пример см. в `disable_warnings.h`):
/// ```cpp
/// // (В каком-то_файле.cpp)
/// #include "iridium/macros/disable_warnings.h"
/// // ... код, который может генерировать предупреждения ...
/// #include "iridium/macros/enable_warnings.h" // Восстанавливает предупреждения здесь
/// ```
///
/// Если не включить этот файл после `disable_warnings.h`, измененные настройки предупреждений
/// будут действовать до конца единицы трансляции, что может скрыть потенциально полезные
/// предупреждения и в целом не рекомендуется.

#ifdef __clang__
#pragma clang diagnostic pop
#endif
#ifdef _MSC_VER
#pragma warning(pop)
#endif
