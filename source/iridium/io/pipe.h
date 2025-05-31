// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines interfaces related to pipe-like I/O mechanisms, focusing on stream access and data transmission.
/// \~russian @brief Определяет интерфейсы, связанные с каналоподобными механизмами ввода-вывода, с акцентом на доступ к потокам и передачу данных.
#ifndef HEADER_TRANSMITTER_77691541_E3E0_49E9_8FEB_C42A3ED7114B
#define HEADER_TRANSMITTER_77691541_E3E0_49E9_8FEB_C42A3ED7114B


#include "iridium/io/stream.h" // For IStreamReader, IStreamWriter
#include "iridium/io/event.h"  // For Event
#include "iridium/smart_ptr.h" // For DEFINE_INTERFACE and TSharedPtr


namespace iridium {
namespace io {


/// \~english @brief Interface for accessing the reader and writer streams of a pipe-like structure.
///     Provides methods to get and set the underlying stream components that handle reading and writing.
/// \~russian @brief Интерфейс для доступа к потокам чтения и записи каналоподобной структуры.
///     Предоставляет методы для получения и установки базовых потоковых компонентов, которые обрабатывают чтение и запись.
class IPipeStreams {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IPipeStreams)

    /// \~english @brief Gets the stream reader associated with this pipe.
    /// \~russian @brief Возвращает читателя потока, связанного с этим каналом.
    /// \~english @return A shared pointer to the `IStreamReader` for reading from the pipe.
    /// \~russian @return Умный указатель на `IStreamReader` для чтения из канала.
    virtual IStreamReader::TSharedPtr getReader() const = 0;

    /// \~english @brief Gets the stream writer associated with this pipe.
    /// \~russian @brief Возвращает писателя потока, связанного с этим каналом.
    /// \~english @return A shared pointer to the `IStreamWriter` for writing to the pipe.
    /// \~russian @return Умный указатель на `IStreamWriter` для записи в канал.
    virtual IStreamWriter::TSharedPtr getWriter() const = 0;

    // todo: The following methods were commented out in the original source.
    //    virtual IStreamReader::TConstSharedPtr setReader(IStreamReader::TSharedPtr const &reader) const = 0;
    //    virtual IStreamWriter::TConstSharedPtr setWriter(IStreamWriter::TSharedPtr const &writer) const = 0;

    /// \~english @brief Sets both the stream reader and writer for this pipe.
    ///     This method allows configuring or reconfiguring the underlying streams used for pipe operations.
    /// \~russian @brief Устанавливает как читателя, так и писателя потока для этого канала.
    ///     Этот метод позволяет настраивать или перенастраивать базовые потоки, используемые для операций с каналом.
    /// \~english @param reader A shared pointer to the `IStreamReader` to be used for reading.
    /// \~russian @param reader Умный указатель на `IStreamReader`, который будет использоваться для чтения.
    /// \~english @param writer A shared pointer to the `IStreamWriter` to be used for writing.
    /// \~russian @param writer Умный указатель на `IStreamWriter`, который будет использоваться для записи.
    virtual void set(
        IStreamReader::TSharedPtr const &reader,
        IStreamWriter::TSharedPtr const &writer
    ) = 0;
};


/// \~english @brief Interface for transmitting data, typically in response to an event.
///     This suggests a component that actively processes events to move data,
///     possibly between internal buffers or associated streams of a pipe.
/// \~russian @brief Интерфейс для передачи данных, обычно в ответ на событие.
///     Это предполагает компонент, который активно обрабатывает события для перемещения данных,
///     возможно, между внутренними буферами или связанными потоками канала.
class IPipeTransmitter {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IPipeTransmitter)

    /// \~english @brief Transmits data based on the provided event.
    ///     This method is called to process an event (e.g., data available, buffer ready) and perform
    ///     the necessary data transmission. The boolean return value likely indicates if any
    ///     data was transmitted or if internal buffers involved in transmission became empty.
    /// \~russian @brief Передает данные на основе предоставленного события.
    ///     Этот метод вызывается для обработки события (например, доступны данные, буфер готов) и выполнения
    ///     необходимой передачи данных. Возвращаемое логическое значение, вероятно, указывает, были ли
    ///     переданы какие-либо данные или стали ли пустыми внутренние буферы, участвующие в передаче.
    /// \~english @param event A const shared pointer to an `Event` that triggers or guides the transmission.
    /// \~russian @param event Константный умный указатель на `Event`, который инициирует или направляет передачу.
    /// \~english @return `true` if data transmission occurred or if buffers are still non-empty (implying more data might be pending or was processed),
    ///     `false` if internal buffers involved in transmission are now empty.
    /// \~russian @return `true`, если передача данных произошла или если буферы все еще не пусты (подразумевая, что могут ожидаться еще данные или данные были обработаны),
    ///     `false`, если внутренние буферы, участвующие в передаче, теперь пусты.
    virtual bool transmit(Event::TConstSharedPtr const &event) = 0;
};


/// \~english @brief Interface representing a pipe, combining stream access and data transmission capabilities.
///     A pipe is a conduit for data flow, allowing reading and writing, and can actively transmit data.
///     This interface aggregates `IPipeStreams` for stream endpoint access and `IPipeTransmitter` for data flow operations.
/// \~russian @brief Интерфейс, представляющий канал, объединяющий возможности доступа к потокам и передачи данных.
///     Канал представляет собой средство для потока данных, позволяющее чтение и запись, и может активно передавать данные.
///     Этот интерфейс агрегирует `IPipeStreams` для доступа к конечным точкам потока и `IPipeTransmitter` для операций потока данных.
class IPipe: public IPipeStreams, public IPipeTransmitter {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IPipe)
};


//bool operator < (IPipeStreams::TSharedPtr const &l, IPipeStreams::TSharedPtr const &r); // Commented out in source


} // io
} // iridium


#endif // HEADER_TRANSMITTER_77691541_E3E0_49E9_8FEB_C42A3ED7114B
