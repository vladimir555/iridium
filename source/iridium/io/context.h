// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONTEXT_5E470497_AC46_415C_B3E2_7003AE1D21F6
#define HEADER_CONTEXT_5E470497_AC46_415C_B3E2_7003AE1D21F6


#include "event.h"
#include "pipe.h"


namespace iridium::io {


// todo: rm, deprecated
/// \~english @deprecated This interface is deprecated and scheduled for removal.
/// \~russian @deprecated Этот интерфейс устарел и запланирован к удалению.
class IContext {
public:
    DEFINE_INTERFACE(IContext)

    virtual void
        pushEvent(Event::TSharedPtr const &event) = 0;
    virtual std::list<Event::TSharedPtr>
        popEvents() = 0;
    virtual std::list<Event::TSharedPtr>
        checkOutdatedStreams() = 0;
    virtual bool
        update  (Event::TSharedPtr const &event) = 0;
    virtual IPipe::TSharedPtr
        getPipe (Event::TSharedPtr const &event) = 0;
    virtual void
        remove  () = 0;

    // virtual bool transmit(Event::TSharedPtr const &event) = 0;
};


/// \~english @brief Interface for managing named pipes.
///     Provides methods to create, remove, and update named pipes by associating them
///     with stream reader and/or writer interfaces. This suggests a mechanism for
///     inter-process or intra-process communication using named pipe abstractions.
/// \~russian @brief Интерфейс для управления именованными каналами.
///     Предоставляет методы для создания, удаления и обновления именованных каналов путем
///     связывания их с интерфейсами чтения и/или записи потоков. Это предполагает механизм
///     межпроцессного или внутрипроцессного взаимодействия с использованием абстракций именованных каналов.
class IPipeManager {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IPipeManager)

    /// \~english @brief Creates a new named pipe.
    /// \~russian @brief Создает новый именованный канал.
    /// \~english @param name The name of the pipe to create.
    /// \~russian @param name Имя создаваемого канала.
    virtual void createPipe(std::string const &name) = 0;

    /// \~english @brief Removes an existing named pipe.
    /// \~russian @brief Удаляет существующий именованный канал.
    /// \~english @param name The name of the pipe to remove.
    /// \~russian @param name Имя удаляемого канала.
    /// \~english @throws iridium::Exception or similar if removal fails (e.g., pipe not found, permissions).
    /// \~russian @throws iridium::Exception или аналогичное, если удаление не удалось (например, канал не найден, проблемы с разрешениями).
    virtual void removePipe(std::string const &name) = 0;

    /// \~english @brief Associates a stream reader with the named pipe.
    ///     This allows data to be read from the pipe via the provided `IStreamReader` interface.
    /// \~russian @brief Связывает читателя потока с именованным каналом.
    ///     Это позволяет читать данные из канала через предоставленный интерфейс `IStreamReader`.
    /// \~english @param name The name of the pipe to update.
    /// \~russian @param name Имя обновляемого канала.
    /// \~english @param reader A shared pointer to an `IStreamReader` implementation.
    /// \~russian @param reader Умный указатель на реализацию `IStreamReader`.
    /// \~english @throws iridium::Exception or similar if the update fails (e.g., pipe not found).
    /// \~russian @throws iridium::Exception или аналогичное, если обновление не удалось (например, канал не найден).
    virtual void updatePipe(std::string const &name, IStreamReader::TSharedPtr const &reader) = 0;

    /// \~english @brief Associates a stream writer with the named pipe.
    ///     This allows data to be written to the pipe via the provided `IStreamWriter` interface.
    /// \~russian @brief Связывает писателя потока с именованным каналом.
    ///     Это позволяет записывать данные в канал через предоставленный интерфейс `IStreamWriter`.
    /// \~english @param name The name of the pipe to update.
    /// \~russian @param name Имя обновляемого канала.
    /// \~english @param writer A shared pointer to an `IStreamWriter` implementation.
    /// \~russian @param writer Умный указатель на реализацию `IStreamWriter`.
    /// \~english @throws iridium::Exception or similar if the update fails (e.g., pipe not found).
    /// \~russian @throws iridium::Exception или аналогичное, если обновление не удалось (например, канал не найден).
    virtual void updatePipe(std::string const &name, IStreamWriter::TSharedPtr const &writer) = 0;

    /// \~english @brief Associates both a stream reader and a stream writer with the named pipe.
    ///     This allows bidirectional communication through the pipe using the provided interfaces.
    /// \~russian @brief Связывает как читателя, так и писателя потока с именованным каналом.
    ///     Это позволяет осуществлять двунаправленную связь через канал с использованием предоставленных интерфейсов.
    /// \~english @param name The name of the pipe to update.
    /// \~russian @param name Имя обновляемого канала.
    /// \~english @param reader A shared pointer to an `IStreamReader` implementation.
    /// \~russian @param reader Умный указатель на реализацию `IStreamReader`.
    /// \~english @param writer A shared pointer to an `IStreamWriter` implementation.
    /// \~russian @param writer Умный указатель на реализацию `IStreamWriter`.
    /// \~english @throws iridium::Exception or similar if the update fails (e.g., pipe not found).
    /// \~russian @throws iridium::Exception или аналогичное, если обновление не удалось (например, канал не найден).
    virtual void updatePipe(std::string const &name, IStreamReader::TSharedPtr const &reader, IStreamWriter::TSharedPtr const &writer) = 0;
};


} // namespace iridium::io


#endif // HEADER_CONTEXT_5E470497_AC46_415C_B3E2_7003AE1D21F6
