#ifndef HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA
#define HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA


#include "utility/smart_ptr.h"
#include "utility/threading/thread.h"
#include "utility/threading/runnable.h"
#include "utility/pattern/non_copyable.h"

#include "condition.h"

#include <thread>
#include <atomic>
#include <string>


namespace utility {
namespace threading {
namespace implementation {


class CThread: 
    public IThread, 
    public pattern::NonCopyable
{
public:
    DEFINE_CREATE(CThread)
    ///
    CThread(IRunnable::TSharedPtr const &runnuble, std::string const &name);
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
    static void run(IRunnable::TSharedPtr const &runnuble, ICondition::TSharedPtr is_started_condition/*std::atomic<bool> *is_thread_running*/);
private:
    ///
    ICondition::TSharedPtr          m_is_started_condition;
};


} // implementation
} // threading
} // utility


#endif // HEADER_THREAD_4164C6F4_DDC5_41FF_B701_C80679412DFA
