// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FS_EC4EF5DB_F63C_4227_8830_C306F3060E07
#define HEADER_FS_EC4EF5DB_F63C_4227_8830_C306F3060E07


#include <string>
#include <vector>


namespace iridium {
namespace io {
namespace fs {


std::vector<std::string> readTextFile(std::string const &file_name, bool const &is_throw_if_not_exists = true);


bool checkFileExistence(std::string const &file_name);


void remove(std::string const &file_name);


std::string extractFileNameExtension(std::string const &file_name);


} // fs
} // io
} // iridium


#endif // HEADER_FS_EC4EF5DB_F63C_4227_8830_C306F3060E07
