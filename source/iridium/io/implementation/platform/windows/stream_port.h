#ifndef HEADER_STREAM_PORT_070ECA4F_6B07_4564_A97F_165282BB6685
#define HEADER_STREAM_PORT_070ECA4F_6B07_4564_A97F_165282BB6685


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/stream.h"
#include "iridium/io/uri.h"
#include "iridium/pattern/non_copyable.h"


namespace iridium::io::implementation::platform {


class CStreamPort: virtual public IStreamPort, public pattern::NonCopyable {
 public:
	DEFINE_IMPLEMENTATION(CStreamPort)
	CStreamPort(URI const &uri);

	void initialize() override;
    void finalize() override;

	std::list<uintptr_t> getHandles() const override;
	Buffer::TSharedPtr read(size_t const &size = DEFAULT_BUFFER_SIZE) override;
    size_t write(Buffer::TSharedPtr const &buffer) override;
private:
	//HANDLE m_handle;
};


} // iridium::io::implementation::platform


#endif  // WINDOWS_PLATFORM


#endif // HEADER_STREAM_PORT_070ECA4F_6B07_4564_A97F_165282BB6685
