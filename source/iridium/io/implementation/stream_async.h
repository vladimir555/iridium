#ifndef HEADER_STREAM_ASYNC_1647A313_3556_47AC_A239_8CBEF7B25CAF
#define HEADER_STREAM_ASYNC_1647A313_3556_47AC_A239_8CBEF7B25CAF


#include "iridium/io/stream.h"
#include "iridium/io/listener.h"


namespace iridium {
namespace io {
namespace implementation {


class CAsyncStream: public IStream {
public:
    DEFINE_IMPLEMENTATION(CAsyncStream)
    CAsyncStream(IStream::TSharedPtr const &stream, IListener::TSharedPtr const &listener);

    int getID() const override;

    void initialize() override;
    void finalize() override;

private:
    IStream::TSharedPtr     m_stream;
    IListener::TSharedPtr   m_listener;
};


class CAsyncStreamReader: public CAsyncStream, public IStreamReader {
public:
    DEFINE_IMPLEMENTATION(CAsyncStreamReader)
    CAsyncStreamReader(IStreamReader::TSharedPtr const &stream, IListener::TSharedPtr const &listener);
    Buffer::TSharedPtr read(size_t const &size) override;
private:
    IStreamReader::TSharedPtr m_stream;
};


class CAsyncStreamWriter: public CAsyncStream, public IStreamWriter {
public:
    DEFINE_IMPLEMENTATION(CAsyncStreamWriter)
    CAsyncStreamWriter(IStreamWriter::TSharedPtr const &stream, IListener::TSharedPtr const &listener);
    size_t write(Buffer::TSharedPtr const &buffer) override;
private:
    IStreamWriter::TSharedPtr m_stream;
};


} // implementation
} // io
} // iridium


#endif // HEADER_STREAM_ASYNC_1647A313_3556_47AC_A239_8CBEF7B25CAF
