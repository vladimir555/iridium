#include "thread.h"

#include "utility/convertion/convert.h"

#include <iostream>
#include <chrono>


using std::thread;
using std::make_shared;
using std::string;
using std::cerr;
using std::chrono::milliseconds;

using utility::convertion::convert;

auto now = std::chrono::high_resolution_clock::now;


static auto const THREAD_START_TIMEOUT = milliseconds(10000);


namespace utility {
namespace threading {
namespace implementation {


CThread::CThread(IRunnable::TSharedPtr const &runnuble, string const &name)
:
    m_name          (name),
    m_runnuble      (runnuble),
    m_runnuble_name (name)
{}


void CThread::initialize() {
    m_thread = make_shared<thread>(run, m_runnuble, &m_is_thread_running);
    auto timeout_time_point = now() + THREAD_START_TIMEOUT;
    while(!m_is_thread_running) {
        if  (timeout_time_point < now())
            throw std::runtime_error("thread '" + m_name + "' start error: timeout " +
                convert<string>(THREAD_START_TIMEOUT)); // ----->
    }
}


void CThread::finalize() {
    if (m_thread && m_thread->joinable()) {
        m_runnuble->stop();
        m_thread->join();
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


void CThread::run(IRunnable::TSharedPtr const &runnuble, std::atomic<bool> *is_thread_running) {
    try {
        *is_thread_running = true;
        runnuble->run();
    } catch (std::exception &e) {
        cerr << "thread error: " << e.what();
    } catch (...) {
        cerr << "thread error: unknown";
    }
}


} // implementation
} // threading
} // utility
