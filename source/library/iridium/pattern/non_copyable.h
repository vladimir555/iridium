// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NON_COPYABLE_F54A924C_4ECF_486A_BEB7_5E749E8DC0BD
#define HEADER_NON_COPYABLE_F54A924C_4ECF_486A_BEB7_5E749E8DC0BD


namespace iridium::pattern {

/// \~english @brief A utility base class that makes derived classes non-copyable.
///     By inheriting from `NonCopyable` (typically privately or protectedly),
///     a class automatically deletes its copy constructor and copy assignment operator,
///     thus preventing instances of that class from being copied or assigned.
/// \~russian @brief Служебный базовый класс, делающий производные классы некопируемыми.
///     Путем наследования от `NonCopyable` (обычно приватного или защищенного),
///     класс автоматически удаляет свой конструктор копирования и оператор присваивания копированием,
///     тем самым предотвращая копирование или присваивание экземпляров этого класса.
class NonCopyable {
public:
    /// \~english @brief Virtual default destructor.
    ///     Ensures proper destruction of derived classes through a base pointer if `NonCopyable`
    ///     were ever used as a polymorphic base, though its primary use is for its copy-prevention semantics.
    /// \~russian @brief Виртуальный деструктор по умолчанию.
    ///     Обеспечивает правильное уничтожение производных классов через указатель на базовый класс,
    ///     если `NonCopyable` когда-либо использовался как полиморфная база, хотя его основное
    ///     назначение - семантика предотвращения копирования.
    virtual ~NonCopyable() = default;
protected:
    /// \~english @brief Default constructor.
    /// \~russian @brief Конструктор по умолчанию.
    NonCopyable() = default;
    /// \~english @brief Deleted copy constructor.
    /// \~russian @brief Удаленный конструктор копирования.
    NonCopyable(NonCopyable const &) = delete;
    /// \~english @brief Deleted copy assignment operator.
    /// \~russian @brief Удаленный оператор присваивания копированием.
    NonCopyable &operator=(NonCopyable const &) const = delete;
};


} // namespace iridium::pattern


#endif // HEADER_NON_COPYABLE_F54A924C_4ECF_486A_BEB7_5E749E8DC0BD
