// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB
#define HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB


#include "iridium/io/stream.h"


namespace iridium {
namespace io {
namespace implementation {


class CStreamReaderBuffer: public IStreamReader {
public:
    DEFINE_IMPLEMENTATION(CStreamReaderBuffer)
    CStreamReaderBuffer(Buffer::TSharedPtr const &buffer);
    
    void    initialize() override;
    void    finalize() override;
    int     getID() const override;
    
    Buffer::TSharedPtr read(size_t const &size) override;

private:
    Buffer::TSharedPtr  m_buffer;
    size_t              m_pos;
};


// todo: interface with getBuffer
class CStreamWriterBuffer: public IStreamWriter {
public:
    DEFINE_IMPLEMENTATION(CStreamWriterBuffer)
    CStreamWriterBuffer(Buffer::TSharedPtr const &buffer);
    
    void    initialize() override;
    void    finalize() override;
    int     getID() const override;

    size_t  write(Buffer::TSharedPtr const &buffer) override;

private:
    Buffer::TSharedPtr  m_buffer;
};


} // implementation
} // io
} // iridium


#endif // HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB
