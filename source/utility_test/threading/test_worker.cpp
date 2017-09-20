#include <gtest/gtest.h>
#include <iostream>
#include <atomic>

#include <utility/threading/thread.h>
#include <utility/threading/implementation/worker.h>
#include <utility/threading/implementation/worker_pool.h>
#include <utility/threading/implementation/condition.h>
#include <utility/convertion/convert.h>


using namespace std;
using utility::convertion::convert;
using utility::threading::implementation::CWorker;
using utility::threading::implementation::CWorkerPool;
using utility::threading::implementation::CCondition;
using utility::threading::IThread;
using utility::threading::IWorker;
using utility::threading::IWorkerPool;
using utility::threading::IWorkerHandler;
using utility::threading::sleep;
using utility::threading::ICondition;


namespace {


atomic<int>     processed;
list<int>       in;
list<int>       out;


//list<string> in_;
//list<string> out_;
//ICondition::TSharedPtr  condition = CCondition::create();


class Worker: public IWorkerHandler<int> {
public:
    DEFINE_CREATE(Worker)
    Worker() = default;
    virtual ~Worker() = default;
private:
    TItems handleItems(TItems const &items) override {
        for (auto const &i: items) {
//            cout << "processing " << i << endl;
            out.push_back(i);
            processed++;
            sleep(10);
        }
        sleep(100);
        return TItems();
    }

    void handleStart() override {
    }

    void handleStop() override {
//        cout << "stop" << endl;
    }
};


}


namespace utility {
namespace networking {


TEST(threading, worker) {
    IWorker<int>::TSharedPtr worker = CWorker<int>::create("worker", Worker::create());

    processed = 0;
    in.clear();
    out.clear();

    for (int i = 0; i < 50; i++)
        in.push_back(i);

    worker->initialize();

    for (auto const &i: in) {
//        cout << "push " << i << endl;
        worker->push(i);
    }

    for (int i = 0; i < 10 && processed < 50; i++)
        sleep(100);
    worker->finalize();
    ASSERT_EQ(in, out);
    sleep(100);
    worker->push(5);
    ASSERT_EQ(in, out);
}


TEST(threading, worker_pool) {
    list<IWorkerHandler<int>::TSharedPtr > l;

    for (size_t i = 0; i < 10; i++)
        l.push_back(Worker::create());

    IWorkerPool<int>::TSharedPtr worker_pool = CWorkerPool<int>::create(l, "worker_pool");

    processed = 0;
    in.clear();
    out.clear();

    for (int i = 0; i < 50; i++)
        in.push_back(i);

    worker_pool->initialize();

    for (auto const &i: in) {
        worker_pool->push(i);
        sleep(10);
    }

    for (int i = 0; i < 10 && processed < 50; i++)
        sleep(100);
    worker_pool->finalize();
    out.sort();
    ASSERT_EQ(in, out);
}


} // networking
} // utility
