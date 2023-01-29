// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA
#define HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA


#include "iridium/smart_ptr.h"
#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"
#include "iridium/pattern/non_copyable.h"

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
    public pattern::NonCopyable
{
public:
    DEFINE_CREATE(CThread)
    static std::chrono::seconds const DEFAULT_TIMEOUT;
    ///
    CThread(std::string const &name, IRunnable::TSharedPtr const &runnuble, std::chrono::nanoseconds const &timeout = DEFAULT_TIMEOUT);
    ///
    virtual ~CThread() = default;
    ///
    virtual void initialize() override;
    ///
    virtual void finalize() override;
    ///
    virtual bool operator < (IThread const &thread) const override;
    ///
    virtual std::string getName() const override;

protected:
    ///
    std::string                     m_name;
    ///
    IRunnable::TSharedPtr           m_runnuble;
    ///
    std::unique_ptr<std::thread>    m_thread;
    ///
    static void run(
        std::string             const &name,
        IRunnable::TSharedPtr   const &runnuble,
        IAsyncQueuePusher<bool>::TSharedPtr const &statuses,
        std::atomic<bool> *     const  is_running
    );
private:
    std::atomic<bool>
        m_is_running;
    IAsyncQueue<bool>::TSharedPtr
        m_statuses;
    std::chrono::nanoseconds const
        m_timeout;
};


} // implementation
} // threading
} // iridium


#endif // HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA
