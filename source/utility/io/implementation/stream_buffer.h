#ifndef HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB
#define HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB


#include "utility/io/stream.h"


namespace utility {
namespace io {
namespace implementation {


class CStreamReaderBuffer: public IStreamReader {
public:
    DEFINE_IMPLEMENTATION(CStreamReaderBuffer)
    
    CStreamReaderBuffer(
//        IStreamReader::TSharedPtr   const &stream_reader,
        Buffer::TSharedPtr const &buffer);
    
    void    initialize() override;
    void    finalize() override;
    int     getID() const override;
    
    Buffer read(size_t const &size) override;

private:
//    IStreamReader::TSharedPtr   m_reader;
    Buffer::TSharedPtr          m_buffer;
    size_t                      m_pos;
};


class CStreamWriterBuffer: public IStreamWriter {
public:
    DEFINE_IMPLEMENTATION(CStreamWriterBuffer)
    CStreamWriterBuffer(Buffer::TSharedPtr const &buffer);
    
    void    initialize() override;
    void    finalize() override;
    int     getID() const override;

    size_t  write(Buffer const &buffer) override;

private:
    Buffer::TSharedPtr  m_buffer;
};


} // implementation
} // io
} // utility


#endif // HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB
