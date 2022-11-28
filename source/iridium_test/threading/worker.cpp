/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include <iridium/testing/tester.h>
#include <iostream>
#include <atomic>

#include <iridium/threading/thread.h>
#include <iridium/threading/implementation/worker.h>
#include <iridium/threading/implementation/worker_pool.h>
#include <iridium/threading/implementation/recursive_mutex.h>
#include <iridium/convertion/convert.h>


using namespace std;
using iridium::convertion::convert;
using iridium::threading::implementation::CWorker;
using iridium::threading::implementation::CWorkerPool;
using iridium::threading::implementation::CRecursiveMutex;
using iridium::threading::IThread;
using iridium::threading::IWorker;
using iridium::threading::sleep;
using iridium::threading::IMutex;


namespace {


atomic<int>     processed;
list<int>       in;
list<int>       out;


//list<string> in_;
//list<string> out_;
//ICondition::TSharedPtr  condition = CCondition::create();
IMutex::TSharedPtr m = CRecursiveMutex::create();


class CWorkerHandler: public IWorker<int, int>::IHandler {
public:
    DEFINE_CREATE(CWorkerHandler)
    CWorkerHandler() = default;
    virtual ~CWorkerHandler() = default;
private:
    typedef typename IWorker<int>::TInputItems  TInputItems;
    typedef typename IWorker<int>::TOutputItems TOutputItems;

    TInputItems handle(TOutputItems const &items) override {
        for (auto const &i: items) {
            //cout << "processing " << i << endl;
            out.push_back(i);
            processed++;
            sleep(10);
        }
        return {};
    }

    void initialize() override {
        m->lock();
        //cout << "start" << endl;
    }

    void finalize() override {
        m->unlock();
        //cout << "stop" << endl;
    }
};


}


namespace iridium {
namespace threading {


TEST(worker) {
    IWorker<int>::TSharedPtr worker = CWorker<int>::create("worker", CWorkerHandler::create());

    processed = 0;
    in.clear();
    out.clear();

    for (int i = 0; i < 50; i++)
        in.push_back(i);

    worker->initialize();

    for (auto const &i : in) {
        //cout << "push " << i << endl;
        worker->push(i);
    }

    // todo: waiting for single worker
    for (int i = 0; i < 10 && processed < 50; i++)
        sleep(100);

    worker->finalize();

    ASSERT(in, equal, out);

    worker->push(5);

    //m->lock();

    worker->finalize();

    m->lock();
    ASSERT(in, equal, out);
    m->unlock();
    // todo: test for queue size = 0
}


TEST(worker_pool) {
    sleep(500);

    IAsyncQueue<int>::TSharedPtr q = implementation::CAsyncQueue<int>::create();
    q->push(5);
    q->push(55);
    auto i = q->pop();

    ASSERT(2    , equal, i.size());
    ASSERT(5    , equal, i.front());
    ASSERT(55   , equal, i.back());

    CWorkerPool<int>::THandlers handlers;

    for (size_t i = 0; i < 10; i++)
        handlers.push_back(CWorkerHandler::create());

    IWorker<int>::TSharedPtr worker_pool = CWorkerPool<int>::create("worker_pool", handlers);

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

    //for (int i = 0; i < 10 && processed < 50; i++)
    //    sleep(100);

    //m->lock();

    worker_pool->finalize();

    m->lock();

    out.sort();

    ASSERT(in, equal, out);
    m->unlock();
}


} // net
} // iridium
