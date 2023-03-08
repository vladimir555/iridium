/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
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


//CCondition::CCondition(IMutex::TSharedPtr const &mutex) {
//    mutex->
//}


void CCondition::wait() const {
    try {
        unique_lock<mutex> lock(m_mutex);
        m_condition.wait(lock);
    } catch (std::exception const &e) {
        throw std::runtime_error("condition variable waiting error: " + string(e.what())); // ----->
    }
}


bool CCondition::wait(std::chrono::nanoseconds const &timeout) const {
    try {
        unique_lock<mutex> lock(m_mutex);
        // todo: maybe need while !m_mutex
        return m_condition.wait_for(lock, timeout) == std::cv_status::no_timeout;
    } catch (std::exception const &e) {
        throw std::runtime_error("condition variable waiting error: " + string(e.what())); // ----->
    }
}


void CCondition::notifyOne() const {
    m_condition.notify_one();
}


void CCondition::notifyAll() const {
    m_condition.notify_all();
}


//void CCondition::lock() const {
//    CMutex::lock();
//}


//void CCondition::unlock() const {
//    CMutex::unlock();
//}


//void CCondition::setScopeName(std::string const &scope_name) const {
//    CMutex::setScopeName(scope_name);
//}


//std::string CCondition::getScopeName() const {
//    return CMutex::getScopeName();
//}


} // implementation
} // threading
} // iridium
