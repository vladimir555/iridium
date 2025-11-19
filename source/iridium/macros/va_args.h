// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Provides a set of preprocessor macros to emulate variadic macro dispatch based on argument count.
///     This system allows defining a base macro name that can effectively be "overloaded" to call
///     different underlying macros (e.g., `MY_MACRO_0`, `MY_MACRO_1`, ..., `MY_MACRO_7`) depending on the
///     number of arguments passed to it (supports 0 to 7 arguments).
///
/// **Usage Example:**
/// To use this system, you would typically define:
/// 1.  A series of macros for each argument count:
///     `#define MY_MACRO_0()`
///     `#define MY_MACRO_1(a1)`
///     `#define MY_MACRO_2(a1, a2)`
///     ...
///     `#define MY_MACRO_7(a1, ..., a7)`
/// 2.  A special "no argument expander" macro that provides a default (e.g., for 0 arguments) and padding for the argument counter:
///     `#define MY_MACRO_NO_ARG_EXPANDER() ,,,,,,,MY_MACRO_0` (Note: the number of commas is important for the counting mechanism).
/// 3.  A primary macro that uses `DEFINE_MACRO_CHOOSER`:
///     `#define MY_MACRO(...) DEFINE_MACRO_CHOOSER(MY_MACRO, __VA_ARGS__)(__VA_ARGS__)`
///
/// Now, calling `MY_MACRO()` would dispatch to `MY_MACRO_0()`, `MY_MACRO(x)` to `MY_MACRO_1(x)`, and so on.
/// The commented-out `ACTION` macro definitions within this file serve as a conceptual illustration.
///
/// \~russian @file
/// @brief Предоставляет набор макросов препроцессора для эмуляции диспетчеризации вариативных макросов на основе количества аргументов.
///     Эта система позволяет определить базовое имя макроса, которое может быть эффективно "перегружено" для вызова
///     различных базовых макросов (например, `MY_MACRO_0`, `MY_MACRO_1`, ..., `MY_MACRO_7`) в зависимости от
///     количества переданных ему аргументов (поддерживается от 0 до 7 аргументов).
///
/// **Пример использования:**
/// Для использования этой системы обычно необходимо определить:
/// 1.  Серию макросов для каждого количества аргументов:
///     `#define MY_MACRO_0()`
///     `#define MY_MACRO_1(a1)`
///     `#define MY_MACRO_2(a1, a2)`
///     ...
///     `#define MY_MACRO_7(a1, ..., a7)`
/// 2.  Специальный макрос-"расширитель без аргументов", который предоставляет значение по умолчанию (например, для 0 аргументов) и заполнение для счетчика аргументов:
///     `#define MY_MACRO_NO_ARG_EXPANDER() ,,,,,,,MY_MACRO_0` (Примечание: количество запятых важно для механизма подсчета).
/// 3.  Основной макрос, использующий `DEFINE_MACRO_CHOOSER`:
///     `#define MY_MACRO(...) DEFINE_MACRO_CHOOSER(MY_MACRO, __VA_ARGS__)(__VA_ARGS__)`
///
/// Теперь вызов `MY_MACRO()` будет диспетчеризован в `MY_MACRO_0()`, `MY_MACRO(x)` в `MY_MACRO_1(x)` и так далее.
/// Закомментированные определения макроса `ACTION` в этом файле служат концептуальной иллюстрацией.

#ifndef HEADER_VA_ARGS_A200B22F_46BD_4D03_A488_82E71F3E5F7F
#define HEADER_VA_ARGS_A200B22F_46BD_4D03_A488_82E71F3E5F7F


namespace iridium::macros {
} // namespace iridium::macros

