#include <gtest/gtest.h>
#include <iostream>


using namespace std;


namespace utility {
namespace pattern {


// socket
enum TEvent {
    EVENT_READ,
    EVENT_WRITE
};


enum TState {
    STATE_ERROR,
    STATE_DONE,
    STATE_PARSE,
    STATE_NEED_READ,
    STATE_NEED_WRITE
};


/*
    STATE / EVENT   | READ  | WRITE | PARSE
    ---------------------------------------
    ERROR           | END   | END   | READ
    DONE            | PARSE | END   | WRITE
    NEED_READ       | PARSE |
    NEED_WRITE      | PARSE
*/


TEST(pattern, fsm) {

}


} // pattern
} // utility
