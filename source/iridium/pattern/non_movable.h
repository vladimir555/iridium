// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NON_MOVABLE_8C6763E9_B57A_4D83_93DD_AF9608B7E11A
#define HEADER_NON_MOVABLE_8C6763E9_B57A_4D83_93DD_AF9608B7E11A


namespace iridium {
namespace pattern {

/// \~english @brief A utility base class intended to make derived classes non-movable.
///     By inheriting from `NonMovable` (typically privately or protectedly),
///     a class attempts to prevent instances of that class from being moved.
///     Note: For complete non-movability, a deleted move assignment operator
///     (`NonMovable& operator=(NonMovable &&) = delete;`) would typically also be declared.
/// \~russian @brief Служебный базовый класс, предназначенный для того, чтобы делать производные классы неперемещаемыми.
///     Путем наследования от `NonMovable` (обычно приватного или защищенного),
///     класс пытается предотвратить перемещение своих экземпляров.
///     Примечание: Для полной неперемещаемости обычно также объявляется удаленный
///     оператор присваивания перемещением (`NonMovable& operator=(NonMovable &&) = delete;`).
class NonMovable {
public:
    /// \~english @brief Virtual default destructor.
    ///     Ensures proper destruction of derived classes if `NonMovable` were used as a polymorphic base.
    /// \~russian @brief Виртуальный деструктор по умолчанию.
    ///     Обеспечивает правильное уничтожение производных классов, если `NonMovable` используется как полиморфная база.
    virtual ~NonMovable() = default;
protected:
    /// \~english @brief Protected default constructor.
    ///     Allows derived classes to be default-constructed while still being non-movable (partially, in this case).
    /// \~russian @brief Защищенный конструктор по умолчанию.
    ///     Позволяет производным классам быть сконструированными по умолчанию, оставаясь при этом неперемещаемыми (частично, в данном случае).
    NonMovable() = default;
    /// \~english @brief Deleted move constructor (protected).
    ///     Attempts to prevent instances of derived classes from being move-constructed.
    ///     Note: The signature `NonMovable(NonMovable const &&)` is unusual.
    ///     The standard deleted move constructor is typically `NonMovable(NonMovable &&) = delete;`.
    ///     A `const &&` rvalue reference is a rare construct and might not achieve the intended non-movable semantics as robustly as a non-const rvalue reference.
    /// \~russian @brief Удаленный конструктор перемещения (защищенный).
    ///     Пытается предотвратить конструирование перемещением экземпляров производных классов.
    ///     Примечание: Сигнатура `NonMovable(NonMovable const &&)` является необычной.
    ///     Стандартный удаленный конструктор перемещения обычно выглядит так: `NonMovable(NonMovable &&) = delete;`.
    ///     Rvalue-ссылка на константу (`const &&`) — редкая конструкция и может не обеспечивать предполагаемую семантику неперемещаемости так же надежно, как rvalue-ссылка на неконстантный объект.
    NonMovable(NonMovable&&) = delete;
    /// \~english @brief Deleted copy assignment operator (protected).
    /// \~russian @brief Удаленный оператор присваивания копированием (защищенный).
    NonMovable& operator=(NonMovable const&) = delete;
};


} // pattern
} // iridium


#endif // HEADER_NON_MOVABLE_8C6763E9_B57A_4D83_93DD_AF9608B7E11A
