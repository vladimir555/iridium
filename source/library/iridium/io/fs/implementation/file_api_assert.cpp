#include "file_api_assert.h"

#include <string>
#include <stdexcept>


using std::string;


namespace iridium::io::fs::implementation {


int assertOK(const int &result, const string &message) {
    if (result != 0)
        throw std::runtime_error(message); // ----->
    else
        return result; // ----->
}


::FILE *assertOK(::FILE *result, const string &message) {
    if (result == nullptr)
        throw std::runtime_error(message); // ----->
    else
        return result; // ----->
}


} // iridium::io::fs::implementation
