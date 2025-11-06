// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SINGLETON_33ED8344_7855_4F54_A108_7FF9BC7D71B4
#define HEADER_SINGLETON_33ED8344_7855_4F54_A108_7FF9BC7D71B4


#include "non_copyable.h"
#include "non_movable.h"

#include <memory>
#include <list>
#include <atomic>


namespace iridium {
namespace pattern {


template<typename TClass>
class Singleton:
    public NonCopyable,
    public NonMovable
{
public:
    ///
    virtual ~Singleton();
    ///
    static TClass &instance();
protected:
    static std::atomic<bool> m_is_alive;
};


template<typename TClass>
TClass &Singleton<TClass>::instance() {
    static std::shared_ptr<TClass> instance;

    if (!instance) {
        instance = std::shared_ptr<TClass>(new TClass());
        m_is_alive = true;
    }

    return *instance; // ----->
}


template<typename TClass>
Singleton<TClass>::~Singleton() {
    m_is_alive = false;
}


template<typename TClass>
std::atomic<bool> Singleton<TClass>::m_is_alive(false);


} // pattern
} // iridium


#endif // HEADER_SINGLETON_33ED8344_7855_4F54_A108_7FF9BC7D71B4
