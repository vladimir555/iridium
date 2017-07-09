#include "synchronized_scope.h"

#include <stdexcept>
#include <thread>
#include <sstream>

#include "utility/convertion/convert.h"
#include "utility/logging/logger.h"
#include "utility/threading/thread.h"


using std::string;
using std::exception;
using std::runtime_error;
using std::stringstream;
using utility::convertion::convert;


namespace utility {
namespace threading {


SynchronizedScope::SynchronizedScope(IMutex *mutex, string const &scope_name)
:
    m_mutex(mutex)
{
    string scope_name_ = scope_name + ":" + convert<string>(getThreadID());
    try {
        m_mutex->lock();
        m_mutex->m_scope_name = std::move(scope_name_);
    } catch (exception const &e) {
        throw runtime_error("mutex deadlock " + m_mutex->m_scope_name + " with " + scope_name_ + "': " + e.what()); // ----->
    }
}


SynchronizedScope::~SynchronizedScope() {
    m_mutex->unlock();
}


SynchronizedScopeSimple::SynchronizedScopeSimple(IMutex *mutex)
:
    m_mutex(mutex) 
{
    m_mutex->lock();
}


SynchronizedScopeSimple::~SynchronizedScopeSimple() {
    m_mutex->unlock();
}


} // threading
} // utility
