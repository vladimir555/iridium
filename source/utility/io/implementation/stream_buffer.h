#ifndef HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB
#define HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB


#include "utility/io/stream.h"


namespace utility {
namespace io {
namespace implementation {


class CStreamReaderBuffer: public IStreamReader {
public:
    DEFINE_IMPLEMENTATION(CStreamReaderBuffer)
    CStreamReaderBuffer(TBuffer const &buffer);
    TBuffer read(size_t const &size) override;

private:
    TBuffer     m_buffer;
    size_t      m_pos;
};


class CStreamWriterBuffer: public IStreamWriter {
public:
    DEFINE_IMPLEMENTATION(CStreamWriterBuffer)
    CStreamWriterBuffer(TBuffer &buffer);
    size_t write(TBuffer const &packet) override;
private:
    TBuffer &m_buffer;
};


} // implementation
} // io
} // utility


#endif // HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB
