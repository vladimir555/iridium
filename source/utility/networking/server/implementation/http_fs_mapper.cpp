#include "http_fs_mapper.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>
#include <unordered_map>

#include "utility/fs/files.h"


using std::string;
using std::ifstream;
using std::istream_iterator;
using std::copy;
using std::bind;
using std::unordered_map;
using std::placeholders::_1;
using utility::fs::checkFileExistence;


namespace utility {
namespace networking {
namespace server {
namespace implementation {


ISocket::TPacket readFile(string const &file_name) {
    LOGT << "read file: '" << file_name << "'";

    std::ifstream file(file_name, std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("file read error: file '" + file_name); // ----->

    typedef istream_iterator<ISocket::TPacket::value_type> istream_iterator;
    ISocket::TPacket buffer;

    file >> std::noskipws;
    copy(istream_iterator(file), istream_iterator(),  std::back_inserter(buffer));

    return buffer; // ----->
}


CHTTPFSMapper::CHTTPFSMapper(string const &path)
:
    m_path(path)
{}


http::response::THttp CHTTPFSMapper::handle(http::request::THttp const &request) {
    http::response::THttp response;
    try {
        auto file_name = m_path + request.Message.get().uri;
        response.Body = readFileCached(file_name);
        if (fs::extractFileNameExtension(file_name) == "jpg")
            response.Headers.ContentType = "image/jpg";
    } catch (std::exception const &e) {
        response.Message = {"HTTP/1.1", 404, "Not found"};
    }
    return response;
}


ISocket::TPacket CHTTPFSMapper::readFileCached(string const &file_name) {
    auto result           = m_file_cache[file_name];

    if (result.empty()) {
        result                  = readFile(file_name);
        m_file_cache[file_name] = result;
    }
    return result;
}


} // implementation
} // server
} // networking
} // utility
