#include "transmitter.h"


#include <set>


namespace iridium {
namespace io {


bool operator < (ITransmitter::TSharedPtr const &l, ITransmitter::TSharedPtr const &r) {
    std::set<IStream::TSharedPtr> sl{ l->getReader(), l->getWriter() };
    std::set<IStream::TSharedPtr> sr{ r->getReader(), r->getWriter() };

    return sl < sr; // ----->
}


} // io
} // iridium
