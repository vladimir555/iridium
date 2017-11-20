#ifndef HEADER_FS_6EC22508_5BF1_436A_96A8_66FF4C98ED51
#define HEADER_FS_6EC22508_5BF1_436A_96A8_66FF4C98ED51


#include "utility/networking/socket.h"
#include "utility/networking/server/http.h"
#include "utility/logging/logger.h"

#include <string>
#include <unordered_map>


namespace utility {
namespace networking {
namespace server {
namespace implementation {


class CHTTPFSMapper: public IHTTPHandler {
public:
    DEFINE_CREATE(CHTTPFSMapper)
    CHTTPFSMapper(std::string const &path);
    virtual ~CHTTPFSMapper() = default;

    http::response::THttp handle(http::request::THttp const &request) override;
private:
    std::string m_path;
    std::unordered_map<std::string, networking::ISocket::TPacket> m_file_cache;

    networking::ISocket::TPacket readFileCached(std::string const &file_name);
};


} // implementation
} // server
} // networking
} // utility


#endif // HEADER_FS_6EC22508_5BF1_436A_96A8_66FF4C98ED51
