#include "files.h"

#include <fstream>
#include <stdio.h>


using std::string;
using std::vector;
using std::ifstream;


namespace iridium {
namespace io {
namespace fs {


vector<string> readTextFile(string const &file_name, bool const &is_throw_if_not_exists) {
    vector<string>  text;

    try {
        ifstream file(file_name);

        if (!is_throw_if_not_exists && !file)
            return text; // ----->

        if (file.is_open()) {
            while (file.good()) {
                string line;
                getline(file, line);
                text.push_back(line);
            }
            file.close();
        } else
            throw std::runtime_error("read file '" + file_name + "' error"); // ----->
    } catch (std::exception const &e) {
        throw std::runtime_error("read file '" + file_name + "' error: " + e.what()); // ----->
    }

    return text; // ----->
}


bool checkFileExistence(string const &file_name) {
    ifstream f(file_name);
    return static_cast<bool>(f); // ----->
}


void remove(string const &file_name) {
    if (checkFileExistence(file_name)) {
        auto result = std::remove(file_name.c_str());
        if (result != 0)
            throw std::runtime_error("removing file '" + file_name + "' error"); // ----->
    }
}


string extractFileNameExtension(string const &file_name) {
    auto i = file_name.find_last_of('.');
    if (i == string::npos)
        throw std::runtime_error("file name '" + file_name + "' have not extension"); // ----->
    else
        return file_name.substr(i + 1, file_name.size() - i); // ----->
}


} // fs
} // io
} // iridium
