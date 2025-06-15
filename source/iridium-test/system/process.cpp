#include <iridium/testing/tester.h>

#include <iridium/system/implementation/process.h>
#include <iridium/system/shell.h>
#include <iridium/threading/thread.h>


using iridium::system::implementation::CProcessStream;
using iridium::system::Shell;
using iridium::threading::sleep;


TEST(shell) {
    auto shell  = Shell::create();
    shell->initialize();
    // todo: unix / windows test string
    auto result = shell->run("echo 1 && sleep 0.1 && echo 2");
    shell->finalize();
    ASSERT("1\n2\n", equal, result.output);
    ASSERT(0,        equal, result.code);
}
