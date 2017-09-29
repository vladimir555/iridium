#ifndef HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA
#define HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA


#include "utility/parsing/serialization/node.h"

#include <string>


namespace utility {
namespace netwoking {
namespace http {


DEFINE_ROOT_NODE_BEGIN(HTTPResponse)
    DEFINE_ATTRIBUTE(std::string, ContentType)
    DEFINE_NODE_LIST_BEGIN(AllowList)
        DEFINE_ATTRIBUTE(std::string, Allow)
    DEFINE_NODE_LIST_END()
    DEFINE_ATTRIBUTE(size_t, ContentLength)
DEFINE_ROOT_NODE_END()


} // http
} // networking
} // utility


#endif // HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA
