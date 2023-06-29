// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STRINGS_C3181805_20F9_4385_B40C_23E8C7598D9B
#define HEADER_STRINGS_C3181805_20F9_4385_B40C_23E8C7598D9B


#include <string>
#include <vector>
#include <list>


namespace iridium {


/// \~english @brief split string to list of strings by delimeter string
/// \~russian @brief split разделить строку на список строк при помощи строки резделителя
/// \~english @param source source string
/// \~russian @param source исходная строка
/// \~english @param delimiter delimeter string
/// \~russian @param delimiter строка разделитель
/// \~english @param max_items maximum number of splits
/// \~russian @param max_items максимальное количество разделений
/// \~english @return string split result
/// \~russian @return результат разделения строки
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
///
std::string lowerCase(std::string const &source);
///
std::string upperCase(std::string const &source);
///
std::string replace(std::string const &source, std::string const &from, std::string const &to);
///
std::string rjust(std::string const &source, size_t const &width, char const &symbol);
/// \~english @brief remove symbols from begin and end of string
std::string trim(std::string const &source, std::string const &symbols);
/// \~english @brief symbols ' \t\\n\r' from begin and end of string
std::string trim(std::string const &source);


} // iridium


#endif // HEADER_STRINGS_C3181805_20F9_4385_B40C_23E8C7598D9B
