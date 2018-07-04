#ifndef HEADER_HTTP_54A372F7_F1CF_4F95_A39E_0C3CF90C46AF
#define HEADER_HTTP_54A372F7_F1CF_4F95_A39E_0C3CF90C46AF


#include "utility/smart_ptr.h"

#include "request.h"
#include "response.h"

#include <string>
#include <list>


namespace utility {
namespace io {
namespace protocol {
namespace http {


class IHTTPSource {
public:
    DEFINE_SMART_PTR(IHTTPSource)
    virtual ~IHTTPSource() = default;

    virtual response::THttp handle(request::THttp const &request) = 0;
};


} // http
} // protocol
} // io
} // utility


#endif // HEADER_HTTP_54A372F7_F1CF_4F95_A39E_0C3CF90C46AF
