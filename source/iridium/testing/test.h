#ifndef HEADER_TEST_2AF68F5B_6537_45C6_876E_EC1DB2BBAA67
#define HEADER_TEST_2AF68F5B_6537_45C6_876E_EC1DB2BBAA67


#include <string>
#include "iridium/smart_ptr.h"


namespace iridium {
namespace testing {


class ITest {
public:
    DEFINE_INTERFACE(ITest)
    virtual void run() = 0;
};


} // testing
} // iridium


#endif // HEADER_TEST_2AF68F5B_6537_45C6_876E_EC1DB2BBAA67