/// \~english @def DEFINE_FUNC_CHOOSER_7(_f1, _f2, _f3, _f4, _f5, _f6, _f7, N, ... )
/// @brief A helper macro used to select the Nth argument from a list of arguments.
///     This is a key component in the argument counting mechanism. It effectively shifts arguments
///     such that the desired argument (which is one of `_f1` through `_f7` in the context of `DEFINE_CHOOSE_FROM_ARG_COUNT`)
///     becomes the `N` parameter, which is then returned.
/// \~russian @def DEFINE_FUNC_CHOOSER_7(_f1, _f2, _f3, _f4, _f5, _f6, _f7, N, ... )
/// @brief Вспомогательный макрос, используемый для выбора N-го аргумента из списка аргументов.
///     Это ключевой компонент в механизме подсчета аргументов. Он эффективно сдвигает аргументы
///     таким образом, что желаемый аргумент (который является одним из `_f1`...`_f7` в контексте `DEFINE_CHOOSE_FROM_ARG_COUNT`)
///     становится параметром `N`, который затем возвращается.
/// \~english @param _f1 Placeholder for the 7-argument version of a macro name.
/// \~russian @param _f1 Заполнитель для версии имени макроса с 7 аргументами.
/// \~english @param _f2 Placeholder for the 6-argument version.
/// \~russian @param _f2 Заполнитель для версии с 6 аргументами.
/// \~english @param _f3 Placeholder for the 5-argument version.
/// \~russian @param _f3 Заполнитель для версии с 5 аргументами.
/// \~english @param _f4 Placeholder for the 4-argument version.
/// \~russian @param _f4 Заполнитель для версии с 4 аргументами.
/// \~english @param _f5 Placeholder for the 3-argument version.
/// \~russian @param _f5 Заполнитель для версии с 3 аргументами.
/// \~english @param _f6 Placeholder for the 2-argument version.
/// \~russian @param _f6 Заполнитель для версии с 2 аргументами.
/// \~english @param _f7 Placeholder for the 1-argument version (or 0-argument version in some constructs).
/// \~russian @param _f7 Заполнитель для версии с 1 аргументом (или с 0 аргументов в некоторых конструкциях).
/// \~english @param N The argument that gets selected and returned by the macro.
/// \~russian @param N Аргумент, который выбирается и возвращается макросом.
/// \~english @param ... Variadic arguments that are effectively discarded after `N` is selected.
/// \~russian @param ... Вариативные аргументы, которые фактически отбрасываются после выбора `N`.
// --- поддержка до 7 аргументов включительно
#define DEFINE_FUNC_CHOOSER_7(_f1, _f2, _f3, _f4, _f5, _f6, _f7, N, ... ) N

/// \~english @def DEFINE_FUNC_RECOMPOSER(argsWithParentheses)
/// @brief Helper macro designed to correctly pass arguments (which are already enclosed in parentheses) to `DEFINE_FUNC_CHOOSER_7`.
///     This macro essentially removes one layer of macro expansion processing, allowing `DEFINE_FUNC_CHOOSER_7` to correctly interpret its arguments.
/// \~russian @def DEFINE_FUNC_RECOMPOSER(argsWithParentheses)
/// @brief Вспомогательный макрос, предназначенный для корректной передачи аргументов (уже заключенных в скобки) в `DEFINE_FUNC_CHOOSER_7`.
///     Этот макрос по сути удаляет один уровень обработки макрорасширения, позволяя `DEFINE_FUNC_CHOOSER_7` правильно интерпретировать свои аргументы.
/// \~english @param argsWithParentheses A list of arguments already enclosed in a single set of parentheses.
/// \~russian @param argsWithParentheses Список аргументов, уже заключенный в одну пару скобок.
#define DEFINE_FUNC_RECOMPOSER(argsWithParentheses)\
DEFINE_FUNC_CHOOSER_7 argsWithParentheses

