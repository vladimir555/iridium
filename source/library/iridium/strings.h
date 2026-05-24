// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STRINGS_C3181805_20F9_4385_B40C_23E8C7598D9B
#define HEADER_STRINGS_C3181805_20F9_4385_B40C_23E8C7598D9B


/// \~english @file
/// @brief Provides a set of utility functions for string manipulation.
/// \~russian @file
/// @brief Предоставляет набор вспомогательных функций для работы со строками.


#include <string>
#include <vector>
#include <list>


namespace iridium {


/// \~english @brief Splits a string by a delimiter, up to a maximum number of items.
/// \~russian @brief Разбивает строку по разделителю, но не более чем на максимальное количество элементов.
/// \~english @param source The string to split.
/// \~russian @param source Строка для разбиения.
/// \~english @param delimiter The delimiter string.
/// \~russian @param delimiter Строка-разделитель.
/// \~english @param max_items The maximum number of items to return.
/// \~russian @param max_items Максимальное количество возвращаемых элементов.
/// \~english @return A list of strings.
/// \~russian @return Список строк.
std::list<std::string> split(std::string const &source, std::string const &delimiter, size_t const &max_items);
/// \~english @brief Splits a string by a delimiter.
/// \~russian @brief Разбивает строку по разделителю.
/// \~english @param source The string to split.
/// \~russian @param source Строка для разбиения.
/// \~english @param delimiter The delimiter string.
/// \~russian @param delimiter Строка-разделитель.
/// \~english @return A list of strings.
/// \~russian @return Список строк.
std::list<std::string> split(std::string const &source, std::string const &delimiter);

/// \~english @brief Converts a string to lower case.
/// \~russian @brief Преобразует строку в нижний регистр.
/// \~english @param source The string to convert.
/// \~russian @param source Строка для преобразования.
/// \~english @return The lower-cased string.
/// \~russian @return Строка в нижнем регистре.
std::string lowerCase(std::string const &source);
/// \~english @brief Converts a string to upper case.
/// \~russian @brief Преобразует строку в верхний регистр.
/// \~english @param source The string to convert.
/// \~russian @param source Строка для преобразования.
/// \~english @return The upper-cased string.
/// \~russian @return Строка в верхнем регистре.
std::string upperCase(std::string const &source);

/// \~english @brief Replaces all occurrences of a substring with another substring.
/// \~russian @brief Заменяет все вхождения подстроки на другую подстроку.
/// \~english @param source The original string.
/// \~russian @param source Исходная строка.
/// \~english @param from The substring to replace.
/// \~russian @param from Заменяемая подстрока.
/// \~english @param to The substring to replace with.
/// \~russian @param to Подстрока для замены.
/// \~english @return The modified string.
/// \~russian @return Измененная строка.
std::string replace(std::string const &source, std::string const &from, std::string const &to);

/// \~english @brief Right-justifies a string by padding it with a symbol.
/// \~russian @brief Выравнивает строку по правому краю, дополняя ее символом.
/// \~english @param source The string to justify.
/// \~russian @param source Строка для выравнивания.
/// \~english @param width The desired width of the string.
/// \~russian @param width Желаемая ширина строки.
/// \~english @param symbol The symbol to use for padding.
/// \~russian @param symbol Символ для заполнения.
/// \~english @return The right-justified string.
/// \~russian @return Выровненная по правому краю строка.
std::string rjust(std::string const &source, size_t const &width, char const &symbol);
/// \~english @brief Left-justifies a string by padding it with a symbol.
/// \~russian @brief Выравнивает строку по левому краю, дополняя ее символом.
/// \~english @param source The string to justify.
/// \~russian @param source Строка для выравнивания.
/// \~english @param width The desired width of the string.
/// \~russian @param width Желаемая ширина строки.
/// \~english @param symbol The symbol to use for padding.
/// \~russian @param symbol Символ для заполнения.
/// \~english @return The left-justified string.
/// \~russian @return Выровненная по левому краю строка.
std::string ljust(std::string const &source, size_t const &width, char const &symbol);

/// \~english @brief Removes leading and trailing characters from a string.
/// \~russian @brief Удаляет начальные и конечные символы из строки.
/// \~english @param source The string to trim.
/// \~russian @param source Строка для обрезки.
/// \~english @param symbols The characters to remove.
/// \~russian @param symbols Символы для удаления.
/// \~english @return The trimmed string.
/// \~russian @return Обрезанная строка.
std::string trim(std::string const &source, std::string const &symbols);
/// \~english @brief Removes leading and trailing whitespace from a string.
/// \~russian @brief Удаляет начальные и конечные пробельные символы из строки.
/// \~english @param source The string to trim.
/// \~russian @param source Строка для обрезки.
/// \~english @return The trimmed string.
/// \~russian @return Обрезанная строка.
std::string trim(std::string const &source);


} // iridium


#endif // HEADER_STRINGS_C3181805_20F9_4385_B40C_23E8C7598D9B
