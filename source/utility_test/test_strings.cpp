#include <gtest/gtest.h>

#include <utility/strings.h>


namespace utility {


TEST(strings, trim) {
    ASSERT_EQ(""        , trim(" "));
    ASSERT_EQ(""        , trim("     "));
    ASSERT_EQ("A"       , trim("A"));
    ASSERT_EQ("a"       , trim(" a"));
    ASSERT_EQ("abcde"   , trim("     \t\r\n\tabcde     \t\n\r"));
}


} // utility
