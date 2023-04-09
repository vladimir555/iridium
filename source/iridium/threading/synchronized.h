// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
#define HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD


#include "thread.h"
#include "iridium/pattern/non_copyable.h"
#include "iridium/pattern/non_movable.h"


namespace iridium {
namespace threading {


// ----- class definitions


// todo: SyncFast, SyncTimedFast, SyncDebug, SyncTimedDebug with templates


template<typename TMutex>
class Synchronized {
public:
    Synchronized() = default;
    virtual ~Synchronized() = default;
    
protected:
    class LockedScope:
        private pattern::NonCopyable,
        private pattern::NonMovable
    {
    public:
        LockedScope(Synchronized<TMutex> const * const sync, std::string const &path);
        virtual ~LockedScope() = default;
    protected:
        std::unique_ptr< std::unique_lock<TMutex> > m_lock;
    };
    friend class LockedScope;
private:
    std::string mutable m_locked_path;
    TMutex      mutable m_mutex;
};


template<typename TMutex>
class SynchronizedTimed: public Synchronized<TMutex> {
public:
    SynchronizedTimed() = default;
    virtual ~SynchronizedTimed();
    void interrupt();
protected:
    
    class LockedScope: public Synchronized<TMutex>::LockedScope {
    public:
        LockedScope(SynchronizedTimed<TMutex> const * const sync, std::string const &path);
        LockedScope(SynchronizedTimed<TMutex> const * const sync, std::string const &path, bool);
        virtual ~LockedScope();
        auto getWaitingMethodLambda();
    private:
        SynchronizedTimed const * const m_synchronized_timed;
        bool                            m_is_necessay_notify;
    };
    friend class LockedScope;
private:
    std::condition_variable mutable m_cv;
};


// ----- class method implementations


template<typename TMutex>
Synchronized<TMutex>::LockedScope::LockedScope(Synchronized<TMutex> const * const synchronized, std::string const &path) {
    std::string lock_path = IThread::getNameStatic() + ":" + path;
    try {
        m_lock = std::make_unique< std::unique_lock<TMutex> >(synchronized->m_mutex);
        synchronized->m_locked_path = std::move(lock_path);
    } catch (std::exception const &e) {
        throw std::runtime_error("mutex deadlock '" + synchronized->m_locked_path + "' with '" + lock_path + "': " + e.what()); // ----->
    } catch (...) {
        throw std::runtime_error("mutex deadlock '" + synchronized->m_locked_path + "' with '" + lock_path + "'"); // ----->
    }
}


template<typename TMutex>
SynchronizedTimed<TMutex>::~SynchronizedTimed() {
    interrupt();
}


template<typename TMutex>
void SynchronizedTimed<TMutex>::interrupt() {
    m_cv.notify_all();
}


template<typename TMutex>
SynchronizedTimed<TMutex>::LockedScope::LockedScope(SynchronizedTimed<TMutex> const * const synchronized, std::string const &path)
:
    Synchronized<TMutex>::LockedScope(synchronized, path), m_synchronized_timed(synchronized), m_is_necessay_notify(true)
{}


template<typename TMutex>
SynchronizedTimed<TMutex>::LockedScope::LockedScope(SynchronizedTimed<TMutex> const * const synchronized, std::string const &path, bool)
:
    Synchronized<TMutex>::LockedScope(synchronized, path), m_synchronized_timed(synchronized), m_is_necessay_notify(false)
{}


template<typename TMutex>
SynchronizedTimed<TMutex>::LockedScope::~LockedScope() {
    if (m_is_necessay_notify)
        m_synchronized_timed->m_cv.notify_one();
}


template<typename TMutex>
auto SynchronizedTimed<TMutex>::LockedScope::getWaitingMethodLambda() {
    return [&] (std::chrono::nanoseconds const &timeout = std::chrono::nanoseconds(0)) {
        if (m_is_necessay_notify) {
            return false; // ----->
        } else {
            if (timeout == std::chrono::nanoseconds(0)) {
                m_synchronized_timed->m_cv.wait(*Synchronized<TMutex>::LockedScope::m_lock);
                return true; // ----->
            } else
                return m_synchronized_timed->m_cv.wait_for(*Synchronized<TMutex>::LockedScope::m_lock, timeout) == std::cv_status::no_timeout; // ----->
        }
    };
}


} // threading
} // iridium


#define LOCK_SCOPE() \
    LockedScope _____locked_scope_____(this, __FUNCTION__)


#define LOCK_SCOPE_TIMED(wait_method_name) \
    LockedScope _____locked_scope_____(this, __FUNCTION__, true); \
    auto wait_method_name = _____locked_scope_____.getWaitingMethodLambda()


#endif // HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
