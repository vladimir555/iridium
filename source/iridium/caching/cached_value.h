// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `CachedValue<TValue>` template class for interning and caching values.
///     This header provides mechanisms to reduce memory usage by sharing identical immutable values.
///     It also includes a `CachedString` typedef and related `std::hash` specializations.
/// \~russian @file
/// @brief Определяет шаблонный класс `CachedValue<TValue>` для интернирования и кэширования значений.
///     Этот заголовочный файл предоставляет механизмы для уменьшения использования памяти путем совместного использования идентичных неизменяемых значений.
///     Также включает typedef `CachedString` и связанные специализации `std::hash`.

#ifndef HEADER_CACHED_VALUE_589D8C09_61B8_4639_9F01_493EC80EE2D8
#define HEADER_CACHED_VALUE_589D8C09_61B8_4639_9F01_493EC80EE2D8


#include <memory>
#include <string>
#include <unordered_map>
#include <iridium/convertion/convert.h>


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
/// \~english @brief Contains classes and utilities related to caching and value interning.
/// \~russian @brief Содержит классы и утилиты, связанные с кэшированием и интернированием значений.
namespace caching {


// ----- interface


// todo: remove CachedValue class due memory control problems
// todo: init static mutex for any translation units
/// \~english @brief Represents a cached, potentially shared value of type TValue.
///     This class aims to reduce memory usage by interning identical values.
///     Each unique value is stored once per thread and shared via std::shared_ptr.
///     Note: There are TODO comments in the source regarding potential memory control problems
///     and thread safety for the caching map initialization across translation units.
/// \~russian @brief Представляет кэшированное, потенциально разделяемое значение типа TValue.
///     Этот класс предназначен для уменьшения использования памяти путем интернирования одинаковых значений.
///     Каждое уникальное значение хранится один раз для каждого потока и разделяется через std::shared_ptr.
///     Примечание: В исходном коде есть комментарии TODO относительно потенциальных проблем с контролем памяти
///     и потокобезопасностью инициализации карты кэширования между единицами трансляции.
template<typename TValue>
class CachedValue {
public:
    /// \~english @brief Default constructor. Initializes the cached value to a default-constructed TValue.
    ///     The default-constructed TValue is then potentially interned or retrieved from the thread-local cache.
    /// \~russian @brief Конструктор по умолчанию. Инициализирует кэшированное значение значением TValue, созданным по умолчанию.
    ///     Созданное по умолчанию значение TValue затем потенциально интернируется или извлекается из кэша потока.
    CachedValue();

    /// \~english @brief Constructor that initializes the cached value with a given parameter.
    ///     The provided value is used to construct a TValue, which is then interned or retrieved from the thread-local cache.
    /// \~russian @brief Конструктор, инициализирующий кэшированное значение заданным параметром.
    ///     Предоставленное значение используется для конструирования TValue, которое затем интернируется или извлекается из кэша потока.
    /// \~english @param value_ The initial value to be cached.
    /// \~russian @param value_ Начальное значение для кэширования.
    template<typename TParam>
    CachedValue(TParam const &value_);

    /// \~english @brief Default destructor. Manages the lifetime of the shared cached value.
    /// \~russian @brief Деструктор по умолчанию. Управляет временем жизни разделяемого кэшированного значения.
    ~CachedValue() = default;

    /// \~english @brief Assignment operator. Assigns a new value to the CachedValue.
    ///     The new value is obtained by constructing a temporary CachedValue from the parameter,
    ///     effectively interning the new value or finding it in the cache, and then sharing its underlying pointer.
    /// \~russian @brief Оператор присваивания. Присваивает новое значение объекту CachedValue.
    ///     Новое значение получается путем создания временного CachedValue из параметра,
    ///     что приводит к интернированию нового значения или его нахождению в кэше, после чего разделяется его внутренний указатель.
    /// \~english @param param The parameter to construct the new value from.
    /// \~russian @param param Параметр для конструирования нового значения.
    /// \~english @return A reference to this CachedValue instance with the new value.
    /// \~russian @return Ссылка на этот экземпляр CachedValue с новым значением.
    template<typename TParam>
    CachedValue &operator = (TParam const &param);

    /// \~english @brief Conversion operator to TValue.
    ///     Allows implicit conversion of a CachedValue object to its underlying TValue.
    /// \~russian @brief Оператор преобразования к TValue.
    ///     Позволяет неявное преобразование объекта CachedValue к его базовому типу TValue.
    /// \~english @return The underlying TValue.
    /// \~russian @return Базовое значение TValue.
    operator TValue() const;

