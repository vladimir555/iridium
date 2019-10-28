#include "condition.h"

#include <memory>
#include <chrono>

#include "iridium/convertion/convert.h"


using std::mutex;
using std::unique_lock;
using std::string;
using std::chrono::milliseconds;
using iridium::convertion::convert;


namespace iridium {
namespace threading {
namespace implementation {


void CCondition::wait() const {
    try {
        unique_lock<mutex> lock(m_mutex);
        m_condition.wait(lock);
    } catch (std::exception const &e) {
        throw std::runtime_error("condition variable waiting error: " + string(e.what())); // ----->
    }
}


void CCondition::wait(int const &timeout_ms) const {
    try {
        unique_lock<mutex> lock(m_mutex);
        // todo: maybe need while !m_mutex
        m_condition.wait_for(lock, milliseconds(timeout_ms));
    } catch (std::exception const &e) {
        throw std::runtime_error("condition variable waiting error, timeout " + convert<string>(timeout_ms) + " ms: " + e.what()); // ----->
    }
}


void CCondition::notifyOne() const {
    m_condition.notify_one();
}


void CCondition::notifyAll() const {
    m_condition.notify_all();
}


} // implementation
} // threading
} // iridium
