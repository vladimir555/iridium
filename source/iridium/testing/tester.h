#ifndef HEADER_TESTER_2DFB0DB0_F2C1_425A_947D_7883FD5FAB64
#define HEADER_TESTER_2DFB0DB0_F2C1_425A_947D_7883FD5FAB64


#include "iridium/pattern/singleton.h"
#include "iridium/parsing/node_type.h"
#include "iridium/smart_ptr.h"
#include "iridium/logging/logger.h"
#include "iridium/convertion/convert.h"
#include "iridium/macros/va_args.h"

#include <string>
#include <map>
#include "test.h"
#include "unit_test.h"


namespace iridium {
namespace testing {


typedef parsing::INodeType<ITest *> INodeTest;


class Tester final: public pattern::Singleton<Tester> {
public:
    virtual ~Tester() = default;
    ///
    void add(ITest * const test, std::string const &path);
    ///
    int  run(int argc, char* argv[], std::string const &main_cpp_path);

private:
    friend class pattern::Singleton<Tester>;
    ///
    void runTests(INodeTest::TSharedPtr const &node, std::string const &path);
    ///
    Tester();
    ///
    std::string             m_path_base;
    std::string             m_path_include;
    std::string             m_path_exclude;
    INodeTest::TSharedPtr   m_test_root_node;

    std::list< std::pair< ITest *, std::string > > m_test_list;
    std::list<std::string>  m_failed_paths;
};


} // testing
} // iridium


DEFINE_CONVERT(std::string, iridium::testing::INodeTest::TConstSharedPtr)
DEFINE_CONVERT(std::string, iridium::testing::INodeTest::TSharedPtr)


#define TEST(name) \
class Test_##name final: public iridium::testing::ITest, public iridium::testing::UnitTest { \
public: \
    DEFINE_IMPLEMENTATION(Test_##name) \
    Test_##name() { iridium::testing::Tester::instance().add(this, std::string(__FILE__) + "/" + #name); } \
    void run() override; \
} test_##name; \
void Test_##name::run()

#define ASSERT_3(left, condition, right) \
condition(left, right, std::string(#left) + " " + #condition + " " + #right, iridium::convertion::convert<std::string>(__LINE__));

#define ASSERT_2(func, exception) \
{ auto const l = [&](){func;}; assert<decltype(l), exception> \
(l, std::string(#func) + " doesn't throw " + #exception, iridium::convertion::convert<std::string>(__LINE__)); }

#define ASSERT_1(is_true) \
assert(is_true, std::string(#is_true), iridium::convertion::convert<std::string>(__LINE__));

#define ASSERT(...) \
    dMACRO_CHOOSER(ASSERT, __VA_ARGS__)(__VA_ARGS__)

#define IMPLEMENT_TEST_MAIN() \
int main(int argc, char* argv[]) { \
    return iridium::testing::Tester::instance().run(argc, argv, __FILE__); \
}


#endif // HEADER_TESTER_2DFB0DB0_F2C1_425A_947D_7883FD5FAB64
