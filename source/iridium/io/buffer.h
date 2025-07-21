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


class Buffer: public std::vector<uint8_t> {
public:
    DEFINE_CREATE(Buffer)
    Buffer() = default;
    /// @brief
    /// @param str
    Buffer(char const * const str);
    /// @brief
    /// @param str
    Buffer(std::string const &str);
    /// @brief
    /// @param buffers
    Buffer(std::list<Buffer::TSharedPtr> const &buffers);
    /// @brief
    /// @tparam ...TArgs
    /// @param ... args
    template<typename ... TArgs>
    Buffer(TArgs ... args);
    /// @brief
    /// @param suffix
    /// @return
    bool checkSuffixEqual(std::string const &suffix) const;
    /// @brief
    /// @param suffix
    /// @return
    bool checkSuffixEqual(uint8_t const * const suffix, size_t const &size) const;
    /// @brief
    /// @param suffix
    /// @return
    bool checkSuffixEqual(std::vector<uint8_t> const &suffix) const;

    void emplace_back(TSharedPtr const &);
};


template<typename ... TArgs>
Buffer::Buffer(TArgs ... args)
:
    std::vector<uint8_t>(args ...)
{}


} // io
} // iridium


DEFINE_CONVERT(iridium::io::Buffer, std::string)
DEFINE_CONVERT(std::string, iridium::io::Buffer)


#endif // HEADER_BUFFER_65899B5B_473A_41CF_A262_28E212DF19F1
