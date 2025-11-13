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


namespace iridium::threading {


// ----- class definition


template<typename TMutex, bool const is_tracable = false>
class Synchronized {
protected:
    Synchronized() = default;
    virtual ~Synchronized() = default;

    void interrupt();

    class Locker:
        public pattern::NonCopyable,
        public pattern::NonMovable
    {
    public:
        Locker(Synchronized const * const, char const *, int);
       ~Locker();

        bool wait();
        bool wait(std::chrono::nanoseconds const &timeout);
    private:
        Synchronized const * const
            m_s;
        std::unique_lock<TMutex>
            m_l;
        char const *
            m_file = nullptr;
        int m_line = 0;
    };
private:
    friend class Locker;

    TMutex mutable
        m_mutex;
    std::condition_variable mutable
        m_cv;
    std::atomic<bool> mutable
        m_is_waitable = true;
};


// class implementation


template<typename TMutex, bool const is_tracable>
void Synchronized<TMutex, is_tracable>::interrupt() {
    m_is_waitable = false;
    m_cv.notify_all();
}


template<typename TMutex, bool const is_tracable>
Synchronized<TMutex, is_tracable>::Locker::Locker(
    Synchronized const * const s,
    char const *file, int line)
:
    m_s         (s),
    m_l         (s->m_mutex)
{
    if (is_tracable) {
        m_file = file;
        m_line = line;
        printf("%s LM\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            file, line);
    }
}


template<typename TMutex, bool const is_tracable>
Synchronized<TMutex, is_tracable>::Locker::~Locker() {
    if (is_tracable)
        printf("%s UM\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            m_file, m_line);

    m_l.unlock();
    m_s->m_cv.notify_one();
}


template<typename TMutex, bool const is_tracable>
bool Synchronized<TMutex, is_tracable>::Locker::wait() {
    if (m_s->m_is_waitable)
        m_s->m_cv.wait(m_l);

    if (is_tracable)
        printf("%s WM -> %s\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            m_s->m_is_waitable ? "OK" : "interrupted",
            m_file, m_line);

    return m_s->m_is_waitable; // ----->
}


template<typename TMutex, bool const is_tracable>
bool Synchronized<TMutex, is_tracable>::Locker::wait(std::chrono::nanoseconds const &timeout) {
    bool result =
        m_s->m_is_waitable &&
        m_s->m_cv.wait_for(m_l, timeout) == std::cv_status::no_timeout;
    if (is_tracable)
        printf("%s WM -> %s\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            result ? "OK" : m_s->m_is_waitable ? "timeout" : "interrupted",
            m_file, m_line);
    return result; // ----->
}


} // namespace iridium::threading


#define LOCK_SCOPE() \
Synchronized::Locker _____locked_scope_____(this, __FILE__, __LINE__)


#define LOCK_SCOPE_TRY_WAIT(timeout) \
_____locked_scope_____.wait(timeout)


#endif // HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
