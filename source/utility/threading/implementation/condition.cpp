#include "condition.h"

#include <memory>
#include <chrono>

#include "utility/convertion/convert.h"


using std::mutex;
using std::unique_lock;
using std::string;
using std::chrono::seconds;
using utility::convertion::convert;


namespace utility {
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


void CCondition::wait(int const &timeout_sec) const {
    try {
        unique_lock<mutex> lock(m_mutex);
        m_condition.wait_for(lock, seconds(timeout_sec));
    } catch (std::exception const &e) {
        throw std::runtime_error("condition variable waiting error, timeout " + convert<string>(timeout_sec) + " sec: " + e.what()); // ----->
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
} // utility
