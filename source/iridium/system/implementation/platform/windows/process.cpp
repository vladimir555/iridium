#include "process.h"


#ifdef WINDOWS_PLATFORM


namespace iridium {
namespace system {
namespace implementation {
namespace platform {


CProcessStream::CProcessStream(
    std::string const &app,
    std::string const &args)
//: 
//    CStreamPort(io::URL("file://" + app)) 
{}


CProcessStream::CProcessStream(
    std::string const &app,
    std::vector<std::string> const &args)
//: 
//    CStreamPort(io::URL("file://" + app)) 
{}


void CProcessStream::initialize() {
    throw std::runtime_error("process stream is not implemented");
}


void CProcessStream::finalize()  {
    throw std::runtime_error("process stream is not implemented");
}


IProcess::TState CProcessStream::getState() {
    throw std::runtime_error("process stream is not implemented");
}


IProcess::TID CProcessStream::getID() const {
    throw std::runtime_error("process stream is not implemented");
}


io::Buffer::TSharedPtr CProcessStream::read(size_t const &size) {
    throw std::runtime_error("process stream is not implemented");
}


size_t CProcessStream::write(io::Buffer::TSharedPtr const &buffer) {
    throw std::runtime_error("process stream is not implemented");
}


} // platform
} // implementation
} // system
} // iridium


#endif // WINDOWS_PLATFORM
