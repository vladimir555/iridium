#ifndef HEADER_PARALLEL_32399357_4743_4072_9BE1_2E3B22B7E9EE
#define HEADER_PARALLEL_32399357_4743_4072_9BE1_2E3B22B7E9EE


// todo:
/*
 * example:
 * thread pool for:
 * 
 * int  f1(int const &);
 * void f2(int const &);
 *
 * async( f2( async( f1(1) ) + async( f2(2) ) ) ); // f1 runs parallel with f2
 *
 * waiting on operator<TValue>() const
 *
 * ==
 *
 * f2( f1(1) + f2(2) );
 *
 * for (...) async(same_func());
 *
 */


#include "thread.h"
#include "runnable.h"
#include "worker_pool.h"


namespace iridium {
namespace threading {


// todo: worker with jobs
template<typename TReturn, typename ... TAgrs>
class Async {
public:
    Async(TArgs &&args ...);
   ~Async() = default;
    TReturn &get() const;
    operator TReturn () const;
private:
    class CRunnable: public IRunnable {
    public:
        void run(std::atomic<bool> &is_running) override;
    };
};


class Parallel {
public:
    Parallel();
    // todo:
//    auto makeAsyncFunc() {
//        return [this](){f();};
//    }

private:
    IWorkerPool<>::TSharedPtr m_worker_pool;
};

    
} // threading
} // iridium


#endif // HEADER_PARALLEL_32399357_4743_4072_9BE1_2E3B22B7E9EE
