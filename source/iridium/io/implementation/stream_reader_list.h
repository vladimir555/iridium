// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_LIST_3E262388_342C_4EDD_9ADF_F42774DC3E9C
#define HEADER_STREAM_LIST_3E262388_342C_4EDD_9ADF_F42774DC3E9C


#include "iridium/io/stream.h"
#include <list>
#include <memory>


namespace iridium {
namespace io {
namespace implementation {


/// \~english @brief Implements `IStreamReader` by composing a list of other `IStreamReader` instances.
///     It reads sequentially from the streams in the list. When one stream reaches EOF,
///     it automatically switches to the next one.
/// \~russian @brief Реализует `IStreamReader` путем композиции списка других экземпляров `IStreamReader`.
///     Он последовательно читает из потоков в списке. Когда один поток достигает EOF,
///     он автоматически переключается на следующий.
class CStreamReaderList: public IStreamReader {
public:
    DEFINE_IMPLEMENTATION(CStreamReaderList)
    /// \~english @brief Constructs an empty `CStreamReaderList`.
    ///     Streams must be added using the `add()` method.
    /// \~russian @brief Конструирует пустой `CStreamReaderList`.
    ///     Потоки должны быть добавлены с помощью метода `add()`.
    CStreamReaderList();

    /// \~english @brief Initializes all managed stream readers.
    /// \~russian @brief Инициализирует все управляемые читатели потоков.
    void    initialize()    override;
    /// \~english @brief Finalizes all managed stream readers.
    /// \~russian @brief Завершает работу всех управляемых читателей потоков.
    void    finalize()      override;
    
    /// \~english @brief Reads data sequentially from the list of managed streams.
    ///     If the current stream reaches EOF, it attempts to read from the next stream in the list.
    /// \~russian @brief Последовательно читает данные из списка управляемых потоков.
    ///     Если текущий поток достигает EOF, он пытается читать из следующего потока в списке.
    /// \~english @param size The maximum number of bytes to read.
    /// \~russian @param size Максимальное количество байт для чтения.
    /// \~english @return A buffer containing the data read, or `nullptr` if all streams have reached EOF.
    /// \~russian @return Буфер, содержащий прочитанные данные, или `nullptr`, если все потоки достигли EOF.
    Buffer::TSharedPtr      read(size_t const &size) override;

    /// \~english @brief Gets the OS handles of the currently active stream reader.
    /// \~russian @brief Получает дескрипторы ОС текущего активного читателя потока.
    /// \~english @return A list of OS handles from the current stream, or an empty list if no stream is active or the current stream has no handles.
    /// \~russian @return Список дескрипторов ОС из текущего потока или пустой список, если ни один поток не активен или текущий поток не имеет дескрипторов.
    std::list<uintptr_t>    getHandles() const override;

    /// \~english @brief Gets the URI of the currently active stream reader.
    /// \~russian @brief Получает URI текущего активного читателя потока.
    /// \~english @return The URI of the current stream, or `nullptr` if no stream is active or the current stream has no URI.
    /// \~russian @return URI текущего потока или `nullptr`, если ни один поток не активен или текущий поток не имеет URI.
    URI::TSharedPtr         getURI() const override;

    /// \~english @brief Adds a stream reader to the end of the list of streams to be read.
    /// \~russian @brief Добавляет читателя потока в конец списка потоков для чтения.
    /// \~english @param stream_reader The stream reader to add.
    /// \~russian @param stream_reader Читатель потока для добавления.
    void    add(IStreamReader::TSharedPtr const &stream_reader);

private:
    /// \~english @brief The list of stream readers managed by this instance.
    /// \~russian @brief Список читателей потоков, управляемых этим экземпляром.
    std::list<IStreamReader::TSharedPtr>            m_streams;
    /// \~english @brief An iterator pointing to the current stream reader in `m_streams` from which data is being read.
    /// \~russian @brief Итератор, указывающий на текущего читателя потока в `m_streams`, из которого читаются данные.
    std::list<IStreamReader::TSharedPtr>::iterator  m_stream;
};


} // implementation
} // io
} // iridium


#endif // HEADER_STREAM_LIST_3E262388_342C_4EDD_9ADF_F42774DC3E9C
