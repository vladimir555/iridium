/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "synchronized_scope.h"

#include <stdexcept>
#include <thread>
#include <iostream>

#include "iridium/convertion/convert.h"
#include "iridium/logging/logger.h"
#include "iridium/threading/thread.h"
#include "iridium/assert.h"


using std::string;
using std::exception;
using std::runtime_error;
using std::stringstream;
using iridium::convertion::convert;


namespace iridium {
namespace threading {


SynchronizedScope::SynchronizedScope(Synchronized const * const synchronized, string const &scope_name)
:
    m_mutex(synchronized->getMutex())
{
    string const scope_name_ = scope_name + ":" + convert<string>(std::this_thread::get_id());
    try {
        m_mutex->lock();
        m_mutex->setScopeName(scope_name_);
    } catch (exception const &e) {
        throw runtime_error("mutex deadlock " + m_mutex->getScopeName() + " with " + scope_name_ + "': " + e.what()); // ----->
    }
}


SynchronizedScope::~SynchronizedScope() {
    m_mutex->unlock();
    m_mutex->setScopeName("");
}


SynchronizedScopeSimple::SynchronizedScopeSimple(Synchronized const * const synchronized)
:
    m_mutex(synchronized->getMutex())
{
    m_mutex->lock();
}


SynchronizedScopeSimple::~SynchronizedScopeSimple() {
    m_mutex->unlock();
}


} // threading
} // iridium
