#include <iridium/testing/tester.h>

#include <iridium/io/implementation/stream_port_fifo.h>


using iridium::io::URI;
using iridium::io::Buffer;
using iridium::io::implementation::CStreamPortFIFO;


TEST(stream_port_fifo) {
    URI uri("fifo:///var/tmp/test_fifo");
    auto ipcss = CStreamPortFIFO::create(uri, true);
    auto ipcsc = CStreamPortFIFO::create(uri);

    ipcss->initialize();
    ipcsc->initialize();

    ipcsc->write(Buffer::create("message"));
    auto buffer = ipcss->read();

    ipcsc->finalize();
    ipcss->finalize();

    ASSERT(Buffer("message"), equal, *buffer);
}
