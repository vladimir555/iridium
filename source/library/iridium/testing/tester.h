// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_TESTER_2DFB0DB0_F2C1_425A_947D_7883FD5FAB64
#define HEADER_TESTER_2DFB0DB0_F2C1_425A_947D_7883FD5FAB64


#include "iridium/dp/singleton.h"
#include "iridium/parsing/node_type.h"
#include "iridium/smart_ptr.h"
#include "iridium/logging/logger.h"
#include "iridium/convertion/convert.h"
#include "iridium/macros/va_args.h"

#include <string>
#include <map>

#include "test.h"
#include "unit_test_case.h"


namespace iridium::testing {


class Tester final: public dp::Singleton<Tester> {
public:
    Tester() = default;
    virtual ~Tester() = default;

    typedef parsing::INodeType<IUnitTestCase *> IUnitTestCaseNode;

    void add(
        IUnitTestCase
          * const  test,
        std::string
            const &path,
        size_t
            const &line,
        std::string
            const &name);

    int run(int argc, char* argv[], std::string const &main_cpp_path);

private:
    friend class dp::Singleton<Tester>;

    struct TEntry {
        std::string
            path;
        size_t
            line;
        std::string
            name;

        bool operator < (TEntry const &o) const {
            return std::tie(path, line, name) < std::tie(o.path, o.line, o.name);
        }
    };

    typedef std::map< TEntry, IUnitTestCase * >
        TMapEntryUnitTestCase;

    IUnitTestCaseNode::TSharedPtr getTestTree(
        std::string
            const &testing_source_directory_path,
        std::string
            const &include = {},
        std::list<std::string>
            const &exclude = {}
    ) const;

    TMapEntryUnitTestCase
        m_map_path_test;
};


} // namespace iridium::testing


// #define CONCAT_IMPL( x, y ) x##y
// #define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )


#define TEST(name) \
class UnitTestCase_##name final: \
    public ::iridium::testing::IUnitTestCase, \
    public ::iridium::testing::UnitTestCase { \
public: \
    DEFINE_IMPLEMENTATION(UnitTestCase_##name) \
    UnitTestCase_##name() { iridium::testing::Tester::instance().add(this, std::string(__FILE__), __LINE__, #name); } \
    void run() override; \
private: \
    size_t getLine() const override { return __LINE__; }; \
} UnitTestCase_##name; \
void UnitTestCase_##name::run()

#define ASSERT_3(left, condition, right) \
condition(left, right, std::string(#left) + " " + #condition + " " + #right, __LINE__);

#define ASSERT_2(func, exception) \
{ auto const l = [&](){func;}; assert_<decltype(l), exception> \
(l, std::string(#func) + " doesn't throw " + #exception, __LINE__); }

#define ASSERT_1(is_true) \
assert_(is_true, std::string(#is_true), __LINE__);

#define ASSERT(...) \
    DEFINE_MACRO_CHOOSER(ASSERT, __VA_ARGS__)(__VA_ARGS__)

#define IMPLEMENT_TEST_MAIN() \
int main(int argc, char* argv[]) { \
    std::printf("main thread: %s\n", iridium::threading::IThread::getNameStatic().c_str()); \
    return iridium::testing::Tester::instance().run(argc, argv, std::string(__FILE__)); \
}


#endif // HEADER_TESTER_2DFB0DB0_F2C1_425A_947D_7883FD5FAB64
