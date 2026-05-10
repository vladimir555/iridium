// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_LIST_3E262388_342C_4EDD_9ADF_F42774DC3E9C
#define HEADER_STREAM_LIST_3E262388_342C_4EDD_9ADF_F42774DC3E9C


#include "iridium/io/stream.h"
#include <list>
#include <memory>


namespace iridium::io::implementation {


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

    void initialize() override;
    void finalize() override;

    Buffer::TSharedPtr
        read(size_t const &size) override;
    TMapHandleTypeIdent
        getHandles() const override;
    URI::TSharedPtr
        getURI() const override;

    void add(IStreamReader::TSharedPtr const &stream_reader);

private:
    /// \~english @brief The list of stream readers managed by this instance.
    /// \~russian @brief Список читателей потоков, управляемых этим экземпляром.
    std::list<IStreamReader::TSharedPtr>            m_streams;
    /// \~english @brief An iterator pointing to the current stream reader in `m_streams` from which data is being read.
    /// \~russian @brief Итератор, указывающий на текущего читателя потока в `m_streams`, из которого читаются данные.
    std::list<IStreamReader::TSharedPtr>::iterator  m_stream;
};


} // iridium::io::implementation


#endif // HEADER_STREAM_LIST_3E262388_342C_4EDD_9ADF_F42774DC3E9C
