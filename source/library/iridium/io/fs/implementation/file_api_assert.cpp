// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

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
