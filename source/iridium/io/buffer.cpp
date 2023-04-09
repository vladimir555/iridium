#include "buffer.h"

#include <string>


namespace iridium {
namespace io {


Buffer::Buffer(std::string const &str)
:
    std::vector<unsigned char>(str.begin(), str.end())
{}


Buffer::Buffer(std::list<Buffer::TSharedPtr> const &buffers) {
    for (auto const &buffer: buffers)
        this->insert(end(), buffer->begin(), buffer->end());
}


//size_t Buffer::findLast(std::string const &str) {
//    auto i = std::find_end(begin(), end(), str.begin(), str.end());
//    if (i == end())
//        return std::string::npos;

//    return i - begin();
//}


} // io
} // iridium


namespace {


iridium::io::Buffer convert_(std::string const &source) {
    return iridium::io::Buffer(source.begin(), source.end()); // ----->
}


std::string convert_(iridium::io::Buffer const &source) {
    return std::string(source.begin(), source.end()); // ----->
}


} // unnamed


IMPLEMENT_CONVERT(iridium::io::Buffer, std::string, convert_)
IMPLEMENT_CONVERT(std::string, iridium::io::Buffer, convert_)
