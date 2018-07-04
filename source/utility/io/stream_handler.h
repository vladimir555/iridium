#ifndef HEADER_STREAM_HANDLER_5D620AAC_438B_4AE1_85D9_45A5A447505E
#define HEADER_STREAM_HANDLER_5D620AAC_438B_4AE1_85D9_45A5A447505E


#include "listener.h"
#include "utility/smart_ptr.h"


namespace utility {
namespace io {
    

class IStreamHandler {
public:
    DEFINE_INTERFACE(IStreamHandler)
    virtual Event::TType handle(Event::TType const &type) = 0;
};
    
    
} // io
} // utility


#endif // HEADER_STREAM_HANDLER_5D620AAC_438B_4AE1_85D9_45A5A447505E
