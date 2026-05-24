// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
#define HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1


#include "iridium/convertion/convert.h"
#include "iridium/smart_ptr.h"

#include <vector>
#include <string>
#include <list>


namespace iridium::io {


//todo: maybe rm inheritance
class Buffer: private std::vector<uint8_t> {
public:
    DEFINE_CREATE(Buffer)

    Buffer() = default;
    Buffer(char const *s);
    Buffer(std::string const &s);
    Buffer(std::list<Buffer::TSharedPtr> const &buffers);

    template<typename ... TArgs>
    Buffer(TArgs && ... args);

    using std::vector<uint8_t>::size;
    using std::vector<uint8_t>::empty;
    using std::vector<uint8_t>::clear;
    using std::vector<uint8_t>::resize;
    using std::vector<uint8_t>::data;
    using std::vector<uint8_t>::begin;
    using std::vector<uint8_t>::end;
    using std::vector<uint8_t>::cbegin;
    using std::vector<uint8_t>::cend;
    using std::vector<uint8_t>::rbegin;
    using std::vector<uint8_t>::rend;
    using std::vector<uint8_t>::insert;
    using std::vector<uint8_t>::operator[];
    using std::vector<uint8_t>::at;
    using std::vector<uint8_t>::front;
    using std::vector<uint8_t>::back;

    bool checkSuffixEqual(std::string const &suffix, std::string const &skip = "") const;
    bool checkSuffixEqual(uint8_t const *suffix, size_t const &size) const;
    bool checkSuffixEqual(std::vector<uint8_t> const &suffix) const;
    void emplace_back(Buffer::TSharedPtr const &buffer);
};


template<typename ... TArgs>
Buffer::Buffer(TArgs && ... args)
:
    std::vector<uint8_t>(std::forward<TArgs>(args) ...)
{}


} // iridium::io


DEFINE_CONVERT(iridium::io::Buffer, std::string)
DEFINE_CONVERT(std::string, iridium::io::Buffer)


#endif // HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
