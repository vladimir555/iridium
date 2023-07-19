#include <iridium/testing/tester.h>


#include <iridium/system/implementation/process.h>
#include <iridium/io/implementation/multiplexer.h>
#include <iridium/items.h>


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
//    LOGT << "trace log";
//    LOGD << "debug log";
//    LOGI << "info  log";
    
//    //ASSERT(true);
//    //ASSERT(1, equal, 1);
//    //ASSERT("asd", equal, string("asd"));
//    //ASSERT(2, greater, 1);
//    double d = 5.5;
//    ASSERT(5.5, equal, d);
}


//TEST(sleep) {
//    using iridium::threading::sleep;
//
//    for (uint32_t i = 0; i < 10; i++) {
//        LOGT << i;
//        sleep(1000);
//    }
//}


//template<typename TResult>
//TResult assertOK(TResult const &result, std::string const &method) {
//    if (result) {
//        return result;
//    } else {
//        DWORD error_message_code    = ::GetLastError();
//        LPSTR buffer                = nullptr;
//
//        size_t size = FormatMessageA(
//            FORMAT_MESSAGE_ALLOCATE_BUFFER  | 
//            FORMAT_MESSAGE_FROM_SYSTEM      | 
//            FORMAT_MESSAGE_IGNORE_INSERTS,
//            NULL, error_message_code, 
//            MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), 
//            (LPSTR)&buffer, 0, NULL);
//
//        std::string message(buffer, size);
//        LocalFree(buffer);
//        throw std::runtime_error(method + " error: " + message); // ----->
//    }
//}



//// todo: move to /system/process
//TEST(process) {
//    using iridium::system::implementation::CProcessStream;
//    using iridium::io::implementation::CMultiplexer;
//    using iridium::io::Buffer;
//    using iridium::io::IEvent;
//    using iridium::convertion::convert;
//    using iridium::checkOneOf;
//
//    auto multiplexer = CMultiplexer::create();
//
//    multiplexer->initialize();
//
//    auto process = CProcessStream::create("iridium_test.exe", "run --mode=raw /testing/example.cpp/sleep");
//
//    multiplexer->subscribe(process);
//
//    std::list<Buffer::TSharedPtr> buffers;
//
//    for (int i = 0; i < 5; i++) {
//        LOGT << "wait events ...";
//        auto events = multiplexer->waitEvents();
//        LOGT << "wait events OK";
//
//        for (auto const& event_: events) {
//            LOGT << event_->getType();
//            if (checkOneOf(event_->getType(), IEvent::TType::READ, IEvent::TType::UNKNOWN)) {
//                auto buffer = process->read(1024);
//                buffers.push_back(buffer);
//                LOGT << "stdout:\n'" << buffer << "'";
//            }
//        }
//    }
//
//    LOGT << process->getState().condition << " " << process->getState().exit_code;
//    iridium::threading::sleep(100);
//    LOGT << process->getState().condition << " " << process->getState().exit_code;
//
//    multiplexer->unsubscribe(process);
//    multiplexer->finalize();
//
//    std::string output;
//    for (auto buffer: buffers)
//        output += convert<std::string>(*buffer);
//
//    LOGT << "\noutput:\n" << output;
//
//    //string output;
//    //try {
//    //    auto result = Command("./child2", std::chrono::seconds(5)).run(output);
//    //    LOGT << output;
//    //    LOGT << result;
//    //} catch (Command::Exception const &e) {
//    //    LOGE << "error: " << e.what();
//    //    LOGE << "state: " << e.getState().condition;
//    //    LOGT << "output: " << output;
//    //}
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
