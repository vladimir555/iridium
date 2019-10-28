#ifndef HEADER_SYNCHRONIZED_CONTAINER_239086AF_AB7B_403B_A1B5_7D5CB97DA8E6
#define HEADER_SYNCHRONIZED_CONTAINER_239086AF_AB7B_403B_A1B5_7D5CB97DA8E6


#include <unordered_map>
#include "iridium/smart_ptr.h"
#include "synchronized_scope.h"
#include "implementation/recursive_mutex.h"


namespace iridium {
namespace threading {


template<typename TKey, typename TValue>
class SynchronizedContainer: public Synchronized {
public:
    DEFINE_CREATE(SynchronizedContainer)
    SynchronizedContainer();
   ~SynchronizedContainer() = default;

    void    set(TKey const &key, TValue const &value);
    TValue  get(TKey const &key) const;
    void    del(TKey const &key);
    
private:
    std::unordered_map<TKey, TValue> m_map_key_value;
};

    
template<typename TKey, typename TValue>
SynchronizedContainer<TKey, TValue>::SynchronizedContainer()
:
    Synchronized(implementation::CRecursiveMutex::create())
{}
    
    
template<typename TKey, typename TValue>
void SynchronizedContainer<TKey, TValue>::set(TKey const &key, TValue const &value) {
    LOCK_SCOPE_FAST
    m_map_key_value[key] = value;
}

    
template<typename TKey, typename TValue>
TValue SynchronizedContainer<TKey, TValue>::get(TKey const &key) const {
    LOCK_SCOPE_FAST
    auto i  = m_map_key_value.find(key);
    if  (i == m_map_key_value.end())
        return {}; // ----->
    else
        return i->second; // ----->
}

    
template<typename TKey, typename TValue>
void SynchronizedContainer<TKey, TValue>::del(TKey const &key) {
    LOCK_SCOPE_FAST
    m_map_key_value.erase(key); // ----->
}

    
} // threading
} // iridium


#endif // HEADER_SYNCHRONIZED_CONTAINER_239086AF_AB7B_403B_A1B5_7D5CB97DA8E6
