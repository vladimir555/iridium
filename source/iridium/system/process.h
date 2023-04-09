// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROCESS_70CE2A25_38CC_4D60_A1A7_B14F33DB94CF
#define HEADER_PROCESS_70CE2A25_38CC_4D60_A1A7_B14F33DB94CF


#include "iridium/io/stream.h"
#include "iridium/enum.h"


namespace iridium {
namespace system {


class IProcess: virtual public io::IStreamPort {
public:
    DEFINE_INTERFACE(IProcess)

    struct TState {
        DEFINE_ENUM(TCondition, DONE, CRASHED, INTERRUPTED, RUNNING, TIMEOUT);
        TCondition              condition;
        std::shared_ptr<int>    exit_code;
    };

    virtual TState getState() = 0;
};


} // system
} // iridium


#endif // HEADER_PROCESS_70CE2A25_38CC_4D60_A1A7_B14F33DB94CF
