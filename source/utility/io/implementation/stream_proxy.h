#ifndef HEADER_STREAM_PROXY_F1072816_4A43_4799_9949_2BBC7729A098
#define HEADER_STREAM_PROXY_F1072816_4A43_4799_9949_2BBC7729A098


#include "utility/io/stream_proxy.h"


namespace utility {
namespace io {
namespace implementation {


class CStreamProxy: public IStreamProxy {
public:
    DEFINE_IMPLEMENTATION(CStreamProxy)
    CStreamProxy(
        IStreamReader::TSharedPtr   const &reader,
        IStreamWriter::TSharedPtr   const &writer,
        size_t                      const &buffer_transmitting_size);
    void        transmit() override;
    TBuffer    &getBuffer() override;
    void        setReader(IStreamReader::TSharedPtr const &reader) override;
    void        setWriter(IStreamWriter::TSharedPtr const &writer) override;

private:
    IStreamReader::TSharedPtr   m_reader;
    IStreamWriter::TSharedPtr   m_writer;
    TBuffer                     m_buffer;
    size_t                      m_buffer_transmitting_size;
};


} // implementation
} // io
} // utility


#endif // HEADER_STREAM_PROXY_F1072816_4A43_4799_9949_2BBC7729A098
