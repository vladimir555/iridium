#include <iridium/testing/tester.h>

#include <iridium/io/implementation/stream_port_ipc_client.h>


using iridium::io::URI;
using iridium::io::Buffer;
//using iridium::io::implementation::CStreamPortIPCClient;


TEST(stream_port_fifo) {
//    URI uri("fifo:///var/tmp/test_fifo");
//    auto fifo_server = CStreamPortIPCClient::create(uri);
//    auto fifo_client = CStreamPortIPCClient::create(uri);
//
//    fifo_server->initialize();
//    fifo_client->initialize();
//
//    fifo_client->write(Buffer::create("message"));
//    auto buffer = fifo_server->read();
//
//    fifo_client->finalize();
//    fifo_server->finalize();
//
//    ASSERT(Buffer("message"), equal, *buffer);
}
