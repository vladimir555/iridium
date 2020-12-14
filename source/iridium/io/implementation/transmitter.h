// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

//
//  transmitter.h
//  iridium
//
//  created by volodja on 2019-05-29
//


#ifndef HEADER_TRANSMITTER_5C67BADB_452C_4A6B_963B_63DBF32A6D5D
#define HEADER_TRANSMITTER_5C67BADB_452C_4A6B_963B_63DBF32A6D5D


#include "iridium/io/transmitter.h"
#include "iridium/io/stream.h"
#include "iridium/threading/async_queue.h"


namespace iridium {
namespace io {
namespace implementation {


class CTransmitter: public ITransmitter {
public:
    DEFINE_CREATE(CTransmitter)

    static size_t const DEFAULT_BUFFER_SIZE;
    static size_t const DEFAULT_BUFFER_COUNT;

    CTransmitter(
        size_t                  const &buffer_size  = DEFAULT_BUFFER_SIZE,
        size_t                  const &buffer_count = DEFAULT_BUFFER_COUNT);

    virtual ~CTransmitter();

    void set(
        IStreamReader::TSharedPtr const &reader,
        IStreamWriter::TSharedPtr const &writer
    ) override;

    IStreamReader::TConstSharedPtr getReader() const override;
    IStreamWriter::TConstSharedPtr getWriter() const override;

    bool transmit(Event::TSharedPtr const &event) override;

private:
    IStreamReader::TSharedPtr       m_reader;
    IStreamWriter::TSharedPtr       m_writer;
    size_t const                    m_buffer_size;
    size_t const                    m_buffer_count;
    std::list<Buffer::TSharedPtr>   m_buffers;
};
    

} // implementation
} // io
} // iridium


#endif // HEADER_TRANSMITTER_5C67BADB_452C_4A6B_963B_63DBF32A6D5D
