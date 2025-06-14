#include <iridium/testing/tester.h>

#include <iridium/system/implementation/process.h>
#include <iridium/system/shell.h>
#include <iridium/threading/thread.h>


using iridium::system::implementation::CProcessStream;
using iridium::system::Command;
using iridium::threading::sleep;


TEST(call) {
//    auto process = CProcessStream::create("ls");
//    process->initialize();
//    sleep(500);
//    process->initialize();
    auto command = Command::create("pwd");
    std::string output;
    command->run(output);
    sleep(500);
    LOGT << "'" << output << "'";
}
