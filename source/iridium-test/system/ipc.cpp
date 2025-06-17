#include <iridium/testing/tester.h>

#include <iridium/system/implementation/ipc_stream.h>


using iridium::io::URI;
using iridium::io::Buffer;
using iridium::system::implementation::CIPCStream;


TEST(fifo) {
    URI uri("ipc:///var/tmp/test_fifo");
    auto ipcss = CIPCStream::create(uri, true);
    auto ipcsc = CIPCStream::create(uri);

    ipcss->initialize();
    ipcsc->initialize();

    ipcsc->write(Buffer::create("message"));
    auto buffer = ipcss->read();

    ipcsc->finalize();
    ipcss->finalize();

    ASSERT(Buffer("message"), equal, *buffer);
}
