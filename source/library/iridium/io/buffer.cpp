// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "buffer.h"

#include <string>
#include <string.h>


namespace iridium::io {


Buffer::Buffer(char const *s)
:
    std::vector<uint8_t>(s, s + strlen(s))
{}


Buffer::Buffer(std::string const &s)
:
    std::vector<uint8_t>(s.begin(), s.end())
{}


Buffer::Buffer(std::list<Buffer::TSharedPtr> const &buffers) {
    for (auto const& buffer: buffers) {
        if (buffer && !buffer->empty()) {
            insert(end(), buffer->begin(), buffer->end());
        }
    }
}


bool Buffer::checkSuffixEqual(std::string const &suffix, std::string const &skip) const {
    if (suffix.empty())
        return true;

    if (size() < suffix.size())
        return false;

    size_t i = size();

    while (i > 0 && skip.find((*this)[i - 1]) != std::string::npos) {
        --i;
    }

    return (i >= suffix.size()) &&
           std::equal(suffix.rbegin(), suffix.rend(), rbegin() + (size() - i));
}


bool Buffer::checkSuffixEqual(uint8_t const *suffix, size_t const &size) const {
    return
        (this->size() >= size) &&
        std::equal(suffix, suffix + size, data() + (this->size() - size));
}


bool Buffer::checkSuffixEqual(std::vector<uint8_t> const &suffix) const {
    return (size() >= suffix.size()) &&
           std::equal(suffix.begin(), suffix.end(), end() - suffix.size());
}


void Buffer::emplace_back(Buffer::TSharedPtr const &buffer) {
    if (buffer && !buffer->empty()) {
        insert(end(), buffer->begin(), buffer->end());
    }
}


bool checkSuffix(std::list<Buffer::TSharedPtr> const &buffers, std::string const &suffix) {
    if (suffix.empty())
        return true;

    size_t total_bytes = 0;
    for (auto const &buffer : buffers) {
        if (buffer)
            total_bytes += buffer->size();
    }

    if (total_bytes < suffix.length())
        return false;

    ptrdiff_t suffix_idx = static_cast<ptrdiff_t>(suffix.length()) - 1;

    for (auto i = buffers.rbegin(); i != buffers.rend(); ++i) {
        auto const &buffer = *i;

        if (!buffer || buffer->size() == 0)
            continue;

        uint8_t const *data = buffer->data();
        size_t buf_size = buffer->size();

        for (size_t i = buf_size; i > 0; --i) {
            if (data[i - 1] != static_cast<uint8_t>(suffix[suffix_idx]))
                return false;

            --suffix_idx;

            if (suffix_idx < 0)
                return true;
        }
    }

    return false;
}


} // namespace iridium::io


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
