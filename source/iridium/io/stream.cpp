#include "stream.h"


namespace iridium {
namespace io {

    
bool operator < (IStream::TSharedPtr const &l, IStream::TSharedPtr const &r) {
    return l && r && l->getID() < r->getID();
}
    
    
} // io
} // iridium

