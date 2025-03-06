#include <iridium/testing/tester.h>
#include <iostream>
#include <atomic>

#include <iridium/threading/thread.h>
#include <iridium/threading/implementation/worker.h>
#include <iridium/threading/implementation/worker_pool.h>
#include <iridium/convertion/convert.h>
#include <iridium/items.h>


using iridium::convertion::convert;
using iridium::threading::implementation::CWorker;
using iridium::threading::implementation::CWorkerPool;
using iridium::threading::implementation::CAsyncQueue;
using iridium::threading::IThread;
using iridium::threading::IWorker;
using iridium::threading::IAsyncQueue;
using iridium::threading::sleep;


namespace {


std::atomic<int> processed  (0);
std::atomic<int> sum        (0);


class CWorkerHandler: public IWorker<int, int>::IHandler {
public:
    DEFINE_IMPLEMENTATION(CWorkerHandler)
    CWorkerHandler() = default;
private:
    typedef typename IWorker<int>::TInputItems  TInputItems;
    typedef typename IWorker<int>::TOutputItems TOutputItems;

    TOutputItems handle(TInputItems const &items) override {
        TInputItems result;
        for (auto const &i: items) {
            processed++;
            sum += i;
            result.push_back(sum);
            sleep(10);
        }
        return result;
    }

    void initialize() override {}
    void finalize()   override {}
};


}


namespace iridium {
namespace threading {


TEST(worker) {
    IWorker<int>::TSharedPtr worker = CWorker<int>::create("worker", CWorkerHandler::create());

    processed           = 0;
    sum                 = 0;
    int const count     = 100;
    int const count_sum = 4950;

    worker->initialize();

    for (int i = 0; i < count; i++) {
//        std::printf("worker->push, i = %i ...", i);
//        LOGT << "push i = " << i;
        worker->push(i);
    }

    for (int i = 0; i < count && processed < count; i++) {
//        LOGT << "processed = " << i;
        sleep(100);
    }

    worker->finalize();

//    LOGT << processed << " equal " << count;
    ASSERT(processed, equal, count);
    ASSERT(sum, equal, count_sum);

    worker->push(5);

    ASSERT(processed, equal, count);
    ASSERT(sum, equal, count_sum);
}


TEST(worker_pool) {
    IWorker<int>::TSharedPtr worker = CWorkerPool<int>::create("worker",
        createObjects<IWorker<int>::IHandler, CWorkerHandler>(std::thread::hardware_concurrency()));

    // global vars
    processed = 0;
    sum = 0;

    int const count = 100;
    int const count_sum = 4950;

    worker->initialize();

    for (int i = 0; i < count; i++)
        worker->push(i);

//    for (int i = 0; i < 10 && processed < count; i++)
//        sleep(100);
    std::list<int> result;
//    LOGT << "start";
    while (result.size() < count) {
        auto items = worker->pop(std::chrono::seconds(5));
        if (items.empty())
            break; // --->
        result.splice(result.end(), items);
    }
//    LOGT << "stop";

    worker->finalize();

    ASSERT(processed, equal     , result.size());
    ASSERT(processed, greater   , 0);
    ASSERT(sum      , equal     , result.back());

    ASSERT(processed, equal     , count);
    ASSERT(sum      , equal     , count_sum);

    worker->push(5);

    ASSERT(processed, equal     , count);
    ASSERT(sum      , equal     , count_sum);
}


} // net
} // iridium
