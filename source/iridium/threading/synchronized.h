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


template<typename TMutex, bool const is_tracing = false>
class Synchronized {
protected:
    Synchronized() = default;
    virtual ~Synchronized() = default;

    
    void interrupt();
    

    std::shared_ptr<std::unique_lock<TMutex> >
    wait(std::string const &file, std::string const &line,
         std::chrono::nanoseconds const &timeout = std::chrono::nanoseconds(0));


    std::shared_ptr< std::lock_guard<TMutex> >
    lock(std::string const &file, std::string const &line);
    

private:
#ifdef BUILD_TYPE_DEBUG
    struct TLockedPath {
        DEFINE_CREATE(TLockedPath)
        
        TLockedPath(std::string const &line_, Synchronized const &sync);
        
        void log(std::string const &action) const;
        
        std::string const thread_name;
        std::string const mutex_name;
        std::string const line;
    };
    typename TLockedPath::TConstSharedPtr
        m_locked_path;
#endif
    TMutex
        m_mutex;
    std::condition_variable
        m_cv;
};


// methods implementation


template<typename TMutex, bool const is_tracing>
void Synchronized<TMutex, is_tracing>::interrupt() {
    m_cv.notify_all();
}


template<typename TMutex, bool const is_tracing>
std::shared_ptr<std::unique_lock<TMutex> >
Synchronized<TMutex, is_tracing>::wait(
    std::string const &file, std::string const &line,
    std::chrono::nanoseconds const &timeout)
{
#ifdef BUILD_TYPE_DEBUG
    auto locked_path = TLockedPath::create(file + ":" + line, *this);
    locked_path->log("WT");
#endif

    std::unique_lock<TMutex> *l;
    
    try {
        l = new std::unique_lock<TMutex>(m_mutex);
    } catch (std::exception const &e) {
#ifdef BUILD_TYPE_DEBUG
        locked_path->log("ET");
#endif
        throw std::runtime_error(
            std::string("mutex lock error: ") + e.what()
#ifdef BUILD_TYPE_DEBUG
            + ", at\n" + locked_path->line
#endif
        );
    }

    std::shared_ptr< std::unique_lock<TMutex> > locker(
        l,
        [&] (auto locker) {
#ifdef BUILD_TYPE_DEBUG
            if (m_locked_path) {
                m_locked_path->log("UT");
                m_locked_path.reset();
            }
#endif
            delete locker;
        }
    );

    bool is_locked = false;
    
    if (timeout == std::chrono::nanoseconds(0)) {
        m_cv.wait(*locker);
        is_locked = true;
    } else {
        is_locked = m_cv.wait_for(*locker, timeout) == std::cv_status::no_timeout;
    }
    
    if (is_locked) {
#ifdef BUILD_TYPE_DEBUG
        if (m_locked_path)
            throw std::runtime_error("lock mutex error:\ndeadlock\n"
                +   locked_path->line + "\n"
                + m_locked_path->line);
        
        m_locked_path = locked_path;
        m_locked_path->log("LT");
#endif
        return locker;
    }
#ifdef BUILD_TYPE_DEBUG
    // skip locking
    locked_path->log("ST");
#endif
    return nullptr;
}


template<typename TMutex, bool const is_tracing>
std::shared_ptr< std::lock_guard<TMutex> >
Synchronized<TMutex, is_tracing>::lock(
    std::string const &file, std::string const &line)
{
#ifdef BUILD_TYPE_DEBUG
    auto locked_path = TLockedPath::create(file + ":" + line, *this);
    locked_path->log("WM");
#endif
    std::lock_guard<TMutex> *l;
    
    try {
        l = new std::lock_guard<TMutex>(m_mutex);
    } catch (std::exception const &e) {
#ifdef BUILD_TYPE_DEBUG
        locked_path->log("EM");
#endif
        throw std::runtime_error(
            std::string("mutex lock error: ") + e.what()
#ifdef BUILD_TYPE_DEBUG
            + ", at\n" + locked_path->line
#endif
        );
    }

    std::shared_ptr< std::lock_guard<TMutex> > locker(
        l,
        [this] (auto locker) {
#ifdef BUILD_TYPE_DEBUG
            m_locked_path->log("UM");
            m_locked_path.reset();
#endif
            m_cv.notify_one();
            delete locker;
        }
    );
#ifdef BUILD_TYPE_DEBUG
    if (m_locked_path)
        throw std::runtime_error("lock mutex error:\ndeadlock\n"
            +   locked_path->line + "\n"
            + m_locked_path->line);
    m_locked_path = locked_path;
    m_locked_path->log("LM");
#endif
    return locker;
}


#ifdef BUILD_TYPE_DEBUG
template<typename TMutex, bool const is_tracing>
Synchronized<TMutex, is_tracing>::TLockedPath::TLockedPath(std::string const &line_, Synchronized const &sync)
:
    thread_name (IThread::getNameStatic()),
    mutex_name  (8, ' '),
    line        (line_)
{
    std::snprintf(const_cast<char *>(mutex_name.c_str()), mutex_name.size(), "%p", &sync.m_mutex);
}


template<typename TMutex, bool const is_tracing>
void Synchronized<TMutex, is_tracing>::TLockedPath::log(std::string const &action) const {
    if (is_tracing) {
        std::printf("%s %s %s %s\n%s\n",
            mutex_name.c_str(),
            action.c_str(),
            ljust(IThread::getNameStatic(), 32, ' ').c_str(),
            ljust(thread_name, 32, ' ').c_str(),
            line.c_str()
        );
    }
}
#endif


} // threading
} // iridium


#define LOCK_SCOPE() \
auto _____locked_scope_____ = this->lock(__FILE__, std::to_string(__LINE__));


#define LOCK_SCOPE_WAIT(timeout) LOCK_SCOPE_WAIT_ ## timeout
#define LOCK_SCOPE_WAIT_ \
this->wait(__FILE__, std::to_string(__LINE__))
#define LOCK_SCOPE_WAIT_timeout \
this->wait(__FILE__, std::to_string(__LINE__), timeout)


//#define WAIT2(timeout) \
//this->wait(__FILE__, std::to_string(__LINE__), timeout)


//#define LOCK_SCOPE_TIMED(wait_method_name) \
//    LockedScope _____locked_scope_____(this, std::string(__FILE__) + ":" + \
//    iridium::convertion::convert<std::string>(__LINE__), true); \
//    auto wait_method_name = _____locked_scope_____.getWaitingMethodLambda()


#endif // HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
