#include <gtest/gtest.h>
#include <iostream>

#include <utility/threading/thread.h>
#include <utility/threading/implementation/worker.h>
#include <utility/threading/implementation/worker_pool.h>
#include <utility/convertion/convert.h>


using namespace std;
using utility::convertion::convert;
using utility::threading::implementation::CWorker;
using utility::threading::implementation::CWorkerPool;
using utility::threading::IThread;
using utility::threading::IWorker;
using utility::threading::IWorkerPool;
using utility::threading::IWorkerHandler;
using utility::threading::sleep;


namespace {


list<int> in;
list<int> out;


list<string> in_;
list<string> out_;


class Worker: public IWorkerHandler<int> {
public:
    DEFINE_CREATE(Worker)
    Worker() = default;
    virtual ~Worker() = default;
private:
    void handleItems(TItems const &items) override {
        for (auto const &i: items) {
//            cout << "thread: " << utility::threading::getThreadID() << " handle: " << i << endl;
            out.push_back(i);
            sleep(10);
        }
    }

    void handleStart() override {
//        cout << "thread: " << utility::threading::getThreadID() << " start" << endl;
    }

    void handleStop() override {
//        cout << "thread: " << utility::threading::getThreadID() << " stop" << endl;
    }
};


}


namespace utility {
namespace networking {


TEST(threading, worker) {
    IWorker<int>::TSharedPtr worker = CWorker<int>::create("worker", Worker::create());

    in.clear();
    out.clear();

    for (int i = 0; i < 50; i++)
        in.push_back(i);

    worker->initialize();

    for (auto const i: in)
        worker->push(i);

    sleep(200);
    worker->finalize();
    ASSERT_EQ(in, out);
}


TEST(threading, worker_pool) {
    list<IWorkerHandler<int>::TSharedPtr > l;

    for (size_t i = 0; i < 10; i++)
        l.push_back(Worker::create());

    IWorkerPool<int>::TSharedPtr worker_pool = CWorkerPool<int>::create(l, "worker_pool");

    in.clear();
    out.clear();

    for (int i = 0; i < 50; i++)
        in.push_back(i);

    worker_pool->initialize();

    for (auto const i: in) {
        worker_pool->push(i);
        sleep(10);
    }

    sleep(100);
    worker_pool->finalize();
    out.sort();
    ASSERT_EQ(in, out);
}


} // networking
} // utility
