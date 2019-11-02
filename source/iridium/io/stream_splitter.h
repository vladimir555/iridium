// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_SPLITTER_4B83B055_A008_4ECF_A9B4_D98F8738A808
#define HEADER_STREAM_SPLITTER_4B83B055_A008_4ECF_A9B4_D98F8738A808


#include "stream.h"


namespace iridium {
namespace io {
    

class IStreamSplitter: public IStreamPort {
public:
    DEFINE_INTERFACE(IStreamSplitter)
    virtual void set(IStreamReader::TSharedPtr const &reader) = 0;
    virtual void set(IStreamWriter::TSharedPtr const &writer) = 0;
};


} // io
} // iridium


#endif // HEADER_STREAM_SPLITTER_4B83B055_A008_4ECF_A9B4_D98F8738A808
