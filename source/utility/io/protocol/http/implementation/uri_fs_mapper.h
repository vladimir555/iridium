#ifndef HEADER_FS_6EC22508_5BF1_436A_96A8_66FF4C98ED51
#define HEADER_FS_6EC22508_5BF1_436A_96A8_66FF4C98ED51


#include "utility/io/net/types.h"
#include "utility/io/protocol/http/http_source.h"
//#include "utility/protocol/packet.h"
#include "utility/logging/logger.h"

#include <string>
#include <unordered_map>


namespace utility {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


class CURIFSMapper: public IHTTPSource {
public:
    DEFINE_IMPLEMENTATION(CURIFSMapper)

    CURIFSMapper(std::string const &fs_path);

    response::THttp handle(request::THttp const &request) override;
private:
    std::string m_fs_path;
    std::unordered_map<std::string, net::TPacket> m_file_cache;

    net::TPacket readFileCached(std::string const &file_name);
};


} // implementation
} // http
} // protocol
} // io
} // utility


#endif // HEADER_FS_6EC22508_5BF1_436A_96A8_66FF4C98ED51
