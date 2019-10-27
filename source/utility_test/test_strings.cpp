#include <gtest/gtest.h>

#include <utility/strings.h>

#include <iostream>
using namespace std;

namespace utility {


TEST(strings, trim) {
    ASSERT_EQ(""        , trim(" "));
    ASSERT_EQ(""        , trim("     "));
    ASSERT_EQ("A"       , trim("A"));
    ASSERT_EQ("a"       , trim(" a"));
    ASSERT_EQ("abcde"   , trim("     \t\r\n\tabcde     \t\n\r"));
}


TEST(strings, split) {
    ASSERT_EQ( list<string>( { "1", "22", "333", "4444", "5" } ), split("1 22  333   4444    5  ", " ") );
}
    
    
} // utility
