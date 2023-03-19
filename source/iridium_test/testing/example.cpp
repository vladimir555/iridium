#include <iridium/testing/tester.h>


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


//TEST(OK) {
//    //ASSERT(true);
//    //ASSERT(1, equal, 1);
//    //ASSERT("asd", equal, string("asd"));
//    //ASSERT(2, greater, 1);
//    double d = 5.5;
//    ASSERT(5.5, equal, d);
//}


// todo: move to /system/process
//TEST(process) {
//    string output;
//    try {
//        auto result = Command("./child2", std::chrono::seconds(5)).run(output);
//        LOGT << output;
//        LOGT << result;
//    } catch (Command::Exception const &e) {
//        LOGE << "error: " << e.what();
//        LOGE << "state: " << e.getState().condition;
//        LOGT << "output: " << output;
//    }
//}


//#include "iridium/threading/thread.h"
//TEST(timeout) {
//    LOGT << "sleep ...";
//    iridium::threading::sleep(30000);
//    LOGT << "sleep OK";
//}


//TEST(crash) {
//    int *a = nullptr;
//    *a = 5;
//}
