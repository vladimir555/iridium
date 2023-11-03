// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
#define HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD


#include "thread.h"
#include "iridium/pattern/non_copyable.h"
#include "iridium/pattern/non_movable.h"
#include "iridium/convertion/convert.h"
#include "iridium/strings.h"
#include "iridium/platform.h"

#include <mutex>
#include <condition_variable>
#include <iostream>
#include <atomic>
#include <cstdio>


using iridium::convertion::convert;


namespace iridium {
namespace threading {


// ----- class definitions


// TODO: mutex cv tracing with log level
// todo: log level instead is_tracing
template<typename TMutex, bool const is_tracing = false>
class Synchronized {
protected:
    Synchronized();
    virtual ~Synchronized();

//private:
    TMutex mutable
        m_mutex;
    std::condition_variable mutable
        m_cv;
};


// methods implementation


template<typename TMutex, bool const is_tracing>
Synchronized<TMutex, is_tracing>::Synchronized() {}


template<typename TMutex, bool const is_tracing>
Synchronized<TMutex, is_tracing>::~Synchronized() {}


} // threading
} // iridium



#define LOCK_SCOPE() \
std::lock_guard<std::mutex> l(m_mutex)


// TODO:
/*
//#define LOCK_SCOPE() \
//Locker _____locked_scope_____(*this, __FILE__, __LINE__)


//#define LOCK_SCOPE_WAIT_0() \
//Waiter _____locked_scope_____(*this, __FILE__, __LINE__); \
//if (_____locked_scope_____.wait())
//
//
//#define LOCK_SCOPE_WAIT_1(timeout) \
//Waiter _____locked_scope_____(*this, __FILE__, __LINE__, timeout); \
//if (_____locked_scope_____.wait())
//
//
//#define LOCK_SCOPE_WAIT_X(x,A,LOCK_SCOPE_WAIT_FUNC, ...) \
//LOCK_SCOPE_WAIT_FUNC
//
//
//#define LOCK_SCOPE_WAIT(...) \
//LOCK_SCOPE_WAIT_X(,##__VA_ARGS__, \
//LOCK_SCOPE_WAIT_1(__VA_ARGS__), \
//LOCK_SCOPE_WAIT_0(__VA_ARGS__) \
//)
*/

#endif // HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