    /// \~english @brief Equality comparison operator.
    ///     Compares the underlying TValue objects for equality.
    /// \~russian @brief Оператор сравнения на равенство.
    ///     Сравнивает базовые объекты TValue на равенство.
    /// \~english @param cached_value The other CachedValue object to compare with.
    /// \~russian @param cached_value Другой объект CachedValue для сравнения.
    /// \~english @return True if the underlying TValues are equal, false otherwise. Returns false if either value pointer is null.
    /// \~russian @return True, если базовые TValue равны, иначе false. Возвращает false, если один из указателей на значение null.
    bool operator == (CachedValue const &cached_value) const;

    /// \~english @brief Less-than comparison operator.
    ///     Compares the underlying TValue objects.
    /// \~russian @brief Оператор сравнения "меньше чем".
    ///     Сравнивает базовые объекты TValue.
    /// \~english @param cached_value The other CachedValue object to compare with.
    /// \~russian @param cached_value Другой объект CachedValue для сравнения.
    /// \~english @return True if this CachedValue's underlying TValue is less than the other's, false otherwise. Returns false if either value pointer is null.
    /// \~russian @return True, если базовое TValue этого CachedValue меньше, чем у другого, иначе false. Возвращает false, если один из указателей на значение null.
    bool operator <  (CachedValue const &cached_value) const;

private:
    friend struct std::hash < iridium::caching::CachedValue<TValue> >;

    /// \~english @brief Thread-local cache storage.
    ///     This map stores unique TValue instances (via std::shared_ptr) keyed by CachedValue wrappers.
    ///     The `thread_local` keyword ensures that each thread has its own independent cache.
    /// \~russian @brief Потоко-локальное хранилище кэша.
    ///     Эта карта хранит уникальные экземпляры TValue (через std::shared_ptr), ключами к которым являются обертки CachedValue.
    ///     Ключевое слово `thread_local` гарантирует, что каждый поток имеет свой независимый кэш.
    static thread_local std::unordered_map<CachedValue, std::shared_ptr<TValue> > m;

    /// \~english @brief Pointer to the actual cached value.
    ///     This shared_ptr points to the interned TValue instance within the thread-local cache.
    /// \~russian @brief Указатель на фактическое кэшированное значение.
    ///     Этот shared_ptr указывает на интернированный экземпляр TValue в потоко-локальном кэше.
    mutable std::shared_ptr<TValue> m_value;
};


// ----- implementation


template<typename TValue>
thread_local std::unordered_map<CachedValue<TValue>, std::shared_ptr<TValue> > CachedValue<TValue>::m =
std::unordered_map<CachedValue<TValue>, std::shared_ptr<TValue> >();


template<typename TValue>
CachedValue<TValue>::CachedValue() {
    m_value = std::make_shared<TValue>();
    // Attempt to find or insert the default value in the cache
    // This part seems to be missing in the original logic for default constructor,
    // adding it for consistency with the parameterized constructor's caching behavior.
    auto it = m.find(*this);
    if (it == m.end()) {
        m[*this] = m_value;
    } else {
        m_value = it->second;
    }
}


template<typename TValue>
template<typename TParam>
CachedValue<TValue>::CachedValue(TParam const &value) {
    // todo: out of memory handling
    // Initial construction to allow hashing and lookup
    m_value = std::make_shared<TValue>(value);
    auto it = m.find(*this); // Look up based on the content of the new value
    if(it == m.end()) { // Not found in cache
        // todo: out of memory handling
        // m_value is already the correct new value, just insert it into the map
        m[*this] = m_value;
    } else { // Found in cache
        m_value = it->second; // Use the existing shared_ptr
    }
}


template<typename TValue>
template<typename TParam>
CachedValue<TValue> &CachedValue<TValue>::operator = (TParam const &param) {
    // Construct a temporary CachedValue to leverage its caching/interning logic
    CachedValue temp_cached_value(param);
    // Assign the (potentially shared) m_value from the temporary
    m_value = temp_cached_value.m_value;
    return *this;
}


template<typename TValue>
CachedValue<TValue>::operator TValue() const {
    return *m_value;
}


template<typename TValue>
bool CachedValue<TValue>::operator == (CachedValue const &cached_value) const {
    // If both point to the same shared_ptr instance, they are definitionally equal (and fast check)
    if (m_value == cached_value.m_value) return true;
    // Otherwise, if both pointers are valid, compare the pointed-to values.
    if (m_value && cached_value.m_value) return *m_value == *cached_value.m_value;
    // If one is null and the other isn't, or both are null but not the same instance (covered by first check), they are not equal.
    return false;
}


template<typename TValue>
bool CachedValue<TValue>::operator <  (CachedValue const &cached_value) const {
    // Comparison requires valid pointers.
    if (m_value && cached_value.m_value) return *m_value < *cached_value.m_value;
    // Define behavior for nulls: consider null less than non-null, two nulls are not less than each other.
    if (!m_value && cached_value.m_value) return true; // this is null, other is not
    return false; // otherwise (this is not null and other is, or both are null)
}


/// \~english @brief Typedef for a CachedValue specializing in std::string.
///     Provides a convenient alias for frequently used cached strings.
/// \~russian @brief Typedef для CachedValue, специализирующегося на std::string.
///     Предоставляет удобный псевдоним для часто используемых кэшированных строк.
typedef CachedValue<std::string> CachedString;


/// \~english @brief Converts a CachedString to a std::string.
///     This function is likely part of the Iridium conversion framework.
/// \~russian @brief Преобразует CachedString в std::string.
///     Эта функция, вероятно, является частью фреймворка преобразования Iridium.
/// \~english @param source The CachedString object to convert.
/// \~russian @param source Объект CachedString для преобразования.
/// \~english @return The std::string representation of the CachedString.
/// \~russian @return Строковое представление std::string объекта CachedString.
std::string convertToString(iridium::caching::CachedString const &source);


} // caching
} // iridium


