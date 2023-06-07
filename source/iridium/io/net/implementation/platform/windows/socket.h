// Copyright � 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_846B93DA_90F4_4B30_BBFF_EC3D1526964B
#define HEADER_SOCKET_846B93DA_90F4_4B30_BBFF_EC3D1526964B


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/uri.h"
#include "iridium/io/net/socket.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


class CSocket : public ISocket {
public:
	DEFINE_IMPLEMENTATION(CSocket)
	CSocket(URI const &uri, bool const &is_server_mode);

	void    initialize() override;
	void    finalize() override;

	//ISocket::TSharedPtr accept() override;
	//URI     getURI() const override;

	//int     getID() const override;

	//size_t write(Buffer::TSharedPtr const& buffer) override;
	//Buffer::TSharedPtr read(size_t const &size) override;

	//    void    flush() override;

//private:
//	CSocket(URI const &uri, int const &fd);
//
//	URI     getPeerURI(int const &fd);
//	void    setBlockingMode(bool const &is_blocking);
//
//	bool    m_is_blocking;
//	bool    m_is_server_mode;
//	URI     m_uri;
//	int     m_socket;
};


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // WINDOWS_PLATFORM


#endif // HEADER_SOCKET_846B93DA_90F4_4B30_BBFF_EC3D1526964B
