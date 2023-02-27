#include "worker_pool.h"


// todo: make arg
//static std::chrono::minutes DEFAULT_TIMEOUT(1);


#include <iostream>
namespace iridium {
namespace threading {
namespace implementation {


WorkerPoolBase::WorkerPoolBase(std::string const &name)
:
    m_name(name)
{}


//void WorkerPoolBase::waitForMultipleStatus(bool const &status) {
//    static auto const now = std::chrono::system_clock::now;
//    auto const timeout = DEFAULT_TIMEOUT;

//    try {
//        size_t  count   = 0;
//        auto    end     = now() + timeout;

//        while (count < m_threads.size() && now() < end) {
//            for (auto const &s : m_thread_working_status_queue->pop(timeout))
//                if (status == s)
//                    count++;
//        }

//        if (count < m_threads.size())
//            throw std::runtime_error("timeout" );

//    } catch (std::exception const &e) {
//        throw std::runtime_error("worker pool '" + m_name +
//            "' status '" + convertion::convert<std::string>(status) +
//            "' waiting error: " + e.what()); // ----->
//    }
//}


} // implementation
} // threading
} // iridium
