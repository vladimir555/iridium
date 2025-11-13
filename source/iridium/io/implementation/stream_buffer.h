// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB
#define HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB


#include "iridium/io/stream.h"


namespace iridium::io::implementation {


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4250)
#endif // _MSC_VER


class CStreamBuffer: public virtual IStream {
protected:
    explicit CStreamBuffer(Buffer::TSharedPtr const &buffer);
    
    void initialize() override;
    void finalize()   override;
    
    std::list<uintptr_t>    getHandles()    const override;
    URI::TSharedPtr         getURI()        const override;

    Buffer::TSharedPtr m_buffer;
};


class CStreamReaderBuffer: 
    public CStreamBuffer,
    public IStreamReader
{
public:
    DEFINE_IMPLEMENTATION(CStreamReaderBuffer)
    explicit CStreamReaderBuffer(Buffer::TSharedPtr const &buffer);
    
    Buffer::TSharedPtr read(size_t const &size) override;

private:
    size_t m_pos;
};


// todo: interface with getBuffer
class CStreamWriterBuffer: 
    public CStreamBuffer,
    public IStreamWriter
{
public:
    DEFINE_IMPLEMENTATION(CStreamWriterBuffer)
    explicit CStreamWriterBuffer(Buffer::TSharedPtr const &buffer);

    size_t write(Buffer::TSharedPtr const &buffer) override;
};


#ifdef _MSC_VER
#pragma warning(pop)
#endif // _MSC_VER


} // namespace iridium::io::implementation


#endif // HEADER_STREAM_BUFFER_4242674C_C032_40C0_980A_3962F4396ACB
