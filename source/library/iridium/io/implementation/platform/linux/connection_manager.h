#ifndef HEADER_CONNECTION_MANAGER_5DB70C0D_5A67_45B8_92E1_C62348476EFA
#define HEADER_CONNECTION_MANAGER_5DB70C0D_5A67_45B8_92E1_C62348476EFA


#include "iridium/platform.h"


#ifdef LINUX_PLATFORM


#include "iridium/io/connection_manager.h"


namespace iridium::io::implementation::platform {


class CConnectionManager: public IConnectionManager{
public:
    DEFINE_IMPLEMENTATION(CConnectionManager)
    CConnectionManager() = default;

    void initialize() override;
    void finalize() override;

    // client
    void manage(
        URI::TSharedPtr
            const &uri,
        IProtocol::TSharedPtr
            const &protocol) override;
    // server
    void manage(
        URI::TSharedPtr
            const &uri,
        IAcceptor::TSharedPtr
            const &acceptor) override;
};


} // iridium::io::implementation::platform


#endif // LINUX_PLATFORM


#endif // HEADER_CONNECTION_MANAGER_5DB70C0D_5A67_45B8_92E1_C62348476EFA
