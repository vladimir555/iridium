// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CACHED_VALUE_589D8C09_61B8_4639_9F01_493EC80EE2D8
#define HEADER_CACHED_VALUE_589D8C09_61B8_4639_9F01_493EC80EE2D8


#include <memory>
#include <string>
#include <map>


#include "iridium/threading/implementation/mutex.h"


namespace iridium {
namespace caching {


// ----- interface


// todo: init static mutex for any translation units
template<typename TValue>
class CachedValue {
public:
    ///
    CachedValue();
    ///
    template<typename TParam>
    CachedValue(TParam const &value_);
    ///
    ~CachedValue() = default;
    ///
    template<typename TParam>
    CachedValue &operator = (TParam const &param);
    ///
    operator TValue() const;
    ///
    bool operator < (CachedValue const &cached_value) const;

private:
    ///
    static std::map<CachedValue, std::shared_ptr<TValue> > m;
    ///
    static threading::IMutex::TConstSharedPtr m_mutex;
    ///
    mutable std::shared_ptr<TValue> m_value;
};


// ----- implementation


template<typename TValue>
std::map<CachedValue<TValue>, std::shared_ptr<TValue> > CachedValue<TValue>::m = std::map<CachedValue<TValue>, std::shared_ptr<TValue> >();


template<typename TValue>
threading::IMutex::TConstSharedPtr CachedValue<TValue>::m_mutex = std::make_shared<threading::implementation::CMutex>();


template<typename TValue>
CachedValue<TValue>::CachedValue() {
    m_value = std::make_shared<TValue>();
}


template<typename TValue>
template<typename TParam>
CachedValue<TValue>::CachedValue(TParam const &value) {
    m_value = std::make_shared<TValue>(value);
    m_mutex->lock();
    m_value = m[*this];
    if (!m_value) {
        m_value     = std::make_shared<TValue>(value);
        m[*this]    = m_value;
    }
    m_mutex->unlock();
}


template<typename TValue>
template<typename TParam>
CachedValue<TValue> &CachedValue<TValue>::operator = (TParam const &param) {
    CachedValue cached_value(param);
    m_value = cached_value.m_value;
    return *this;
}


template<typename TValue>
CachedValue<TValue>::operator TValue() const {
    return *m_value;
}


template<typename TValue>
bool CachedValue<TValue>::operator < (CachedValue const &cached_value) const {
    return m_value && cached_value.m_value && *m_value < *cached_value.m_value; // ----->
}


typedef CachedValue<std::string> CachedString;


} // caching
} // iridium


#endif // HEADER_CACHED_VALUE_589D8C09_61B8_4639_9F01_493EC80EE2D8
