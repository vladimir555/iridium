#ifndef HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA
#define HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/multiplexer.h"

#include "iridium/threading/synchronized.h"

#include <unordered_map>

#include <Windows.h>


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
    std::list<Event::TSharedPtr> waitEvents() override;
    void   subscribe(IStream::TSharedPtr const &stream) override;
    void unsubscribe(IStream::TSharedPtr const &stream) override;

private:
    HANDLE m_iocp;
    std::unordered_map<HANDLE, IStream::TSharedPtr> m_map_id_stream;
};


} // platform
} // implementation
} // io
} // iridium


#endif // WINDOWS_PLATFORM


#endif // HEADER_MULTIPLEXER_59B3D8EE_C01A_4041_8AD2_0DB9844EDAEA
