#include "worker_pool.h"



namespace iridium {
namespace threading {
namespace implementation {


WorkerPoolBase::WorkerPoolBase(std::string const &name)
:
    m_thread_working_status_queue   (CAsyncQueue<bool>::create()),
    m_name                          (name)
{}


void WorkerPoolBase::waitForMultipleStatus(bool const &status) {
    // todo: wait timeout
    try {
        size_t count = 0;
        while (count < m_threads.size()) {
            for (auto const &s : m_thread_working_status_queue->pop())
                if (status == s)
                    count++;
        }
    } catch (std::exception const &e) {
        throw std::runtime_error("worker pool '" + m_name + "' status waiting error: " + e.what()); // ----->
    }
}


} // implementation
} // threading
} // iridium
