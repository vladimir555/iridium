#include "thread.h"

#include "utility/convertion/convert.h"

#include <iostream>

using std::thread;
using std::make_shared;
using std::string;
using std::cerr;

using utility::convertion::convert;


namespace utility {
namespace threading {
namespace implementation {


CThread::CThread(IRunnable::TSharedPtr runnuble, string const &name)
:
    m_name          (name),
    m_runnuble      (runnuble),
    m_runnuble_name (name)
{}


void CThread::initialize() {
    m_thread = make_shared<thread>(run, m_runnuble);
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


void CThread::run(IRunnable::TSharedPtr m_runnuble) {
    try {
        m_runnuble->run();
    } catch (std::exception &e) {
        cerr << "thread error: " << e.what();
    } catch (...) {
        cerr << "thread error: unknown";
    }
}


} // implementation
} // threading
} // utility
