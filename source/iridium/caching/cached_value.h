// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CACHED_VALUE_589D8C09_61B8_4639_9F01_493EC80EE2D8
#define HEADER_CACHED_VALUE_589D8C09_61B8_4639_9F01_493EC80EE2D8


#include <memory>
#include <string>
#include <unordered_map>


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
    bool operator == (CachedValue const &cached_value) const;
    ///
    bool operator <  (CachedValue const &cached_value) const;

private:
    //friend struct std::equal_to< iridium::caching::CachedValue<TValue> >;
    friend struct std::hash    < iridium::caching::CachedValue<TValue> >;
    ///
    static thread_local std::unordered_map<CachedValue, std::shared_ptr<TValue> > m;
    ///
    mutable std::shared_ptr<TValue> m_value;
};


// ----- implementation


template<typename TValue>
thread_local std::unordered_map<CachedValue<TValue>, std::shared_ptr<TValue> > CachedValue<TValue>::m = 
std::unordered_map<CachedValue<TValue>, std::shared_ptr<TValue> >();


template<typename TValue>
CachedValue<TValue>::CachedValue() {
    m_value = std::make_shared<TValue>();
}


template<typename TValue>
template<typename TParam>
CachedValue<TValue>::CachedValue(TParam const &value) {
    m_value = std::make_shared<TValue>(value);
    m_value = m[*this];
    if(!m_value) {
        m_value     = std::make_shared<TValue>(value);
        m[*this]    = m_value;
    }
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
bool CachedValue<TValue>::operator == (CachedValue const &cached_value) const {
    return m_value && cached_value.m_value && *m_value == *cached_value.m_value; // ----->
}


template<typename TValue>
bool CachedValue<TValue>::operator <  (CachedValue const &cached_value) const {
    return m_value && cached_value.m_value && *m_value < *cached_value.m_value; // ----->
}


typedef CachedValue<std::string> CachedString;


} // caching
} // iridium


//template<typename TValue>
//struct std::equal_to<iridium::caching::CachedValue<TValue> > {
//    std::size_t operator()(iridium::caching::CachedValue<TValue> const &k) const {
//        return std::hash<TValue>()(*k.m_value);
//    }
//};


template<typename TValue>
struct std::hash<iridium::caching::CachedValue<TValue> > {
    std::size_t operator()(iridium::caching::CachedValue<TValue> const &k) const {
        return std::hash<TValue>()(*k.m_value);
    }
};


#endif // HEADER_CACHED_VALUE_589D8C09_61B8_4639_9F01_493EC80EE2D8
