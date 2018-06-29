#include "uri_fs_mapper.h"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>
#include <unordered_map>

#include "utility/io/fs/files.h"

#include "mime.h"


using std::string;
using std::ifstream;
using std::istream_iterator;
using std::copy;
using std::bind;
using std::unordered_map;
using std::placeholders::_1;
using utility::io::fs::checkFileExistence;
using utility::io::fs::extractFileNameExtension;
using utility::io::net::TPacket;


namespace utility {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


TPacket readFile(string const &file_name) {
    std::ifstream file(file_name, std::ios::binary);

    if (!file.is_open())
        throw std::runtime_error("file read error: file '" + file_name); // ----->

    typedef istream_iterator<TPacket::value_type> istream_iterator;
    TPacket buffer;

    file >> std::noskipws;
    copy(istream_iterator(file), istream_iterator(),  std::back_inserter(buffer));

    return buffer; // ----->
}


CURIFSMapper::CURIFSMapper(string const &fs_path)
:
    m_fs_path(fs_path)
{}


response::THttp CURIFSMapper::handle(request::THttp const &request) {
    protocol::http::response::THttp response;
    try {
        auto file_name                  = m_fs_path + request.Message.get().uri;
        response.Body                   = readFileCached(file_name);
        response.Headers.ContentType    = MIME::instance().getByFileNameExtension(extractFileNameExtension(file_name));
    } catch (std::exception const &e) {
        LOGW << "http request error: " << e.what();
        response.Message = {"HTTP/1.1", 404, "Not found"};
    }
    return response; // ----->
}


TPacket CURIFSMapper::readFileCached(string const &file_name) {
    auto result           = m_file_cache[file_name];

    if (result.empty()) {
        result                  = readFile(file_name);
        m_file_cache[file_name] = result;
    }
    return result; // ----->
}


} // implementation
} // http
} // protocol
} // io
} // utility
