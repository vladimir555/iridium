/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "thread.h"

#include "iridium/convertion/convert.h"
#include "iridium/logging/logger.h"
#include "iridium/assert.h"

#include <iostream>


using std::thread;
using std::string;
using std::cerr;
using std::chrono::milliseconds;
using iridium::convertion::convert;


namespace iridium {
namespace threading {
namespace implementation {


std::chrono::seconds const CThread::DEFAULT_TIMEOUT(10);


CThread::CThread(string const &name, IRunnable::TSharedPtr const &runnuble, std::chrono::nanoseconds const &timeout)
:
    m_name      (name),
    m_runnuble  (assertExists(runnuble, "thread '" + name + "' creation error: runnuble is null")),
    m_statuses  (CAsyncQueue<bool>::create()),
    m_timeout   (timeout)
{}


void CThread::initialize() {
    if (m_thread)
        throw std::runtime_error("thread '" + m_name + "' initializing error: already initialized"); // ----->

    m_runnuble->initialize();
    m_is_running = true;
    m_thread = std::make_unique<thread>(run, m_name, m_runnuble, m_statuses, &m_is_running);
    try {
        auto statuses = m_statuses->pop(m_timeout);
        if (statuses.empty())
            throw std::runtime_error("timeout");
    } catch (std::exception const &e) {
        throw std::runtime_error("thread '" + m_name + "' initializing error: " + e.what()); // ----->
    }
}


void CThread::finalize() {
    if (m_thread && m_thread->joinable()) {
        m_is_running = false;

        try {
            auto statuses = m_statuses->pop(m_timeout);
            if (statuses.empty())
                throw std::runtime_error("timeout");
            else {
                m_thread->join();
                m_runnuble->finalize();
            }
        } catch (std::exception const &e) {
            // todo: pthread_cancel & widows teminate thread
            m_thread->detach();
            throw std::runtime_error("thread '" + m_name + "' finalizing error: " + e.what()); // ----->
        }
        m_thread.reset();
    } else {
        throw std::runtime_error("thread '" + m_name + "' finalizing error: not initialized"); // ----->
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


void CThread::run(
    std::string             const &name,
    IRunnable::TSharedPtr   const &runnuble,
    IAsyncQueuePusher<bool>::TSharedPtr  const &statuses,
    std::atomic<bool> *     const  is_running
) {
    std::string error;
    try {
        statuses->push(true);
        runnuble->run(*is_running);
        statuses->push(false);
    } catch (std::exception &e) {
        error = "thread '" + name + "' stopped, error: " + e.what();
    } catch (...) {
        error = "thread '" + name + "' stopped, error: unknown";
    }
    if (!error.empty()) {
        // for exceptions in logger
//        cerr << error;
        LOGT << error;
    }
}


} // implementation
} // threading
} // iridium
