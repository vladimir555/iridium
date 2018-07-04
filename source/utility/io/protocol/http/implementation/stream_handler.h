#ifndef HEADER_STREAM_HANDLER_6C19960A_C91F_46C7_9532_47D511D11010
#define HEADER_STREAM_HANDLER_6C19960A_C91F_46C7_9532_47D511D11010


#include "utility/io/stream_handler.h"
#include "utility/io/stream_splitter.h"
#include "utility/io/stream.h"
#include "utility/io/buffer.h"
#include "utility/io/transmitter.h"
#include "utility/io/protocol/http/content_storage.h"
#include "utility/parsing/parser.h"


namespace utility {
namespace io {
namespace protocol {
namespace http {
namespace implementation {

    
class CStreamHandler: public IStreamHandler {
public:
    DEFINE_IMPLEMENTATION(CStreamHandler)
    CStreamHandler(IStreamPort::TSharedPtr const &client_stream);
    
    Event::TType handle(Event::TType const &type) override;
    
private:
    IStreamSplitter::TSharedPtr     m_server_stream;
    IStreamPort::TSharedPtr         m_client_stream;
    Buffer::TSharedPtr              m_write_buffer;
    parsing::IParser::TSharedPtr    m_parser;
    IContentStorage::TSharedPtr     m_content_storage;
    ITransmitter::TSharedPtr        m_transmitter;
};
    
    
} // implementation
} // http
} // protocol
} // io
} // utility


#endif // HEADER_STREAM_HANDLER_6C19960A_C91F_46C7_9532_47D511D11010
