// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NON_MOVABLE_8C6763E9_B57A_4D83_93DD_AF9608B7E11A
#define HEADER_NON_MOVABLE_8C6763E9_B57A_4D83_93DD_AF9608B7E11A


namespace iridium::pattern {


/// \~english @brief A utility base class that makes derived classes non-movable.
/// \~english @details By inheriting from this class, a derived class's move constructor and move assignment operator are deleted.
/// \~russian @brief Служебный базовый класс, делающий производные классы неперемещаемыми.
/// \~russian @details При наследовании от этого класса, конструктор перемещения и оператор присваивания перемещением производного класса удаляются.
class NonMovable {
public:
    /// \~english @brief Virtual default destructor.
    ///     Ensures proper destruction of derived classes if `NonMovable` were used as a polymorphic base.
    /// \~russian @brief Виртуальный деструктор по умолчанию.
    ///     Обеспечивает правильное уничтожение производных классов, если `NonMovable` используется как полиморфная база.
    virtual ~NonMovable() = default;
protected:
    /// \~english @brief Default constructor.
    /// \~russian @brief Конструктор по умолчанию.
    NonMovable() = default;
    /// \~english @brief Deleted move constructor.
    /// \~russian @brief Удаленный конструктор перемещения.
    NonMovable(NonMovable const &&) = delete;
    /// \~english @brief Deleted move assignment operator.
    /// \~russian @brief Удаленный оператор присваивания перемещением.
    NonMovable &operator=(NonMovable const &) const = delete;
};


} // namespace iridium::pattern


#endif // HEADER_NON_MOVABLE_8C6763E9_B57A_4D83_93DD_AF9608B7E11A
