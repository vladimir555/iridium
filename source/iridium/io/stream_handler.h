// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_HANDLER_5D620AAC_438B_4AE1_85D9_45A5A447505E
#define HEADER_STREAM_HANDLER_5D620AAC_438B_4AE1_85D9_45A5A447505E


#include "listener.h"
#include "iridium/smart_ptr.h"


namespace iridium {
namespace io {
    

class IStreamHandler {
public:
    DEFINE_INTERFACE(IStreamHandler)
    virtual Event::TType handle(Event::TType const &type) = 0;
};
    
    
} // io
} // iridium


#endif // HEADER_STREAM_HANDLER_5D620AAC_438B_4AE1_85D9_45A5A447505E
