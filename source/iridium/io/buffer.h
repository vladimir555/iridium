// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
#define HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1


#include "iridium/convertion/convert.h"
#include "iridium/smart_ptr.h"
#include <vector>
#include <string>


namespace iridium {
namespace io {


class Buffer: public std::vector<unsigned char> {
public:
    DEFINE_CREATE(Buffer)
    Buffer() = default;
    Buffer(std::string const &str);
    template<typename ... TArgs>
    Buffer(TArgs ... args): std::vector<unsigned char>(args ...) {}
};


} // io
} // iridium


DEFINE_CONVERT(iridium::io::Buffer, std::string)
DEFINE_CONVERT(std::string, iridium::io::Buffer)


#endif // HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
