#ifndef HEADER_HTTP_PROTOCOL_0A4A892F_1A6E_40CD_9ADF_3624646EC77E
#define HEADER_HTTP_PROTOCOL_0A4A892F_1A6E_40CD_9ADF_3624646EC77E


#include "utility/io/protocol/protocol.h"
#include "utility/io/protocol/http/http_source.h"


namespace utility {
namespace io {
namespace protocol {


class CHTTPProtocol: public IProtocol {
public:
    DEFINE_IMPLEMENTATION(CHTTPProtocol)
    CHTTPProtocol(http::IHTTPSource::TSharedPtr const &http_source);

    IStreamReader::TSharedPtr exchange(TBuffer const &request) override;
};


} // protocol
} // io
} // utility


#endif // HEADER_HTTP_PROTOCOL_0A4A892F_1A6E_40CD_9ADF_3624646EC77E
