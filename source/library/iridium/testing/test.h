// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_TEST_2AF68F5B_6537_45C6_876E_EC1DB2BBAA67
#define HEADER_TEST_2AF68F5B_6537_45C6_876E_EC1DB2BBAA67


#include <string>
#include "iridium/parsing/node_type.h"


namespace iridium::testing {


class IUnitTestCase {
public:
    DEFINE_INTERFACE(IUnitTestCase)
    virtual void run() = 0;
    virtual size_t getLine() const = 0;
};


} // namespace iridium::testing


#endif // HEADER_TEST_2AF68F5B_6537_45C6_876E_EC1DB2BBAA67
