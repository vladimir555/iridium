#include <iridium/testing/tester.h>

#include <iridium/io/implementation/stream_port_fifo.h>


using iridium::io::URI;
using iridium::io::Buffer;
using iridium::io::implementation::CStreamPortFIFO;


TEST(stream_port_fifo) {
    URI uri("fifo:///var/tmp/test_fifo");
    auto fifo_server = CStreamPortFIFO::create(uri, true);
    auto fifo_client = CStreamPortFIFO::create(uri);

    fifo_server->initialize();
    fifo_client->initialize();

    fifo_client->write(Buffer::create("message"));
    auto buffer = fifo_server->read();

    fifo_client->finalize();
    fifo_server->finalize();

    ASSERT(Buffer("message"), equal, *buffer);
}
