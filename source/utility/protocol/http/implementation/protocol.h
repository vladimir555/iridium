#ifndef HEADER_PROTOCOL_56C5E0DF_5C1C_4268_BCF5_860D48294BD1
#define HEADER_PROTOCOL_56C5E0DF_5C1C_4268_BCF5_860D48294BD1


#include "utility/protocol/protocol.h"
#include "utility/parsing/parser.h"
#include "utility/protocol/http/http.h"
#include "utility/protocol/packet.h"


namespace utility {
namespace protocol {
namespace http {
namespace implementation {


class CProtocol: public IProtocol {
public:
    DEFINE_CREATE(CProtocol)
    CProtocol(IHTTPHandler::TSharedPtr const &http_handler = nullptr);
    virtual ~CProtocol() = default;

    IPacket::TSharedPtr exchange(IPacket::TSharedPtr const &request) override;
private:
    parsing::IParser::TSharedPtr    m_parser;
    IHTTPHandler::TSharedPtr        m_http_handler;
};


} // implementation
} // http
} // protocol
} // utility


#endif // HEADER_PROTOCOL_56C5E0DF_5C1C_4268_BCF5_860D48294BD1
