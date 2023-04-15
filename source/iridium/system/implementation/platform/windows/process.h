#ifndef HEADER_PROCESS_3BDB0B9C_FA27_487B_BF07_5A19DC11193C
#define HEADER_PROCESS_3BDB0B9C_FA27_487B_BF07_5A19DC11193C


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/system/process.h"
#include "iridium/io/implementation/stream_port.h"


namespace iridium {
namespace system {
namespace implementation {
namespace platform {


class CProcessStream: public IProcess {
public:
    DEFINE_IMPLEMENTATION(CProcessStream)
    CProcessStream(
        std::string const &app,
        std::string const &args);
    CProcessStream(
        std::string const &app,
        std::vector<std::string> const &args);
    
    void initialize()   override;
    void finalize()     override;

    TID getID() const override;
    io::Buffer::TSharedPtr read(size_t const &size) override;
    size_t write(io::Buffer::TSharedPtr const &buffer) override;
    
    TState getState()   override;
};


} // platform
} // implementation
} // system
} // iridium


#endif // WINDOWS_PLATFORM


#endif // HEADER_PROCESS_3BDB0B9C_FA27_487B_BF07_5A19DC11193C
