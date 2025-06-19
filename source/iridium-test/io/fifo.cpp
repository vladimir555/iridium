#include <iridium/testing/tester.h>

#include <iridium/io/implementation/stream_port_client.h>


using iridium::io::URI;
using iridium::io::Buffer;
using iridium::io::implementation::CStreamPortClient;


TEST(stream_port_fifo) {
    URI uri("ipc://test_unix_socket");
    auto client_stream = CStreamPortClient::create(uri);
//    auto fifo_client = CStreamPortClient::create(uri);
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
