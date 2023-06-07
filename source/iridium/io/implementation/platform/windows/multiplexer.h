#ifndef HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA
#define HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/multiplexer.h"

#include "iridium/threading/synchronized.h"

#include <unordered_map>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {

class CMultiplexer : public IMultiplexer, public threading::Synchronized<std::mutex> {
public:
    CMultiplexer();
    DEFINE_IMPLEMENTATION(CMultiplexer)

    void initialize() override;
    void finalize() override;
    std::list<IEvent::TSharedPtr> waitEvents() override;
    void subscribe(IStream::TConstSharedPtr const &stream) override;
    void unsubscribe(IStream::TConstSharedPtr const &stream) override;

private:
    HANDLE m_iocp;
    std::unordered_map<IStream::TID, IStream::TSharedPtr> m_map_id_stream;
};


} // namespace platform
} // namespace implementation
} // namespace io
} // iridium


#endif // WINDOWS_PLATFORM


#endif // HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA
