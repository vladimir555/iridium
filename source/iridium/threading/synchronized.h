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

//    typedef std::chrono::nanoseconds TTimeout;
//
//    void interrupt();
//
//#ifdef BUILD_TYPE_DEBUG
//    struct Path {
//        DEFINE_CREATE(Path)
//
//        Path(Synchronized const &sync,
//             std::string  const &file_path,
//             uint32_t     const &line);
//
//        void log(std::string const &action) const;
//
//        std::string const thread_name;
//        std::string const mutex_name;
//        std::string const file_path_line;
//    };
//    typename Path::TConstSharedPtr mutable
//        m_path;
//#endif
//
//    friend class Locker;
//    class Locker {
//    public:
//        Locker(
//            Synchronized    const &sync,
//            std::string     const &file_path,
//            uint32_t        const &line);
//       ~Locker();
//    private:
//        Synchronized const         &m_sync;
//        std::lock_guard<TMutex>     m_lock;
//    };
//
//
//    friend class Waiter;
//    class Waiter {
//    public:
//        Waiter(
//            Synchronized const &sync,
//            std::string  const &file_path,
//            uint32_t     const &line,
//            TTimeout     const &timeout = TTimeout(0));
//       ~Waiter();
//        bool wait() const;
//
//    private:
//        Synchronized const         &m_sync;
//        std::unique_lock<TMutex>    m_lock;
//        bool                        m_is_locked;
//    };
//
//    std::string const
//        m_mutex_name;

//private:
    TMutex mutable
        m_mutex;
    std::condition_variable mutable
        m_cv;
//    static std::atomic<uint16_t>
//        m_mutex_id;
};


// methods implementation


//template<typename TMutex, bool const is_tracing>
//std::atomic<uint16_t> Synchronized<TMutex, is_tracing>::m_mutex_id(0);


template<typename TMutex, bool const is_tracing>
Synchronized<TMutex, is_tracing>::Synchronized()
//:
//    m_mutex_name("0x" + rjust(convert<std::string>(m_mutex_id++, 16), 4, '0'))
{
//#ifdef BUILD_TYPE_DEBUG
//    if (is_tracing)
//        Path::create(*this, "", 0)->log("create");
////        std::printf  ("%s create,  %s\n",     m_mutex_name.c_str(), IThread::getNameStatic().c_str());
//#endif
}


template<typename TMutex, bool const is_tracing>
Synchronized<TMutex, is_tracing>::~Synchronized() {
//#ifdef BUILD_TYPE_DEBUG
//    if (is_tracing)
//        Path::create(*this, "", 0)->log("destroy");
////        std::printf  ("%s destroy, %s\n", m_mutex_name.c_str(), IThread::getNameStatic().c_str());
//#endif
}


//template<typename TMutex, bool const is_tracing>
//void Synchronized<TMutex, is_tracing>::interrupt() {
//#ifdef BUILD_TYPE_DEBUG
//    if (is_tracing) {
////        if (m_path)
////            m_path->log("interrupt");
//        std::printf("%s IT %s\n", m_mutex_name.c_str(), IThread::getNameStatic().c_str());
////        std::printf("%s notify all ...\n", m_mutex_name.c_str());
//    }
//#endif
//    m_cv.notify_all();
    
    
//#ifdef BUILD_TYPE_DEBUG
//    if (is_tracing)
//        std::printf("%s notify all OK\n" , m_mutex_name.c_str());
//#endif
//}


//#ifdef BUILD_TYPE_DEBUG
//template<typename TMutex, bool const is_tracing>
//Synchronized<TMutex, is_tracing>::Path::Path(
//    Synchronized    const &sync,
//    std::string     const &file_path,
//    uint32_t        const &line_)
//:
//    thread_name     (IThread::getNameStatic()),
//    mutex_name      (sync.m_mutex_name),
//    file_path_line  (file_path + ":" + convert<std::string>(line_))
//{}
//
//
//template<typename TMutex, bool const is_tracing>
//void Synchronized<TMutex, is_tracing>::Path::log(std::string const &action) const {
//    if (is_tracing) {
//        if (file_path_line.size() > 2)
//            std::printf("M:%s T:%s T:%s %s\n%s\n",
//                mutex_name.c_str(),
//                ljust(IThread::getNameStatic(), 32, ' ').c_str(),
//                ljust(thread_name, 32, ' ').c_str(),
//                action.c_str(),
//                file_path_line.c_str());
//        else
//            std::printf("M:%s T:%s T:%s %s\n",
//                mutex_name.c_str(),
//                ljust(IThread::getNameStatic(), 32, ' ').c_str(),
//                ljust(thread_name, 32, ' ').c_str(),
//                action.c_str());
//    }
//}
//#endif


//template<typename TMutex, bool const is_tracing>
//Synchronized<TMutex, is_tracing>::Locker::Locker(
//    Synchronized    const &sync,
//    std::string     const &file_path,
//    uint32_t        const &line)
//:
//    m_sync(sync),
//    m_lock(sync.m_mutex)
//{
//#ifdef BUILD_TYPE_DEBUG
//    auto path = Path::create(sync, file_path, line);
//    if (m_sync.m_path) {
//        path->log("deadlock");
//        m_sync.m_path->log("deadlock");
//        throw std::runtime_error("lock mutex error:\ndeadlock\n"
//            +          path->file_path_line + "\n"
//            + m_sync.m_path->file_path_line);
//    }
//
//    m_sync.m_path = path;
//    m_sync.m_path->log("lock mutex");
//#endif
//}


//template<typename TMutex, bool const is_tracing>
//Synchronized<TMutex, is_tracing>::Locker::~Locker() {
////    printf("%s notify_one ...\n", m_sync.m_mutex_name.c_str());
////    printf("%s notify_one OK\n",  m_sync.m_mutex_name.c_str());
//#ifdef BUILD_TYPE_DEBUG
//    m_sync.m_path->log("unlock mutex");
//    m_sync.m_path.reset();
//#endif
//}


//template<typename TMutex, bool const is_tracing>
//Synchronized<TMutex, is_tracing>::Waiter::Waiter(
//    Synchronized const &sync,
//    std::string  const &file_path,
//    uint32_t     const &line,
//    TTimeout     const &timeout)
//:
//    m_sync      (sync),
//    m_lock      (sync.m_mutex)
//{
//    if (timeout == TTimeout(0)) {
//        m_sync.m_cv.wait(m_lock);
//        m_is_locked = true;
//    } else {
////        printf("%s wait_for ...\n", m_sync.m_mutex_name.c_str());
//        m_is_locked = m_sync.m_cv.wait_for(m_lock, timeout) == std::cv_status::no_timeout;
////        printf("%s wait_for OK\n",  m_sync.m_mutex_name.c_str());
//    }
//#ifdef BUILD_TYPE_DEBUG
//    auto path = Path::create(sync, file_path, line);
//
//    if (m_sync.m_path) {
//        path->log("deadlock");
//        m_sync.m_path->log("deadlock");
//        throw std::runtime_error("lock mutex error:\ndeadlock\n"
//            +          path->file_path_line + "\n"
//            + m_sync.m_path->file_path_line);
//    }
//
//    if (m_is_locked) {
//        m_sync.m_path = path;
//        path->log("lock timed");
//    } else {
//        path->log("lock timeout");
//    }
//#endif
//}


//template<typename TMutex, bool const is_tracing>
//Synchronized<TMutex, is_tracing>::Waiter::~Waiter() {
//#ifdef BUILD_TYPE_DEBUG
//    if (m_sync.m_path) {
//        m_sync.m_path->log("unlock timed");
//        m_sync.m_path.reset();
//    }
//#endif
//    m_is_locked = false;
//}


//template<typename TMutex, bool const is_tracing>
//bool Synchronized<TMutex, is_tracing>::Waiter::wait() const {
//    return m_is_locked;
//}


} // threading
} // iridium



#define LOCK_SCOPE() \
std::lock_guard<std::mutex> l(m_mutex)


// TODO:

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


#endif // HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
