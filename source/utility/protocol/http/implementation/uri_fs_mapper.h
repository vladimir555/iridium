#ifndef HEADER_FS_6EC22508_5BF1_436A_96A8_66FF4C98ED51
#define HEADER_FS_6EC22508_5BF1_436A_96A8_66FF4C98ED51


#include "utility/networking/types.h"
#include "utility/protocol/http/http.h"
//#include "utility/protocol/packet.h"
#include "utility/logging/logger.h"

#include <string>
#include <unordered_map>


namespace utility {
namespace protocol {
namespace http {
namespace implementation {


class CURIFSMapper: public IHTTPHandler {
public:
    DEFINE_IMPLEMENTATION(CURIFSMapper)

    CURIFSMapper(std::string const &fs_path);

    response::THttp handle(request::THttp const &request) override;
private:
    std::string m_fs_path;
    std::unordered_map<std::string, networking::TPacket> m_file_cache;

    networking::TPacket readFileCached(std::string const &file_name);
};


} // implementation
} // http
} // protocol
} // utility


#endif // HEADER_FS_6EC22508_5BF1_436A_96A8_66FF4C98ED51
