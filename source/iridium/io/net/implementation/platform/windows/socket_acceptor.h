//// Copyright © 2019 Bulaev Vladimir.
//// Contacts: <bulaev_vladimir@mail.ru>
//// License: https://www.gnu.org/licenses/lgpl-3.0
//
//#pragma once
//
//
//#include "iridium/platform.h"
//
//
//#ifdef WINDOWS_PLATFORM
//
//
//#include "iridium/io/net/socket.h"
//
//
//namespace iridium {
//namespace io {
//namespace net {
//namespace implementation {
//namespace platform {
//
//
//class CSocketAcceptor : public ISocketAcceptor {
//public:
//    DEFINE_IMPLEMENTATION(CSocketAcceptor)
//    CSocketAcceptor(URL const& url, IListenerStreams::TSharedPtr const& listener = nullptr);
//
//    void initialize() override;
//    void finalize() override;
//
//    URL getURL() const override;
//    int getID() const override;
//
//    ISocketStream::TSharedPtr accept() override;
//};
//
//
//}
//}
//}
//}
//}
//
//
//#endif // WINDOWS_PLATFORM
