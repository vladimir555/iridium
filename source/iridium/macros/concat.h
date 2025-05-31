// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Provides preprocessor macros for token concatenation.
///     These macros are utility tools for meta-programming tasks that require pasting two tokens together.
/// \~russian @file
/// @brief Предоставляет макросы препроцессора для конкатенации токенов.
///     Эти макросы являются служебными инструментами для задач метапрограммирования, требующих склеивания двух токенов.

#ifndef HEADER_CONCAT_DAB81941_51B9_42ED_8BC6_248EF03E73AD
#define HEADER_CONCAT_DAB81941_51B9_42ED_8BC6_248EF03E73AD

/// \~english @def DEFINE_CONCAT_INNER(x, y)
/// @brief Inner macro that performs direct token concatenation using the preprocessor `##` operator.
///     This macro does not expand its arguments before concatenation.
/// \~russian @def DEFINE_CONCAT_INNER(x, y)
/// @brief Внутренний макрос, выполняющий прямое склеивание токенов с использованием оператора препроцессора `##`.
///     Этот макрос не раскрывает свои аргументы перед конкатенацией.
/// \~english @param x The first token.
/// \~russian @param x Первый токен.
/// \~english @param y The second token.
/// \~russian @param y Второй токен.
#define DEFINE_CONCAT_INNER(x, y) x##y

/// \~english @def DEFINE_CONCAT(x, y)
/// @brief Macro that performs token concatenation with prior expansion of its arguments.
///     This is often necessary when `x` or `y` are themselves macros whose results need to be
///     concatenated. It uses a two-step process (via `DEFINE_CONCAT_INNER`) to ensure arguments are expanded first.
/// \~russian @def DEFINE_CONCAT(x, y)
/// @brief Макрос, выполняющий конкатенацию токенов с предварительным раскрытием своих аргументов.
///     Это часто необходимо, когда `x` или `y` сами являются макросами, результаты которых нужно
///     склеить. Использует двухэтапный процесс (через `DEFINE_CONCAT_INNER`) для обеспечения предварительного раскрытия аргументов.
/// \~english @param x The first token or macro expanding to a token.
/// \~russian @param x Первый токен или макрос, раскрывающийся в токен.
/// \~english @param y The second token or macro expanding to a token.
/// \~russian @param y Второй токен или макрос, раскрывающийся в токен.
#define DEFINE_CONCAT(x, y) DEFINE_CONCAT_INNER(x, y)


#endif // HEADER_CONCAT_DAB81941_51B9_42ED_8BC6_248EF03E73AD
