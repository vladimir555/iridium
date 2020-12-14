#include "transmitter.h"


#include <set>


namespace iridium {
namespace io {


bool operator < (ITransmitterStreams::TSharedPtr const &l, ITransmitterStreams::TSharedPtr const &r) {
    std::set<IStream::TConstSharedPtr> sl{ l->getReader(), l->getWriter() };
    std::set<IStream::TConstSharedPtr> sr{ r->getReader(), r->getWriter() };

    return sl < sr; // ----->
}


} // io
} // iridium