/// \~english @def DEFINE_MACRO_CHOOSER(target_, ...)
/// @brief Core dispatch macro that selects a specific macro implementation (e.g., `target_1`, `target_2`) based on the number of variadic arguments provided.
///     It relies on a helper macro named `target_##_NO_ARG_EXPANDER` which must be defined by the user. This `_NO_ARG_EXPANDER`
///     macro provides the necessary padding and the name of the 0-argument version of the `target_` macro.
/// \~russian @def DEFINE_MACRO_CHOOSER(target_, ...)
/// @brief Основной макрос диспетчеризации, который выбирает конкретную реализацию макроса (например, `target_1`, `target_2`) на основе количества предоставленных вариативных аргументов.
///     Он полагается на вспомогательный макрос с именем `target_##_NO_ARG_EXPANDER`, который должен быть определен пользователем. Этот макрос `_NO_ARG_EXPANDER`
///     предоставляет необходимое заполнение и имя версии макроса `target_` для 0 аргументов.
/// \~english @param target_ The base name for the set of "overloaded" macros (e.g., if `MY_MACRO`, then implementations `MY_MACRO_1`, `MY_MACRO_2`, etc., are expected).
/// \~russian @param target_ Базовое имя для набора "перегруженных" макросов (например, если `MY_MACRO`, то ожидаются реализации `MY_MACRO_1`, `MY_MACRO_2` и т.д.).
/// \~english @param ... The variadic arguments passed to the user-facing macro.
/// \~russian @param ... Вариативные аргументы, переданные в макрос, видимый пользователю.
#define DEFINE_MACRO_CHOOSER(target_, ...)\
DEFINE_CHOOSE_FROM_ARG_COUNT(target_, target_##_NO_ARG_EXPANDER __VA_ARGS__ ())

/// \~english @def DEFINE_CHOOSE_FROM_ARG_COUNT(arg_, ...)
/// @brief Helper macro used by `DEFINE_MACRO_CHOOSER` to construct the call to the argument-specific macro.
///     It takes the base name (`arg_`) and the list of variadic arguments (which includes the `_NO_ARG_EXPANDER` output)
///     and uses `DEFINE_FUNC_RECOMPOSER` (and thus `DEFINE_FUNC_CHOOSER_7`) to select the correct suffixed macro name (e.g., `arg_1`, `arg_0`).
/// \~russian @def DEFINE_CHOOSE_FROM_ARG_COUNT(arg_, ...)
/// @brief Вспомогательный макрос, используемый `DEFINE_MACRO_CHOOSER` для конструирования вызова макроса, специфичного для количества аргументов.
///     Он принимает базовое имя (`arg_`) и список вариативных аргументов (который включает вывод `_NO_ARG_EXPANDER`)
///     и использует `DEFINE_FUNC_RECOMPOSER` (и, следовательно, `DEFINE_FUNC_CHOOSER_7`) для выбора правильного имени макроса с суффиксом (например, `arg_1`, `arg_0`).
/// \~english @param arg_ The base name of the target macro.
/// \~russian @param arg_ Базовое имя целевого макроса.
/// \~english @param ... The variadic arguments, which crucially start with the expansion of `target_##_NO_ARG_EXPANDER` followed by user arguments.
/// \~russian @param ... Вариативные аргументы, которые обязательно начинаются с раскрытия `target_##_NO_ARG_EXPANDER`, за которым следуют пользовательские аргументы.
#define DEFINE_CHOOSE_FROM_ARG_COUNT(arg_, ...) \
DEFINE_FUNC_RECOMPOSER((__VA_ARGS__, arg_##_7, arg_##_6, arg_##_5, arg_##_4, arg_##_3, arg_##_2, arg_##_1, ))

// -----

/*
//// реализации перегруженного макроса под различное количество аргументов
//#define dACTION_7(a1, a2, a3, a4, a5, a6, a7) \
//    std::cout << "7:ACTION(" << a1 << "," << a2 << "," << a3 << "," << a4 << ","<< a5 << "," << a6 << "," << a7 << ")\n"

//#define dACTION_6(a1, a2, a3, a4, a5, a6) \
//    std::cout << "6:ACTION(" << a1 << "," << a2 << "," << a3 << "," << a4 << ","<< a5 << "," << a6 << ")\n"

//#define dACTION_5(a1, a2, a3, a4, a5) \
//    std::cout << "5:ACTION(" << a1 << "," << a2 << "," << a3 << "," << a4 << ","<< a5 << ")\n"

//#define dACTION_4(a1, a2, a3, a4) \
//    std::cout << "4:ACTION(" << a1 << "," << a2 << "," << a3 << "," << a4 << ")\n"

//#define dACTION_3(a1, a2, a3) \
//    std::cout << "3:ACTION(" << a1 << "," << a2 << "," << a3 << ")\n"

//#define dACTION_2(a1, a2) \
//    std::cout << "2:ACTION(" << a1 << "," << a2 << ")\n"

//#define dACTION_1(a1) \
//    std::cout << "1:ACTION(" << a1 << ")\n"

//#define dACTION_0() \
//    std::cout << "0:ACTION()\n"


//#define dACTION_NO_ARG_EXPANDER() \
//    ,,,,,,,dACTION_0

//#define ACTION(...)\
//    dMACRO_CHOOSER( dACTION, __VA_ARGS__)(__VA_ARGS__)


//#include <iostream>

//int main()
//{
//    std::cout << "Hello, world!\n";

//    ACTION();
//    ACTION(1);
//    ACTION(1,2);
//    ACTION(1,2,3);
//    ACTION(1,2,3,4);
//    ACTION(1,2,3,4,5);
//    ACTION(1,2,3,4,5,6);
//    ACTION(1,2,3,4,5,6,7);
//}
*/


#endif // HEADER_VA_ARGS_A200B22F_46BD_4D03_A488_82E71F3E5F7F
