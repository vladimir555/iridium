// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STRINGS_C3181805_20F9_4385_B40C_23E8C7598D9B
#define HEADER_STRINGS_C3181805_20F9_4385_B40C_23E8C7598D9B


#include <string>
#include <vector>
#include <list>


namespace iridium {


/// \~english @brief    split string to list of strings by delimeter
///     string
/// \~russian @brief    split разделить строку на список строк при помощи
///     строки резделителя
/// \~english @param    source source string
/// \~russian @param    source исходная строка
/// \~english @param    delimiter delimeter string
/// \~russian @param    delimiter строка разделитель
/// \~english @param    max_items maximum number of splits
/// \~russian @param    max_items максимальное количество разделений
/// \~english @return   string split result
/// \~russian @return   результат разделения строки
std::list<std::string> split(std::string const &source, std::string const &delimiter, size_t const &max_items);
/// \~english @brief split string to list of strings by delimeter string
/// \~russian @brief split разделить строку на список строк при помощи строки резделителя
/// \~english @param source source string
/// \~russian @param source исходная строка
/// \~english @param delimiter delimeter string
/// \~russian @param delimiter строка разделитель
/// \~english @return string split result
/// \~russian @return результат разделения строки
std::list<std::string> split(std::string const &source, std::string const &delimiter);
/// \~english @brief Converts a string to lowercase.
/// \~russian @brief Преобразует строку в нижний регистр.
/// \~english @param source The input string.
/// \~russian @param source Исходная строка.
/// \~english @return The lowercase version of the input string.
/// \~russian @return Версия исходной строки в нижнем регистре.
std::string lowerCase(std::string const &source);
/// \~english @brief Converts a string to uppercase.
/// \~russian @brief Преобразует строку в верхний регистр.
/// \~english @param source The input string.
/// \~russian @param source Исходная строка.
/// \~english @return The uppercase version of the input string.
/// \~russian @return Версия исходной строки в верхнем регистре.
std::string upperCase(std::string const &source);
/// \~english @brief Replaces all occurrences of a substring with another substring.
/// \~russian @brief Заменяет все вхождения подстроки на другую подстроку.
/// \~english @param source The original string.
/// \~russian @param source Исходная строка.
/// \~english @param from The substring to be replaced.
/// \~russian @param from Подстрока, которую нужно заменить.
/// \~english @param to The substring to replace with.
/// \~russian @param to Подстрока, на которую нужно заменить.
/// \~english @return A new string with all occurrences of `from` replaced by `to`.
/// \~russian @return Новая строка, в которой все вхождения `from` заменены на `to`.
std::string replace(std::string const &source, std::string const &from, std::string const &to);
/// \~english @brief Right-justifies a string within a specified width, padding with a given symbol.
/// \~russian @brief Выравнивает строку по правому краю в пределах указанной ширины, дополняя заданным символом.
/// \~english @param source The string to justify.
/// \~russian @param source Строка для выравнивания.
/// \~english @param width The total width of the resulting string.
/// \~russian @param width Общая ширина результирующей строки.
/// \~english @param symbol The character to use for padding on the left.
/// \~russian @param symbol Символ для заполнения слева.
/// \~english @return The right-justified string. If the source string's length is greater than or equal to width, the original string is returned.
/// \~russian @return Строка, выровненная по правому краю. Если длина исходной строки больше или равна ширине, возвращается исходная строка.
std::string rjust(std::string const &source, size_t const &width, char const &symbol);
/// \~english @brief Left-justifies a string within a specified width, padding with a given symbol.
/// \~russian @brief Выравнивает строку по левому краю в пределах указанной ширины, дополняя заданным символом.
/// \~english @param source The string to justify.
/// \~russian @param source Строка для выравнивания.
/// \~english @param width The total width of the resulting string.
/// \~russian @param width Общая ширина результирующей строки.
/// \~english @param symbol The character to use for padding on the right.
/// \~russian @param symbol Символ для заполнения справа.
/// \~english @return The left-justified string. If the source string's length is greater than or equal to width, the original string is returned.
/// \~russian @return Строка, выровненная по левому краю. Если длина исходной строки больше или равна ширине, возвращается исходная строка.
std::string ljust(std::string const &source, size_t const &width, char const &symbol);
/// \~english @brief Removes specified symbols from the beginning and end of a string.
/// \~russian @brief Удаляет указанные символы из начала и конца строки.
/// \~english @param source The input string.
/// \~russian @param source Исходная строка.
/// \~english @param symbols A string containing the set of characters to trim.
/// \~russian @param symbols Строка, содержащая набор символов для удаления.
/// \~english @return The trimmed string.
/// \~russian @return Строка с удаленными начальными и конечными указанными символами.
std::string trim(std::string const &source, std::string const &symbols);
/// \~english @brief Removes leading and trailing whitespace characters (space, tab, newline, carriage return) from a string.
/// \~russian @brief Удаляет начальные и конечные пробельные символы (пробел, табуляция, перевод строки, возврат каретки) из строки.
/// \~english @param source The input string.
/// \~russian @param source Исходная строка.
/// \~english @return The string with leading and trailing whitespace removed.
/// \~russian @return Строка с удаленными начальными и конечными пробельными символами.
std::string trim(std::string const &source);


} // iridium


#endif // HEADER_STRINGS_C3181805_20F9_4385_B40C_23E8C7598D9B
