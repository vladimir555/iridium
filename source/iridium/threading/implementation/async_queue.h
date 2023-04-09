// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
#define HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258


#include <list>

#include "iridium/smart_ptr.h"
#include "iridium/pattern/non_copyable.h"

#include "iridium/threading/thread.h"
#include "iridium/threading/async_queue.h"
#include "iridium/convertion/convert.h"
#include "iridium/logging/logger.h"


#include "condition.h"
#include <iostream>


#define SYNC_SCOPE() \
    Scope _____locked_scope_____(this, __FUNCTION__);

#define SYNC_SCOPE_TIMED(wait_method_name) \
    Scope _____locked_scope_____(this, __FUNCTION__, true); \
    auto wait_method_name = _____locked_scope_____.getWaitingMethodLambda();


namespace iridium {
namespace threading {
namespace implementation {


// ----- interface

// todo: SyncFast, SyncTimedFast

template<typename TMutex>
class Sync {
public:
    Sync() = default;
    virtual ~Sync() = default;
    
protected:
    friend class SyncScope;
    class Scope {
    public:
        Scope(Sync<TMutex> const * const sync, std::string const &path) {
            std::string lock_path = IThread::getNameStatic() + ":" + path;
            try {
                m_lock = std::make_unique< std::unique_lock<TMutex> >(sync->m_mutex);
                sync->m_locked_path = std::move(lock_path);
            } catch (std::exception const &e) {
                throw std::runtime_error("mutex deadlock '" + sync->m_locked_path + "' with '" + lock_path + "': " + e.what()); // ----->
            } catch (...) {
                throw std::runtime_error("mutex deadlock '" + sync->m_locked_path + "' with '" + lock_path + "'"); // ----->
            }
        }
        virtual ~Scope() = default;
    protected:
        std::unique_ptr< std::unique_lock<TMutex> > m_lock;
    };

private:
    std::string mutable m_locked_path;
    TMutex      mutable m_mutex;
};


template<typename TMutex>
class SyncTimed: public Sync<TMutex> {
public:
    SyncTimed() = default;
    virtual ~SyncTimed() {
        interrupt();
    }
    void interrupt() {
        m_cv.notify_all();
    }
protected:
    friend class Scope;
    class Scope: public Sync<TMutex>::Scope {
    public:
        Scope(SyncTimed<TMutex> const * const sync, std::string const &path)
        : Sync<TMutex>::Scope(sync, path), sync(sync), do_notify(true)
        {}
        Scope(SyncTimed<TMutex> const * const sync, std::string const &path, bool)
        : Sync<TMutex>::Scope(sync, path), sync(sync), do_notify(false)
        {}
        virtual ~Scope() {
            if (do_notify)
                sync->m_cv.notify_one();
        }
        auto getWaitingMethodLambda() {
            return [&] (std::chrono::nanoseconds const &timeout = std::chrono::nanoseconds(0)) {
                if (do_notify) {
                    return false; // ----->
                } else {
                    if (timeout == std::chrono::nanoseconds(0)) {
                        sync->m_cv.wait(*Sync<TMutex>::Scope::m_lock);
                        return true; // ----->
                    } else
                        return sync->m_cv.wait_for(*Sync<TMutex>::Scope::m_lock, timeout) == std::cv_status::no_timeout; // ----->
                }
            };
        }
    private:
        SyncTimed const * const sync;
        bool do_notify;
    };
private:
    std::condition_variable mutable m_cv;
};


template<typename TItem>
class CAsyncQueue:
    public  IAsyncQueue<TItem>,
    public  pattern::NonCopyable,
    private SyncTimed<std::mutex>
{
public:
    DEFINE_IMPLEMENTATION(CAsyncQueue)
    ///
    CAsyncQueue();
    ///
    size_t push(TItem const &item) override;
    ///
    size_t push(std::list<TItem> const &items) override;
    ///
    std::list<TItem> pop(bool const &is_waiting = true) override;
    ///
    std::list<TItem> pop(std::chrono::nanoseconds const &timeout) override;
    ///
    void interrupt() override;
    ///
    bool empty() const override;
private:
    std::list<TItem>        m_items;
    std::atomic<bool>       m_is_waiting;
    std::atomic<bool>       m_is_empty;
    
//    ITrigger::TSharedPtr    m_trigger;
//    std::mutex              m_mutex;
//    std::mutex              m_mutex_condition;
//    std::condition_variable m_condition;
};


// ----- implementation


template<typename TItem>
CAsyncQueue<TItem>::CAsyncQueue()
:
    m_is_waiting(true),
    m_is_empty  (true)
{}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(TItem const &item) {
    {
        size_t size = 0;
        {
//            Scope _____locked_scope_____(this, __FUNCTION__);
//            SYNC_SCOPE()
            SYNC_SCOPE()
            
            m_items.push_back(item);
            m_is_empty = m_items.empty();
            size = m_items.size();
        }
        
//        m_trigger->notify();
        
        return size;
    }
    
//    {
//        size_t size = 0;
//        {
//            std::unique_lock<std::mutex> l(m_mutex);
//
//            m_items.push_back(item);
//            m_is_empty = m_items.empty();
//            size = m_items.size();
//        }
//
//        {
//            std::unique_lock<std::mutex> l(m_mutex_condition);
//            m_condition.notify_one();
//
//            return size; // ----->
//        }
//    }

//    {
//        std::unique_lock<std::mutex> l(m_mutex);
//
//        m_items.push_back(item);
//        m_is_empty = m_items.empty();
//        auto size  = m_items.size();
//
//        l.unlock();
//        m_condition.notify_one();
//
//        return size; // ----->
//    }
}


template<typename TItem>
size_t CAsyncQueue<TItem>::push(std::list<TItem> const &items) {
    {
//        Scope _____locked_scope_____(this, __FUNCTION__);
        SYNC_SCOPE();
        
        m_items.insert(m_items.end(), items.begin(), items.end());
        m_is_empty = m_items.empty();
        return m_items.size();
    }

//    {
//        size_t size = 0;
//        // synchronized
//        {
//            std::unique_lock<std::mutex> l(m_mutex);
//
//            m_items.insert(m_items.end(), items.begin(), items.end());
//            m_is_empty = m_items.empty();
//            size = m_items.size();
//        }
//
//        // notify
//        {
//            std::unique_lock<std::mutex> l(m_mutex_condition);
//            m_condition.notify_one();
//
//            return size; // ----->
//        }
//    }

//    {
//        std::unique_lock<std::mutex> l(m_mutex);
//
//        m_items.insert(m_items.end(), items.begin(), items.end());
//        m_is_empty  = m_items.empty();
//        auto size   = m_items.size();
//
//        l.unlock();
//        m_condition.notify_one();
//
//        return size; // ----->
//    }
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop(bool const &is_waiting) {
    {
//        Scope _____locked_scope_____(this, __FUNCTION__, true);
//        auto wait = _____locked_scope_____.getWaitingMethodLambda();
        SYNC_SCOPE_TIMED(wait);
        
        if (m_is_waiting && m_is_empty && is_waiting)
            wait();
        
        m_is_empty = true;
        return std::move(m_items); // ----->
    }
    
//    {
//        {
//            std::unique_lock<std::mutex> l(m_mutex_condition);
//            while (m_is_waiting && m_is_empty && is_waiting)
//                m_condition.wait(l);
//        }
//
//        {
//            std::unique_lock<std::mutex> l(m_mutex);
//            m_is_empty = true;
//            return std::move(m_items); // ----->
//        }
//    }

//    {
//        std::unique_lock<std::mutex> l(m_mutex);
//        while (m_is_waiting && m_is_empty && is_waiting)
//            m_condition.wait(l);
//
//        m_is_empty = true;
//        return std::move(m_items); // ----->
//    }
}


template<typename TItem>
std::list<TItem> CAsyncQueue<TItem>::pop(std::chrono::nanoseconds const &timeout) {
    {
//        Scope _____locked_scope_____(this, __FUNCTION__, true);
//        auto wait = _____locked_scope_____.getWaitingMethodLambda();
        SYNC_SCOPE_TIMED(wait);
        
        if (m_is_waiting && m_is_empty)
            wait(timeout);
        
        m_is_empty = true;
        return std::move(m_items); // ----->
    }

//    {
//        if (m_is_waiting && m_is_empty) {
//            std::unique_lock<std::mutex> l(m_mutex_condition);
//            m_condition.wait(l);
//        }
//
//        {
//            std::unique_lock<std::mutex> l(m_mutex);
//            m_is_empty = true;
//            return std::move(m_items); // ----->
//        }
//    }

//    {
//        std::unique_lock<std::mutex> l(m_mutex);
//
//        if (m_is_waiting && m_is_empty)
//            m_condition.wait_for(l, timeout);
//
//        m_is_empty = true;
//
//        return std::move(m_items); // ----->
//    }
}


template<typename TItem>
void CAsyncQueue<TItem>::interrupt() {
    {
        m_is_waiting = false;
        SyncTimed::interrupt();
    }

//    {
//        m_is_waiting = false;
//        m_condition.notify_all();
//    }
}


template<typename TItem>
bool CAsyncQueue<TItem>::empty() const {
    return m_is_empty; // ----->
}


} // implementation
} // threading
} // iridium


#endif // HEADER_ASYNC_QUEUE_A0382C4D_41C2_4C4F_BC6D_DD295F054258