/// \~english @brief Specialization of std::hash for iridium::caching::CachedValue.
///     Enables CachedValue objects to be used as keys in unordered associative containers like std::unordered_map.
///     The hash is computed based on the underlying TValue.
/// \~russian @brief Специализация std::hash для iridium::caching::CachedValue.
///     Позволяет использовать объекты CachedValue в качестве ключей в неупорядоченных ассоциативных контейнерах, таких как std::unordered_map.
///     Хэш вычисляется на основе базового TValue.
template<typename TValue>
struct std::hash<iridium::caching::CachedValue<TValue> > {
    /// \~english @brief Computes the hash for a CachedValue.
    /// \~russian @brief Вычисляет хэш для CachedValue.
    /// \~english @param k The CachedValue object to hash.
    /// \~russian @param k Объект CachedValue для хэширования.
    /// \~english @return The computed hash value.
    /// \~russian @return Вычисленное хэш-значение.
    std::size_t operator()(iridium::caching::CachedValue<TValue> const &k) const {
        // Hash is based on the pointed-to value.
        // Assumes k.m_value is always valid for a CachedValue instance used in a hashable context.
        // If m_value could be null for a "valid" key, this would need adjustment.
        return std::hash<TValue>()(*(k.m_value));
    }
};


/// \~english @brief Overloads the addition operator for CachedValue.
///     Allows adding a TLValue to a CachedValue<TRValue>, assuming TRValue can be constructed from TLValue or they are compatible.
/// \~russian @brief Перегружает оператор сложения для CachedValue.
///     Позволяет складывать TLValue с CachedValue<TRValue>, предполагая, что TRValue может быть сконструирован из TLValue или они совместимы.
/// \~english @param l The left-hand operand.
/// \~russian @param l Левый операнд.
/// \~english @param r The right-hand CachedValue operand.
/// \~russian @param r Правый операнд типа CachedValue.
/// \~english @return The result of the addition, typically of type TRValue.
/// \~russian @return Результат сложения, обычно типа TRValue.
template<typename TLValue, typename TRValue>
TRValue operator + (TLValue const &l, iridium::caching::CachedValue<TRValue> const &r) {
    return static_cast<TRValue>(l) + static_cast<TRValue>(r);
}


/// \~english @brief Overloads the inequality operator for CachedValue.
///     Compares a TLValue with a CachedValue<TRValue> for inequality.
/// \~russian @brief Перегружает оператор неравенства для CachedValue.
///     Сравнивает TLValue с CachedValue<TRValue> на неравенство.
/// \~english @param l The left-hand operand.
/// \~russian @param l Левый операнд.
/// \~english @param r The right-hand CachedValue operand.
/// \~russian @param r Правый операнд типа CachedValue.
/// \~english @return True if the values are not equal, false otherwise.
/// \~russian @return True, если значения не равны, иначе false.
template<typename TLValue, typename TRValue>
bool operator != (TLValue const &l, iridium::caching::CachedValue<TRValue> const &r) {
    return static_cast<TRValue>(r) != static_cast<TRValue>(l);
}


DEFINE_CONVERT(std::string, iridium::caching::CachedString);


#endif // HEADER_CACHED_VALUE_589D8C09_61B8_4639_9F01_493EC80EE2D8
