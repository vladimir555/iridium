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


namespace iridium {
namespace io {


class Buffer: public std::vector<unsigned char> {
public:
    DEFINE_CREATE(Buffer)
    Buffer() = default;
    Buffer(char const * const str);
    Buffer(std::string const &str);
    Buffer(std::list<Buffer::TSharedPtr> const &buffers);
    template<typename ... TArgs>
    Buffer(TArgs ... args)
    :
        std::vector<unsigned char>(args ...)
    {}
//    size_t findLast(std::string const &str);
};


} // io
} // iridium


DEFINE_CONVERT(iridium::io::Buffer, std::string)
DEFINE_CONVERT(std::string, iridium::io::Buffer)


#endif // HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
