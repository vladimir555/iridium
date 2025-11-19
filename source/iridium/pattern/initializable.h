// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
#define HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08


#include "iridium/smart_ptr.h"


namespace iridium::pattern {


class IInitializable {
public:
    /// \~english @brief Defines standard smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.) for the `IInitializable` interface.
    /// \~russian @brief Определяет стандартные typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.) для интерфейса `IInitializable`.
    DEFINE_SMART_PTR(IInitializable)
    /// \~english @brief Virtual destructor to ensure proper cleanup of resources in derived classes.
    /// \~russian @brief Виртуальный деструктор для обеспечения корректного освобождения ресурсов в производных классах.
    virtual ~IInitializable() = default;

    virtual void initialize() = 0;
    virtual void finalize() = 0;
};


} // namespace iridium::pattern


#endif // HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
