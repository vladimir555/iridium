// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SYNCHRONIZED_CONTAINER_239086AF_AB7B_403B_A1B5_7D5CB97DA8E6
#define HEADER_SYNCHRONIZED_CONTAINER_239086AF_AB7B_403B_A1B5_7D5CB97DA8E6


#include <unordered_map>
#include <mutex>
#include "iridium/threading/synchronized.h"


namespace iridium::threading {


template<typename TKey, typename TValue>
class SynchronizedContainer: public Synchronized<std::mutex> {
public:
    DEFINE_IMPLEMENTATION(SynchronizedContainer)
    SynchronizedContainer();

    size_t  set(TKey const &key, TValue const &value);
    TValue  get(TKey const &key) const;
    size_t  del(TKey const &key);

private:
    std::unordered_map<TKey, TValue> m_map_key_value;
};


template<typename TKey, typename TValue>
SynchronizedContainer<TKey, TValue>::SynchronizedContainer()
:
    Synchronized(implementation::CRecursiveMutex::create())
{}


template<typename TKey, typename TValue>
size_t SynchronizedContainer<TKey, TValue>::set(TKey const &key, TValue const &value) {
    LOCK_SCOPE()
    m_map_key_value[key] = value;
    return m_map_key_value.size(); // ----->
}


template<typename TKey, typename TValue>
TValue SynchronizedContainer<TKey, TValue>::get(TKey const &key) const {
    LOCK_SCOPE()
    auto i  = m_map_key_value.find(key);
    if  (i == m_map_key_value.end())
        return {}; // ----->
    else
        return i->second; // ----->
}


template<typename TKey, typename TValue>
size_t SynchronizedContainer<TKey, TValue>::del(TKey const &key) {
    LOCK_SCOPE()
    m_map_key_value.erase(key);
    return m_map_key_value.size(); // ----->
}


} // namespace iridium::threading


#endif // HEADER_SYNCHRONIZED_CONTAINER_239086AF_AB7B_403B_A1B5_7D5CB97DA8E6
