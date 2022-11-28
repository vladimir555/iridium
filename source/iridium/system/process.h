#ifndef HEADER_PROCESS_70CE2A25_38CC_4D60_A1A7_B14F33DB94CF
#define HEADER_PROCESS_70CE2A25_38CC_4D60_A1A7_B14F33DB94CF


#include "iridium/io/stream.h"
#include "iridium/enum.h"


namespace iridium {
namespace system {


class IProcess: virtual public io::IStreamPort {
public:
    DEFINE_INTERFACE(IProcess)

    DEFINE_ENUM(TState, DONE, CRASHED, INTERRUPTED, RUNNING, TIMEOUT)

    virtual TState getState() = 0;
    virtual std::shared_ptr<int> getExitCode() = 0;
};


} // system
} // iridium


#endif // HEADER_PROCESS_70CE2A25_38CC_4D60_A1A7_B14F33DB94CF
