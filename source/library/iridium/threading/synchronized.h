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


namespace iridium::threading {


// ----- class definition


// todo: multi mutex waiting; shared mutex wrapping
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
        bool wait(std::chrono::system_clock::duration const &timeout);
    private:
        Synchronized const * const
            m_s;
        std::unique_lock<TMutex>
            m_l;
        char
            const *m_file = nullptr;
        int
            const  m_line = 0;
    };

protected:
    virtual bool checkWaitingPredicate() const;

private:
    friend class Locker;

    TMutex
        mutable m_mutex;
    std::condition_variable
        mutable m_cv;
    std::atomic<int>
        mutable m_waiters_count { 0 };
    std::atomic<bool>
        mutable m_is_interrupted { false };
};


// class implementation


template<typename TMutex, bool const is_tracable>
void Synchronized<TMutex, is_tracable>::interrupt() {
    m_is_interrupted.store(true, std::memory_order_relaxed);
    m_cv.notify_all();
}


template<typename TMutex, bool const is_tracable>
bool Synchronized<TMutex, is_tracable>::checkWaitingPredicate() const {
    return true; // ----->
}


template<typename TMutex, bool const is_tracable>
Synchronized<TMutex, is_tracable>::Locker::Locker(
    Synchronized const * const s,
    char const *file, int line)
:
    m_s(s),
    m_l(s->m_mutex)
{
    if constexpr (is_tracable) {
        m_file = file;
        m_line = line;
        printf("%s LM\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            file, line);
    }
}


template<typename TMutex, bool const is_tracable>
Synchronized<TMutex, is_tracable>::Locker::~Locker() {
    if constexpr (is_tracable) {
        printf("%s UM\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            m_file, m_line);
    }

    m_l.unlock();
    if (m_s->m_waiters_count.load(std::memory_order_acquire) > 0)
        m_s->m_cv.notify_one();
}


template<typename TMutex, bool const is_tracable>
bool Synchronized<TMutex, is_tracable>::Locker::wait() {
    m_s->m_waiters_count.fetch_add(1, std::memory_order_release);

    m_s->m_cv.wait(
        m_l,
        [this] () {
            return m_s->m_is_interrupted.load(std::memory_order_relaxed)
                || m_s->checkWaitingPredicate();
        }
    );

    m_s->m_waiters_count.fetch_sub(1, std::memory_order_relaxed);

    if constexpr (is_tracable) {
        printf("%s WM -> %s\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            m_s->m_is_interrupted ? "interrupted" : "OK",
            m_file, m_line);
    }

    return true; // ----->
}


template<typename TMutex, bool const is_tracable>
bool Synchronized<TMutex, is_tracable>::Locker::wait(
    std::chrono::system_clock::duration
        const &timeout)
{
    if (m_s->m_is_interrupted.load(std::memory_order_relaxed))
        return false; // ----->

    m_s->m_waiters_count.fetch_add(1, std::memory_order_release);

    bool result = m_s->m_cv.wait_for(m_l,
        timeout,
        [this] () {
            return m_s->m_is_interrupted.load(std::memory_order_relaxed)
                || m_s->checkWaitingPredicate();
        }
    );

    m_s->m_waiters_count.fetch_sub(1, std::memory_order_relaxed);

    if constexpr (is_tracable) {
        printf("%s WM -> %s\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            result ? "OK" : m_s->m_is_interrupted ? "interrupted" : "timeout",
            m_file, m_line);
    }

    return result; // ----->
}


} // namespace iridium::threading


#define LOCK_SCOPE() \
Synchronized::Locker _____locked_scope_##__LINE__(this, __FILE__, __LINE__)


#define LOCK_SCOPE_TRY_WAIT(...) \
_____locked_scope_##__LINE__.wait(__VA_ARGS__)


#endif // HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
