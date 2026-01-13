// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONVERT_A8F9AEBC_6239_4771_BA93_C86AA9D34245
#define HEADER_CONVERT_A8F9AEBC_6239_4771_BA93_C86AA9D34245


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "../unix/convert.h"


#include "iridium/convertion/implementation/common.h"


namespace iridium::convertion::implementation {


template<>
struct TConvert<std::string, long long> {
    static std::string convert(long long const &value);
};


} // iridium::convertion::implementation


#endif // LINUX_PLATFORM


#endif // HEADER_CONVERT_A8F9AEBC_6239_4771_BA93_C86AA9D34245
