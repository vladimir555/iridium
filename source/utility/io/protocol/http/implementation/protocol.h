#ifndef HEADER_PROTOCOL_56C5E0DF_5C1C_4268_BCF5_860D48294BD1
#define HEADER_PROTOCOL_56C5E0DF_5C1C_4268_BCF5_860D48294BD1


#include "utility/io/protocol/protocol.h"
#include "utility/parsing/parser.h"
#include "utility/io/protocol/http/http_source.h"
#include "utility/io/protocol/packet.h"


namespace utility {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


class CProtocol: public IProtocol {
public:
    DEFINE_CREATE(CProtocol)
    CProtocol(IHTTPSource::TSharedPtr const &http_handler = nullptr);
    virtual ~CProtocol() = default;

//    IPacket::TSharedPtr exchange(IPacket::TSharedPtr const &request) override;
private:
    parsing::IParser::TSharedPtr    m_parser;
    IHTTPSource::TSharedPtr         m_http_handler;
};


} // implementation
} // http
} // protocol
} // io
} // utility


#endif // HEADER_PROTOCOL_56C5E0DF_5C1C_4268_BCF5_860D48294BD1
