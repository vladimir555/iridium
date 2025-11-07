// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SYNCHRONIZED_CONTAINER_239086AF_AB7B_403B_A1B5_7D5CB97DA8E6
#define HEADER_SYNCHRONIZED_CONTAINER_239086AF_AB7B_403B_A1B5_7D5CB97DA8E6


#include <unordered_map>
#include <mutex>
#include "iridium/threading/synchronized.h"


namespace iridium {
namespace threading {


/// \~english @brief A thread-safe container based on std::unordered_map.
/// \~russian @brief Потокобезопасный контейнер на основе std::unordered_map.
template<typename TKey, typename TValue>
class SynchronizedContainer: public Synchronized<std::mutex> {
public:
    DEFINE_IMPLEMENTATION(SynchronizedContainer)
    /// \~english @brief Constructor.
    /// \~russian @brief Конструктор.
    SynchronizedContainer();

    /// \~english @brief Sets a key-value pair in the container.
    /// \~russian @brief Устанавливает пару "ключ-значение" в контейнере.
    size_t  set(TKey const &key, TValue const &value);
    /// \~english @brief Gets a value by key from the container.
    /// \~russian @brief Возвращает значение по ключу из контейнера.
    TValue  get(TKey const &key) const;
    /// \~english @brief Deletes a key-value pair from the container.
    /// \~russian @brief Удаляет пару "ключ-значение" из контейнера.
    size_t  del(TKey const &key);
    
private:
    /// \~english @brief The underlying unordered_map.
    /// \~russian @brief Базовый unordered_map.
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

    
} // threading
} // iridium


#endif // HEADER_SYNCHRONIZED_CONTAINER_239086AF_AB7B_403B_A1B5_7D5CB97DA8E6
