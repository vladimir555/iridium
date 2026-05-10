// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_PORT_ACCEPTOR_2C3E15E7_1388_4C93_AB3D_1F9403C9C668
#define HEADER_STREAM_PORT_ACCEPTOR_2C3E15E7_1388_4C93_AB3D_1F9403C9C668


#include "iridium/platform.h"


#ifdef FREEBSD_LIKE_PLATFORM


#include "../unix/stream_port_acceptor.h"


namespace iridium::io::implementation::platform {


typedef platform::unix_::CStreamPortAcceptor CStreamPortAcceptor;


} // iridium::io::implementation::platform


#endif // FREEBSD_LIKE_PLATFORM


#endif // HEADER_STREAM_PORT_ACCEPTOR_2C3E15E7_1388_4C93_AB3D_1F9403C9C668
