#include <iridium/testing/tester.h>


//TEST(OK) {
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


//TEST(timeout) {
//    LOGT << "sleep ...";
//    iridium::threading::sleep(30000);
//    LOGT << "sleep OK";
//}


//TEST(crash) {
//    int *a = nullptr;
//    *a = 5;
//}
