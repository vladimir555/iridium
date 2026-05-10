#include <iridium/testing/tester.h>

#include <iridium/io/implementation/stream_port_client.h>
#include <iridium/io/implementation/stream_port_peer.h>
#include <iridium/io/implementation/stream_port_acceptor.h>


using iridium::io::URI;
using iridium::io::Buffer;
using iridium::io::implementation::CStreamPortClient;
using iridium::io::implementation::CStreamPortPeer;
using iridium::io::implementation::CStreamPortAcceptor;


// TEST(stream_port_fifo) {
//     URI uri("ipc://test_unix_socket");
//     auto client     = CStreamPortClient::create(uri);
//     auto acceptor   = CStreamPortAcceptor::create(uri);
//     auto peer       = CStreamPortPeer::create(uri, 0);
// //
// //    fifo_server->initialize();
// //    fifo_client->initialize();
// //
// //    fifo_client->write(Buffer::create("message"));
// //    auto buffer = fifo_server->read();
// //
// //    fifo_client->finalize();
// //    fifo_server->finalize();
// //
// //    ASSERT(Buffer("message"), equal, *buffer);
// }
