/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "stream.h"


namespace iridium {
namespace io {

    
bool operator < (IStream::TConstSharedPtr const &l, IStream::TConstSharedPtr const &r) {
    return l && r && l->getID() < r->getID();
}
    
    
} // io
} // iridium

