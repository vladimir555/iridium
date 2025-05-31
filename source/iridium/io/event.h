// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347
#define HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347


#include "iridium/enum.h"     // For DEFINE_ENUM
#include "iridium/smart_ptr.h"// For DEFINE_CREATE and TSharedPtr (used by Event::TSharedPtr)

#include "stream.h"           // For IStream::TSharedPtr


namespace iridium {
namespace io {


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
        /// \~english @brief Indicates an open operation or that a stream has been opened.
        /// \~russian @brief Указывает на операцию открытия или на то, что поток был открыт.
        OPEN,
        /// \~english @brief Indicates a read operation or that data is available for reading.
        /// \~russian @brief Указывает на операцию чтения или на то, что данные доступны для чтения.
        READ,
        /// \~english @brief Indicates a write operation or that the stream is ready for writing.
        /// \~russian @brief Указывает на операцию записи или на то, что поток готов для записи.
        WRITE,
        /// \~english @brief Indicates an End-Of-File condition has been reached on a stream.
        /// \~russian @brief Указывает, что на потоке достигнуто состояние конца файла (End-Of-File).
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
        /// \~english @brief Indicates the beginning phase of an operation.
        /// \~russian @brief Указывает на начальную фазу операции.
        BEGIN,
        /// \~english @brief Indicates the ending phase of an operation.
        /// \~russian @brief Указывает на конечную фазу операции.
        END
        // Note: The constructor uses TStatus::UNKNOWN as a default,
        // which implies UNKNOWN should ideally be part of this enum.
        // However, it's not explicitly defined by DEFINE_ENUM here.
    )

    /// \~english @brief Constructs an Event object.
    /// \~russian @brief Конструирует объект Event.
    /// \~english @param stream_ A shared pointer to the I/O stream associated with this event.
    /// \~russian @param stream_ Умный указатель на поток ввода-вывода, связанный с этим событием.
    /// \~english @param operation_ The type of I/O operation or occurrence this event represents.
    /// \~russian @param operation_ Тип операции ввода-вывода или события, которое представляет это событие.
    /// \~english @param status_ The status of the operation (e.g., BEGIN, END). Defaults to an unknown/unspecified status
    ///     if not provided (assuming TStatus::UNKNOWN is implicitly available or 0 maps to a sensible default).
    /// \~russian @param status_ Статус операции (например, BEGIN, END). По умолчанию используется неизвестный/неуказанный статус,
    ///     если не предоставлен (предполагая, что TStatus::UNKNOWN неявно доступен или 0 сопоставляется с разумным значением по умолчанию).
    Event(IStream::TSharedPtr const &stream_, TOperation const &operation_, TStatus const &status_ = TStatus::UNKNOWN);

    /// \~english @brief A shared pointer to the I/O stream on which this event occurred or to which it pertains.
    /// \~russian @brief Умный указатель на поток ввода-вывода, на котором произошло это событие или к которому оно относится.
    IStream::TSharedPtr stream;

    /// \~english @brief The type of I/O operation or occurrence this event represents (e.g., READ, WRITE, EOF_).
    /// \~russian @brief Тип операции ввода-вывода или события, которое представляет это событие (например, READ, WRITE, EOF_).
    TOperation          operation;

    /// \~english @brief The status of the operation associated with this event (e.g., BEGIN, END).
    /// \~russian @brief Статус операции, связанной с этим событием (например, BEGIN, END).
    TStatus             status;
};


} // io
} // iridium


/// \~english @brief Specialization of `std::hash` for `iridium::io::Event`.
///     Allows `iridium::io::Event` objects to be used as keys in unordered associative containers
///     like `std::unordered_map` or `std::unordered_set`.
///     The actual hash computation logic is expected in a corresponding .cpp file.
/// \~russian @brief Специализация `std::hash` для `iridium::io::Event`.
///     Позволяет использовать объекты `iridium::io::Event` в качестве ключей в неупорядоченных
///     ассоциативных контейнерах, таких как `std::unordered_map` или `std::unordered_set`.
///     Фактическая логика вычисления хэша ожидается в соответствующем .cpp файле.
template<>
struct std::hash<iridium::io::Event> {
    /// \~english @brief Hash function operator.
    /// \~russian @brief Оператор хэш-функции.
    /// \~english @param e The `Event` object to hash.
    /// \~russian @param e Объект `Event` для хэширования.
    /// \~english @return The computed hash value.
    /// \~russian @return Вычисленное хэш-значение.
    size_t operator()(iridium::io::Event const &e) const;
};


/// \~english @brief Specialization of `std::hash` for `iridium::io::Event::TSharedPtr`.
///     Allows shared pointers to `iridium::io::Event` objects to be used as keys
///     in unordered associative containers.
///     The actual hash computation logic is expected in a corresponding .cpp file.
/// \~russian @brief Специализация `std::hash` для `iridium::io::Event::TSharedPtr`.
///     Позволяет использовать умные указатели на объекты `iridium::io::Event` в качестве ключей
///     в неупорядоченных ассоциативных контейнерах.
///     Фактическая логика вычисления хэша ожидается в соответствующем .cpp файле.
template<>
struct std::hash<iridium::io::Event::TSharedPtr> {
    /// \~english @brief Hash function operator.
    /// \~russian @brief Оператор хэш-функции.
    /// \~english @param e The shared pointer to an `Event` object to hash.
    /// \~russian @param e Умный указатель на объект `Event` для хэширования.
    /// \~english @return The computed hash value.
    /// \~russian @return Вычисленное хэш-значение.
    size_t operator()(iridium::io::Event::TSharedPtr const &e) const;
};


#endif // HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347
