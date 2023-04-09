#ifndef HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA
#define HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/multiplexer.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {

class CMultiplexer : public IMultiplexer {
 public:
	CMultiplexer() = default;
	DEFINE_IMPLEMENTATION(CMultiplexer)

    void initialize() override;
    void finalize() override;
    std::list<IEvent::TSharedPtr> waitEvents() override;
	void subscribe(IStream::TConstSharedPtr const &stream) override;
	void unsubscribe(IStream::TConstSharedPtr const &stream) override;
};


}  // namespace platform
}  // namespace implementation
}  // namespace io
} // iridium


#endif // WINDOWS_PLATFORM


#endif // HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA
