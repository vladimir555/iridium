// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STRINGS_C3181805_20F9_4385_B40C_23E8C7598D9B
#define HEADER_STRINGS_C3181805_20F9_4385_B40C_23E8C7598D9B


#include <string>
#include <vector>
#include <list>


namespace iridium {


std::list<std::string> split(std::string const &source, std::string const &delimiter, size_t const &max_items);
std::list<std::string> split(std::string const &source, std::string const &delimiter);

std::string lowerCase(std::string const &source);
std::string upperCase(std::string const &source);

std::string replace(std::string const &source, std::string const &from, std::string const &to);

std::string rjust(std::string const &source, size_t const &width, char const &symbol);
std::string ljust(std::string const &source, size_t const &width, char const &symbol);

std::string trim(std::string const &source, std::string const &symbols);
std::string trim(std::string const &source);


} // iridium


#endif // HEADER_STRINGS_C3181805_20F9_4385_B40C_23E8C7598D9B
