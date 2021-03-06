#pragma once


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/net/socket.h"
#include "iridium/io/listener.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


class CSocketClient: public ISocketStream {
public:
	DEFINE_IMPLEMENTATION(CSocketClient)
    CSocketClient(URL const& url, IListener::TSharedPtr const& listener = nullptr);

    void initialize() override;
    void finalize() override;

    URL getURL() const override;
    int getID() const override;
    Buffer::TSharedPtr read(size_t const& size) override;
    size_t write(Buffer::TSharedPtr const& buffer) override;
};


}
}
}
}
}


#endif // WINDOWS_PLATFORM
