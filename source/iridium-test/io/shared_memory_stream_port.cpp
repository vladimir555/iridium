#include <iridium/testing/tester.h>
#include <iridium/io/implementation/shared_memory_stream_port.h>


using std::string;
using iridium::io::implementation::CSharedMemoryStreamPort;
using iridium::io::URI;


TEST(uri) {
    auto stream_port = CSharedMemoryStreamPort::create(URI("ipc://iridium_test_shared_memory"));
    stream_port->initialize();
    stream_port->finalize();
}
