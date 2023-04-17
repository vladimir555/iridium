// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SINGLETON_33ED8344_7855_4F54_A108_7FF9BC7D71B4
#define HEADER_SINGLETON_33ED8344_7855_4F54_A108_7FF9BC7D71B4


#include "non_copyable.h"
#include "non_movable.h"


namespace iridium {
namespace pattern {


class Instance {
protected:
    static void *m_instance;
};


///
/// \brief The Meyers singleton class, thread safe
///
template<typename TClass>
class Singleton:
    public Instance,
    public NonCopyable,
    public NonMovable
{
public:
    virtual ~Singleton();
    ///
    static TClass &instance();
};


template<typename TClass>
TClass &Singleton<TClass>::instance() {
    static TClass * const instance = reinterpret_cast<TClass * const>(m_instance = new TClass());
    return *instance; // ----->
}


template<typename TClass>
Singleton<TClass>::~Singleton() {
    delete reinterpret_cast<TClass *>(m_instance);
}


} // pattern
} // iridium


#endif // HEADER_SINGLETON_33ED8344_7855_4F54_A108_7FF9BC7D71B4
