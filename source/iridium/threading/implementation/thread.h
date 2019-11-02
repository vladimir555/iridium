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


namespace iridium {
namespace threading {
namespace implementation {


class CThread: 
    public IThread, 
    public pattern::NonCopyable
{
public:
    DEFINE_CREATE(CThread)
    ///
    CThread(
        IRunnable::TSharedPtr           const &runnuble, 
        std::string                     const &name, 
        IAsyncQueue<bool>::TSharedPtr   const &thread_working_status_queue = nullptr);
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
    std::shared_ptr<std::thread>    m_thread;
    ///
    IRunnable::TSharedPtr           m_runnuble;
    ///
    std::string                     m_runnuble_name;
    ///
    static void run(
        IRunnable::TSharedPtr const &runnuble,
        IAsyncQueuePusher<bool>::TSharedPtr const &thread_working_status_queue,
        std::atomic<bool> * const is_running
    );
private:
    ///
    IAsyncQueue<bool>::TSharedPtr   m_thread_working_status_queue;
    std::atomic<bool>               m_is_running;
};


} // implementation
} // threading
} // iridium


#endif // HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA
