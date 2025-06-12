// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
#define HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08


#include "iridium/smart_ptr.h"


namespace iridium {
namespace pattern {

/// \~english @brief Interface for objects that require explicit initialization and finalization steps in their lifecycle.
///     This pattern is useful for objects whose setup or teardown involves complex operations
///     that should not be performed in the constructor or destructor, such as resource allocation/deallocation
///     that might fail or require specific ordering.
/// \~russian @brief Интерфейс для объектов, требующих явных шагов инициализации и финализации в их жизненном цикле.
///     Этот шаблон полезен для объектов, чья настройка или демонтаж включают сложные операции,
///     которые не должны выполняться в конструкторе или деструкторе, такие как выделение/освобождение ресурсов,
///     которые могут завершиться неудачно или требуют определенного порядка.
class IInitializable {
public:
    /// \~english @brief Defines standard smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.) for the `IInitializable` interface.
    /// \~russian @brief Определяет стандартные typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.) для интерфейса `IInitializable`.
    DEFINE_SMART_PTR(IInitializable)
    /// \~english @brief Virtual destructor to ensure proper cleanup of resources in derived classes.
    /// \~russian @brief Виртуальный деструктор для обеспечения корректного освобождения ресурсов в производных классах.
    virtual ~IInitializable() = default;
    /// \~english @brief Performs the initialization of the object.
    ///     This method should be called after the object is constructed and before it is used.
    ///     Implementations should ensure an object is in a valid and usable state after this call.
    /// \~russian @brief Выполняет инициализацию объекта.
    ///     Этот метод должен быть вызван после конструирования объекта и перед его использованием.
    ///     Реализации должны гарантировать, что объект находится в действительном и пригодном для использования состоянии после этого вызова.
    /// \~english @exception std::exception May be thrown if initialization fails.
    /// \~russian @exception std::exception Может быть выброшено, если инициализация завершается неудачно.
    virtual void initialize() = 0;
    /// \~english @brief Performs the finalization or cleanup of the object.
    ///     This method should be called when the object is no longer needed, typically before its destruction.
    ///     It should release any resources acquired during initialization or operation.
    /// \~russian @brief Выполняет финализацию или очистку объекта.
    ///     Этот метод должен быть вызван, когда объект больше не нужен, обычно перед его уничтожением.
    ///     Он должен освободить любые ресурсы, полученные во время инициализации или работы.
    /// \~english @exception std::exception May be thrown if finalization fails, though this is generally discouraged for cleanup methods.
    /// \~russian @exception std::exception Может быть выброшено, если финализация завершается неудачно, хотя это обычно не рекомендуется для методов очистки.
    virtual void finalize() = 0;
};


} // pattern
} // iridium


#endif // HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
