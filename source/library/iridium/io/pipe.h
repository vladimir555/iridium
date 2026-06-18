// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PIPE_E26326CD_9194_40C7_A155_003BF8889BA1
#define HEADER_PIPE_E26326CD_9194_40C7_A155_003BF8889BA1


#include "stream.h"


namespace iridium::io {


struct TPipe {
    DEFINE_CREATE(TPipe);
    TStream::TSharedPtr
        reader;
    TStream::TSharedPtr
        writer;
};


} // iridium::io


#endif // HEADER_PIPE_E26326CD_9194_40C7_A155_003BF8889BA1
