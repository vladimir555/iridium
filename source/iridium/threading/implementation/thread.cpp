#include "thread.h"

#include "iridium/convertion/convert.h"
#include "iridium/logging/logger.h"
#include "iridium/assert.h"

#include <iostream>

#include "iridium/logging/logger.h"
#include "iridium/threading/implementation/async_queue.h"


using std::thread;
using std::string;
using std::cerr;
using std::chrono::milliseconds;
using iridium::convertion::convert;


namespace iridium {
namespace threading {
namespace implementation {


std::chrono::seconds const CThread::DEFAULT_TIMEOUT(30);


CThread::CThread(string const &name, IRunnable::TSharedPtr const &runnuble, std::chrono::nanoseconds const &timeout)
:
    m_name              (name),
    m_runnuble          (assertExists(runnuble, "thread '" + name + "' creation error: runnuble is null")),
    m_is_running        (false),
    m_error_queue_start (CAsyncQueue<string>::create()),
    m_error_queue_stop  (CAsyncQueue<string>::create()),
    m_timeout           (timeout)
{}


CThread::~CThread() {
    if (m_is_running) {
        std::cerr << "FATAL: destroying thread '" << m_name << "' without finalization" << std::endl;
        finalize();
    }
}


void CThread::initialize() {
    if (m_thread)
        throw std::runtime_error("thread '" + m_name + "' initializing error: already initialized"); // ----->

    m_runnuble->initialize();
    m_is_running = true;
    m_thread = std::make_shared<thread>(run, m_name, m_runnuble, m_error_queue_start, m_error_queue_stop, &m_is_running);

    string error;

    try {
        error = checkErrorQueue(m_error_queue_start);
    } catch (std::exception const &e) {
        throw std::runtime_error("thread '" + m_name + "' initializing error: " + e.what()); // ----->
    }

    if (!error.empty() || !m_thread->joinable())
        throw std::runtime_error("thread '" + m_name + "' initializing error: " + error); // ----->
}


void CThread::finalize() {
    if (!m_thread || !m_thread->joinable())
        throw std::runtime_error("thread '" + m_name + "' finalization error: not initialized"); // ----->

    m_is_running = false;

    string error;
    try {
        error = checkErrorQueue(m_error_queue_stop);

        m_thread->join();
        m_thread.reset();
        m_runnuble->finalize();
    } catch (std::exception const &e) {
        // todo: teminate thread, auto unlock all mutexes before thread canceling
        // pthread_cancel, pthread_cleanup_push, pthread_cleanup_pop
        // TerminateThread, DeleteCriticalSection
        if (m_thread) {
            m_thread->detach();
            m_thread.reset();
        }
        error += e.what();
    }

    if (!error.empty())
        throw std::runtime_error("thread '" + m_name + "' finalization error: " + error); // ----->
}


bool CThread::operator < (IThread const &thread) const {
    return getName() < thread.getName(); // ----->
}


std::string CThread::getName() const {
    if (m_thread)
        return m_name + " " + convert<string>(m_thread->get_id()); // ----->
    else
        return m_name; // ----->
}


void CThread::run(
    std::string             const &name,
    IRunnable::TSharedPtr   const &runnuble,
    IAsyncQueuePusher<string>::TSharedPtr const &status_start,
    IAsyncQueuePusher<string>::TSharedPtr const &status_stop,
    std::atomic<bool> *     const  is_running) 
{
    std::string error;
    bool        is_started = true;

    IThread::setNameStatic(name);
    
    try {
        status_start->push("");
        runnuble->run(*is_running);
        is_started = false;
        status_stop->push("");
    } catch (std::exception &e) {
        error = "thread '" + IThread::getNameStatic() + "' stopped by error: " + e.what();
    } catch (...) {
        error = "thread '" + IThread::getNameStatic() + "' stopped by error: unknown";
    }
    if (!error.empty()) {
        if (is_started)
            status_stop->push(error);
        else
            status_start->push(error);
        LOGF << error;
    }
}


string CThread::checkErrorQueue(IAsyncQueuePopper<std::string>::TSharedPtr const &error_queue) {
    auto errors = error_queue->pop(m_timeout);

    if (errors.empty())
        throw std::runtime_error("timeout: " + convert<string>(m_timeout)); // ----->

    string error_message;

    for (auto const &error: errors)
        if (!error.empty())
            error_message += error + "; ";

    return error_message; // ----->
}


} // implementation
} // threading
} // iridium
