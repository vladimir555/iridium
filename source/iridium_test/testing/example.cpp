#include <iridium/testing/tester.h>

#include <string>


using namespace std;


//TEST(bool_) {
//    ASSERT(1 == 2);
//    ASSERT(true);
//}


//TEST(throw_) {
//    std::string s;
//    ASSERT(s.empty(), std::exception);
//}


//TEST(comparing) {
//    ASSERT(7, less, 5);
//}


//TEST(uncached_throw) {
//    throw std::runtime_error("uncached_throw");
//}


TEST(OK) {
    //ASSERT(true);
    //ASSERT(1, equal, 1);
    //ASSERT("asd", equal, string("asd"));
    //ASSERT(2, greater, 1);

    double d = 5.5;
    ASSERT(5.5, equal, d);
}
