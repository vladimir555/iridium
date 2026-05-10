#include <iridium/testing/tester.h>

#include <iridium/system/implementation/process.h>
#include <iridium/system/shell.h>
#include <iridium/threading/thread.h>


using iridium::system::implementation::CProcessStream;
using iridium::system::Shell;
using iridium::threading::sleep;


static std::string normalizeNewlines(std::string s) {
    std::string r;
    r.reserve(s.size());
    for (char c : s)
        if (c != '\r') r.push_back(c);
    return r;
}


TEST(shell) {
    auto shell  = Shell::create();
    shell->initialize();
    auto result = shell->run("echo 1 ; sleep 0.1 ; echo 2");
    shell->finalize();

    // LOGT << "output: '" << result.output << "'";

    auto output = normalizeNewlines(result.output);

    ASSERT("1\n2\n", equal, output);
    ASSERT(0, equal, result.code);
}


// TEST(value) {
    // LOGT << convert<std::string, int32_t, false>(int{-1});
    // // todo: fix the issue with static_assert in convert when is_throwable=false
    // LOGT << convert<std::string, int32_t, true> (int{-1});
// }
