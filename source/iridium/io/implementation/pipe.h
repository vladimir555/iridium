// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_TRANSMITTER_5C67BADB_452C_4A6B_963B_63DBF32A6D5D
#define HEADER_TRANSMITTER_5C67BADB_452C_4A6B_963B_63DBF32A6D5D


#include "iridium/io/pipe.h"
#include "iridium/io/stream.h"


namespace iridium::io::implementation {


class CPipe: public IPipe {
public:
    DEFINE_CREATE(CPipe)

    static size_t const DEFAULT_BUFFER_SIZE;
    static size_t const DEFAULT_BUFFER_COUNT;

    CPipe();
    virtual ~CPipe();

    void set(
        IStreamReader::TSharedPtr const &reader,
        IStreamWriter::TSharedPtr const &writer
    ) override;

    IStreamReader::TSharedPtr getReader() const override;
    IStreamWriter::TSharedPtr getWriter() const override;

    bool transmit(Event::TConstSharedPtr const &event) override;

private:
    IStreamReader::TSharedPtr       m_reader;
    IStreamWriter::TSharedPtr       m_writer;
    size_t const                    m_buffer_size;
    size_t const                    m_buffer_count;
    std::list<Buffer::TSharedPtr>   m_buffers;
};


} // iridium::io::implementation


#endif // HEADER_TRANSMITTER_5C67BADB_452C_4A6B_963B_63DBF32A6D5D
