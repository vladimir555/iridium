#include "buffer.h"

#include <string>


namespace iridium {
namespace io {


Buffer::Buffer(char const * const str)
:
    Buffer(std::string(str))
{}


Buffer::Buffer(std::string const &str)
:
    std::vector<unsigned char>(str.begin(), str.end())
{}


Buffer::Buffer(std::list<Buffer::TSharedPtr> const &buffers) {
    for (auto const &buffer: buffers)
        this->insert(end(), buffer->begin(), buffer->end());
}


bool Buffer::checkSuffixEqual(std::string const &suffix) const {
    return
        size() >= suffix.size() &&
        std::equal(suffix.rbegin(), suffix.rend(), this->rbegin());
}


bool Buffer::checkSuffixEqual(uint8_t const * const suffix, size_t const &size) const {
    return
        this->size() >= size &&
        std::equal(suffix, suffix + size, this->data() + (this->size() - size));
}


bool Buffer::checkSuffixEqual(std::vector<uint8_t> const &suffix) const {
    return
        size() >= suffix.size() &&
        std::equal(suffix.begin(), suffix.end(), this->end() - suffix.size());
}


void Buffer::emplace_back(Buffer::TSharedPtr const &buffer) {
    if (buffer && !buffer->empty())
        insert(end(), buffer->begin(), buffer->end());
}


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
