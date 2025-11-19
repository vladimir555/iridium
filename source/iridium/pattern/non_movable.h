// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NON_MOVABLE_8C6763E9_B57A_4D83_93DD_AF9608B7E11A
#define HEADER_NON_MOVABLE_8C6763E9_B57A_4D83_93DD_AF9608B7E11A


namespace iridium::pattern {


class NonMovable {
public:
    /// \~english @brief Virtual default destructor.
    ///     Ensures proper destruction of derived classes if `NonMovable` were used as a polymorphic base.
    /// \~russian @brief Виртуальный деструктор по умолчанию.
    ///     Обеспечивает правильное уничтожение производных классов, если `NonMovable` используется как полиморфная база.
    virtual ~NonMovable() = default;
protected:
    NonMovable() = default;
    NonMovable(NonMovable const &&) = delete;
    NonMovable &operator=(NonMovable const &) const = delete;
};


} // namespace iridium::pattern


#endif // HEADER_NON_MOVABLE_8C6763E9_B57A_4D83_93DD_AF9608B7E11A
