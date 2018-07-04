#ifndef HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
#define HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1


#include "utility/convertion/convert.h"
#include "utility/smart_ptr.h"
#include <vector>
#include <string>


namespace utility {
namespace io {


class Buffer: public std::vector<unsigned char> {
public:
    DEFINE_CREATE(Buffer)
    Buffer() = default;
    Buffer(std::string const &str);
    template<typename ... TArgs>
    Buffer(TArgs ... args): std::vector<unsigned char>(args ...) {};
};


} // io
} // utility


DEFINE_CONVERT(utility::io::Buffer, std::string)
DEFINE_CONVERT(std::string, utility::io::Buffer)


#endif // HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
