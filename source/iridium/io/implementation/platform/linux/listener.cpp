/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "listener.h"


#ifdef LINUX_PLATFORM


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


CListener::CListener() {
}


void CListener::initialize() {
}


void CListener::finalize() {
}


void CListener::add(IStream::TSharedPtr const &stream) {
}


void CListener::del(IStream::TSharedPtr const &stream) {
}


CListener::TEvents CListener::wait() {
    return {};
}


} // platform
} // implementation
} // io
} // iridium


#endif // LINUX_PLATFORM
