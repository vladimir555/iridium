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


namespace iridium {
namespace io {
namespace implementation {


class CTransmitter: public ITransmitter {
public:
    DEFINE_IMPLEMENTATION(CTransmitter)

    CTransmitter(
        IStreamReader::TSharedPtr   const &reader,
        IStreamWriter::TSharedPtr   const &writer,
        size_t                      const &buffer_size = 5
    );

//    void set(IStream::TSharedPtr const &reader) override;
//    void set(IStream::TSharedPtr const &writer) override;
//    IStream::TSharedPtr getReader() const override;
//    IStream::TSharedPtr getWriter() const override;

    bool transmit() override;

private:
    IStreamReader::TSharedPtr   m_reader;
    IStreamWriter::TSharedPtr   m_writer;
    size_t                      m_buffer_size;
};
    

} // implementation
} // io
} // iridium


#endif // HEADER_TRANSMITTER_5C67BADB_452C_4A6B_963B_63DBF32A6D5D
