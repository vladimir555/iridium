#include "thread.h"

#include "utility/convertion/convert.h"
#include "utility/assert.h"

#include "async_queue.h"

#include <chrono>
#include <iostream>


using std::thread;
using std::make_shared;
using std::string;
using std::cerr;
using std::chrono::milliseconds;
using utility::convertion::convert;

auto now = std::chrono::system_clock::now;

static auto const THREAD_START_TIMEOUT_MS = 10000;


namespace utility {
namespace threading {
namespace implementation {


CThread::CThread(
    IRunnable::TSharedPtr           const &runnuble, 
    string                          const &name, 
    IAsyncQueue<bool>::TSharedPtr   const &thread_working_status_queue)
:
    m_name                  (name),
    m_runnuble              (assertExists(runnuble, "runnuble is null")),
    m_runnuble_name         (name),
    m_thread_working_status_queue(thread_working_status_queue)
{}


void CThread::initialize() {
    m_runnuble->initialize();
    if (m_thread_working_status_queue) {
        m_thread = make_shared<thread>(run, m_runnuble, m_thread_working_status_queue);
    } else {
        m_thread_working_status_queue = CAsyncQueue<bool>::create();
        m_thread = make_shared<thread>(run, m_runnuble, m_thread_working_status_queue);
        try {
            auto statuses = m_thread_working_status_queue->pop();
            if (statuses.size() != 1 || !statuses.back())
                throw std::runtime_error("wrong thread start status"); // ----->
        } catch (std::exception const &e) {
            throw std::runtime_error("thread '" + m_name + "' start error: " + e.what()); // ----->
        }
    }
}


void CThread::finalize() {
    if (m_thread && m_thread->joinable()) {
        m_runnuble->stop();
        m_thread->join();
        m_runnuble->finalize();
    }
}


bool CThread::operator < (IThread const &thread) const {
    return getName() < thread.getName(); // ----->
}


std::string CThread::getName() const {
    if (m_thread)
        return m_name + " " + convert<string>(m_thread->get_id()); // ----->
    else
        return m_name;
}


void CThread::run(IRunnable::TSharedPtr const &runnuble, IAsyncQueuePusher<bool>::TSharedPtr const &thread_working_status_queue) {
    try {
        thread_working_status_queue->push(true);
        runnuble->run();
        thread_working_status_queue->push(false);
    } catch (std::exception &e) {
        cerr << "thread error: " << e.what();
    } catch (...) {
        cerr << "thread error: unknown";
    }
}


} // implementation
} // threading
} // utility
