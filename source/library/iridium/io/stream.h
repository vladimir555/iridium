// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_C034E5FA_FF03_4205_B4B0_3F7EC4BFA90A
#define HEADER_STREAM_C034E5FA_FF03_4205_B4B0_3F7EC4BFA90A


#include "uri.h"
#include "buffer.h"

#include <list>


namespace iridium::io {


struct TStream {
    DEFINE_CREATE(TStream)
    URI::TSharedPtr
        uri;
    std::list<Buffer::TSharedPtr>
        buffers;
    size_t
        position;
};


} // iridium::io


#endif // HEADER_STREAM_C034E5FA_FF03_4205_B4B0_3F7EC4BFA90A
