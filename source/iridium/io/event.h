// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `Event` structure for I/O operations and related types and utilities.
///     This includes the `Event` struct itself, enumerations for event operations and status,
///     and specializations of `std::hash` for `Event` and `Event::TSharedPtr`.
/// \~russian @file
/// @brief Определяет структуру `Event` для операций ввода-вывода, а также связанные типы и утилиты.
///     Включает саму структуру `Event`, перечисления для операций и статусов событий,
///     а также специализации `std::hash` для `Event` и `Event::TSharedPtr`.

#ifndef HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347
#define HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347


#include "iridium/enum.h"     // For DEFINE_ENUM
#include "iridium/smart_ptr.h"// For DEFINE_CREATE and TSharedPtr (used by Event::TSharedPtr)

#include "stream.h"           // For IStream::TSharedPtr


namespace iridium::io {


// todo: rm, deprecated
struct Event {
    /// \~english @brief Macro likely defining a static `create` method or similar factory functionality
    ///     for creating shared pointers (`Event::TSharedPtr`) to `Event` instances.
    /// \~russian @brief Макрос, вероятно, определяющий статический метод `create` или аналогичную фабричную функциональность
    ///     для создания умных указателей (`Event::TSharedPtr`) на экземпляры `Event`.
    DEFINE_CREATE(Event)

    /// \~english @brief Enumerates the types of I/O operations or significant occurrences for an event.
    /// \~russian @brief Перечисляет типы операций ввода-вывода или значимые события для события.
    DEFINE_ENUM(
        TOperation,
        /// \~english @brief Indicates a stream has been opened.
        /// \~russian @brief Указывает, что поток был открыт.
        OPEN,
        /// \~english @brief Indicates a read operation.
        /// \~russian @brief Указывает на операцию чтения.
        READ,
        /// \~english @brief Indicates a write operation.
        /// \~russian @brief Указывает на операцию записи.
        WRITE,
        TIMEOUT,
        /// \~english @brief Indicates an error has occurred on a stream.
        /// \~russian @brief Указывает, что произошла ошибка на потоке.
        ERROR_,
        /// \~english @brief Indicates a close operation or that a stream has been closed.
        /// \~russian @brief Указывает на операцию закрытия или на то, что поток был закрыт.
        CLOSE
    )

    /// \~english @brief Enumerates the status of an I/O operation.
    ///     This can be used to indicate the phase of an operation, for example, if it has just begun or has ended.
    /// \~russian @brief Перечисляет статус операции ввода-вывода.
    ///     Может использоваться для указания фазы операции, например, только что началась или уже завершилась.
    DEFINE_ENUM(
        TStatus,
        /// \~english @brief Indicates the beginning of an operation.
        /// \~russian @brief Указывает на начало операции.
        BEGIN,
        /// \~english @brief Indicates the end of an operation.
        /// \~russian @brief Указывает на конец операции.
        END)

    Event(
        IStream::TSharedPtr
            const &stream,
        TOperation
            const &operation,
        TStatus
            const &status = TStatus::UNKNOWN);

    IStream::TSharedPtr
              stream;
    TOperation
        operation;
    TStatus
        status;
};


} // namespace iridium::io


/// \~english @brief Specialization of `std::hash` for `iridium::io::Event`.
///     Enables `iridium::io::Event` objects to be used as keys in unordered associative containers
///     like `std::unordered_map` or `std::unordered_set`. The hash is typically computed
///     based on the event's stream, operation, and status.
///     The actual hash computation logic is implemented in the corresponding .cpp file.
/// \~russian @brief Специализация `std::hash` для `iridium::io::Event`.
///     Позволяет использовать объекты `iridium::io::Event` в качестве ключей в неупорядоченных
///     ассоциативных контейнерах, таких как `std::unordered_map` или `std::unordered_set`. Хэш обычно вычисляется
///     на основе потока, операции и статуса события.
///     Фактическая логика вычисления хэша реализована в соответствующем .cpp файле.
template<>
struct std::hash<iridium::io::Event> {
    size_t operator()
        (iridium::io::Event const &e) const;
};


/// \~english @brief Specialization of `std::hash` for `iridium::io::Event::TSharedPtr`.
///     Enables `iridium::io::Event::TSharedPtr` (shared pointers to Event objects) to be used as keys
///     in unordered associative containers like `std::unordered_map` or `std::unordered_set`.
///     The hash is typically computed based on the pointer value itself, or by dereferencing
///     the pointer and using the hash of the pointed-to `Event` object if appropriate (though hashing the pointer is more common for `std::shared_ptr` specializations).
///     The actual hash computation logic is implemented in the corresponding .cpp file.
/// \~russian @brief Специализация `std::hash` для `iridium::io::Event::TSharedPtr`.
///     Позволяет использовать `iridium::io::Event::TSharedPtr` (умные указатели на объекты Event) в качестве ключей
///     в неупорядоченных ассоциативных контейнерах, таких как `std::unordered_map` или `std::unordered_set`.
///     Хэш обычно вычисляется на основе самого значения указателя или путем разыменования
///     указателя и использования хэша объекта `Event`, на который он указывает, если это уместно (хотя хэширование указателя более распространено для специализаций `std::shared_ptr`).
///     Фактическая логика вычисления хэша реализована в соответствующем .cpp файле.
template<>
struct std::hash<iridium::io::Event::TSharedPtr> {
    size_t operator()
        (iridium::io::Event::TSharedPtr const &e) const;
};


DEFINE_ENUM_CONVERT(iridium::io::Event::TOperation)
DEFINE_ENUM_CONVERT(iridium::io::Event::TStatus)
DEFINE_CONVERT(std::string, iridium::io::Event)


#endif // HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347
