#ifndef HEADER_HTTP_54A372F7_F1CF_4F95_A39E_0C3CF90C46AF
#define HEADER_HTTP_54A372F7_F1CF_4F95_A39E_0C3CF90C46AF


#include "utility/smart_ptr.h"
#include "utility/pattern/initializable.h"
#include "utility/protocol/http/request.h"
#include "utility/protocol/http/response.h"

#include <string>
#include <list>


namespace utility {
namespace networking {
namespace server {


class IHTTPHandler {
public:
    DEFINE_SMART_PTR(IHTTPHandler)
    virtual ~IHTTPHandler() = default;

    virtual protocol::http::response::THttp handle(protocol::http::request::THttp const &request) = 0;
};


class IHTTP: public pattern::IInitializable {
public:
    DEFINE_SMART_PTR(IHTTP)
    virtual ~IHTTP() = default;
    typedef std::list<IHTTPHandler::TSharedPtr> THTTPHandlers;
};


} // server
} // networking
} // utility


#endif // HEADER_HTTP_54A372F7_F1CF_4F95_A39E_0C3CF90C46AF
