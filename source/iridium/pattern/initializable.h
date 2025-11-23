// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
#define HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08


#include "iridium/smart_ptr.h"


namespace iridium::pattern {


/// \~english @brief Interface for objects that require explicit initialization and finalization steps.
/// \~english @details This pattern is useful for objects whose setup and teardown are complex and cannot be fully handled in the constructor or destructor alone.
/// \~russian @brief Интерфейс для объектов, требующих явных шагов инициализации и финализации.
/// \~russian @details Этот паттерн полезен для объектов, чья настройка и демонтаж сложны и не могут быть полностью обработаны только в конструкторе или деструкторе.
class IInitializable {
public:
    /// \~english @brief Defines standard smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.) for the `IInitializable` interface.
    /// \~russian @brief Определяет стандартные typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.) для интерфейса `IInitializable`.
    DEFINE_SMART_PTR(IInitializable)
    /// \~english @brief Virtual destructor to ensure proper cleanup of resources in derived classes.
    /// \~russian @brief Виртуальный деструктор для обеспечения корректного освобождения ресурсов в производных классах.
    virtual ~IInitializable() = default;

    /// \~english @brief Performs the initialization logic for the object.
    /// \~russian @brief Выполняет логику инициализации объекта.
    virtual void initialize() = 0;

    /// \~english @brief Performs the finalization logic for the object.
    /// \~russian @brief Выполняет логику финализации объекта.
    virtual void finalize() = 0;
};


} // namespace iridium::pattern


#endif // HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
