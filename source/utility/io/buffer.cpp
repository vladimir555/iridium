#include "buffer.h"

#include <string>


namespace utility {
namespace io {


Buffer::Buffer(std::string const &str)
:
    std::vector<unsigned char>(str.begin(), str.end())
{}


} // io
} // utility


namespace {


utility::io::Buffer convert_(std::string const &source) {
    return utility::io::Buffer(source.begin(), source.end()); // ----->
}


std::string convert_(utility::io::Buffer const &source) {
    return std::string(source.begin(), source.end()); // ----->
}


} // unnamed


IMPLEMENT_CONVERT(utility::io::Buffer, std::string, convert_)
IMPLEMENT_CONVERT(std::string, utility::io::Buffer, convert_)
