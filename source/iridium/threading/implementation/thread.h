// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA
#define HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA


#include "iridium/smart_ptr.h"
#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"
#include "iridium/pattern/non_copyable.h"
#include "iridium/pattern/non_movable.h"

#include "async_queue.h"

#include <thread>
#include <atomic>
#include <string>
#include <chrono>


namespace iridium {
namespace threading {
namespace implementation {


class CThread: 
    public IThread,
    public pattern::NonCopyable,
    public pattern::NonMovable
{
public:
    DEFINE_CREATE(CThread)
    static std::chrono::seconds const DEFAULT_TIMEOUT;
    ///
    CThread(
        std::string const &name,
        IRunnable::TSharedPtr const &runnuble,
        std::chrono::nanoseconds const &timeout = DEFAULT_TIMEOUT);
    ///
    virtual ~CThread();
    ///
    void initialize() override;
    ///
    void finalize() override;
    ///
    bool operator < (IThread const &thread) const override;
    ///
    std::string getName() const override;

protected:
    ///
    static void run(
        std::string             const &name,
        IRunnable::TSharedPtr   const &runnuble,
        IAsyncQueuePusher<std::string>::TSharedPtr const &status_start,
        IAsyncQueuePusher<std::string>::TSharedPtr const &status_stop,
        std::atomic<bool> *     const  is_running
    );
    ///
    std::string const               m_name;
    ///
    IRunnable::TSharedPtr           m_runnuble;
    ///
    std::shared_ptr<std::thread>    m_thread;
private:
    std::string checkErrorQueue(IAsyncQueuePopper<std::string>::TSharedPtr const &error_queue);

    std::atomic<bool>
        m_is_running;
    IAsyncQueue<std::string>::TSharedPtr
        m_error_queue_start;
    IAsyncQueue<std::string>::TSharedPtr
        m_error_queue_stop;
    std::chrono::nanoseconds const
        m_timeout;
};


} // implementation
} // threading
} // iridium


#endif // HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA
