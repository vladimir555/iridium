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


/// \~english @brief Represents an I/O event associated with a stream.
///     This structure encapsulates information about an event that has occurred on an I/O stream,
///     such as the type of operation (read, write, etc.) and its status.
/// \~russian @brief Представляет событие ввода-вывода, связанное с потоком.
///     Эта структура инкапсулирует информацию о событии, произошедшем в потоке ввода-вывода,
///     такую как тип операции (чтение, запись и т.д.) и ее статус.
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
        OPEN,
        READ,
        WRITE,
        EOF_,
        /// \~english @brief Indicates a timeout has occurred related to a stream operation.
        /// \~russian @brief Указывает, что произошел тайм-аут, связанный с операцией потока.
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
        BEGIN,
        END)

    Event(IStream::TSharedPtr const &stream, TOperation const &operation, TStatus const &status = TStatus::UNKNOWN);

    IStream::TSharedPtr stream;

    /// \~english @brief The type of I/O operation or occurrence this event represents (e.g., READ, WRITE, EOF_).
    /// \~russian @brief Тип операции ввода-вывода или события, которое представляет это событие (например, READ, WRITE, EOF_).
    TOperation          operation;

    /// \~english @brief The status of the operation associated with this event (e.g., BEGIN, END).
    /// \~russian @brief Статус операции, связанной с этим событием (например, BEGIN, END).
    TStatus             status;
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
    /// \~english @brief Computes the hash value for an `iridium::io::Event` object.
    /// \~russian @brief Вычисляет хэш-значение для объекта `iridium::io::Event`.
    /// \~english @param e The `iridium::io::Event` object to hash.
    /// \~russian @param e Объект `iridium::io::Event` для хэширования.
    /// \~english @return The computed hash value as `size_t`.
    /// \~russian @return Вычисленное хэш-значение типа `size_t`.
    size_t operator()(iridium::io::Event const &e) const;
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
    /// \~english @brief Computes the hash value for a shared pointer to an `iridium::io::Event` object.
    /// \~russian @brief Вычисляет хэш-значение для умного указателя на объект `iridium::io::Event`.
    /// \~english @param e The `iridium::io::Event::TSharedPtr` to hash.
    /// \~russian @param e Умный указатель `iridium::io::Event::TSharedPtr` для хэширования.
    /// \~english @return The computed hash value as `size_t`.
    /// \~russian @return Вычисленное хэш-значение типа `size_t`.
    size_t operator()(iridium::io::Event::TSharedPtr const &e) const;
};


DEFINE_ENUM_CONVERT(iridium::io::Event::TOperation)
DEFINE_ENUM_CONVERT(iridium::io::Event::TStatus)


#endif // HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347
