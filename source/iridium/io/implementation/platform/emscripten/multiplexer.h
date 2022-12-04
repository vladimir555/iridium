#ifndef HEADER_EVENT_PROVIDER_7E3C9A5C_AD20_4D3F_A2C8_3B4BD3E09DA7
#define HEADER_EVENT_PROVIDER_7E3C9A5C_AD20_4D3F_A2C8_3B4BD3E09DA7


#include "iridium/platform.h"


//#ifdef EMSCRIPTEN_PLATFORM


#include "iridium/io/multiplexer.h"


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


// todo: rename CMultiplexer
class CMultiplexer: public IMultiplexer {
public:
    DEFINE_IMPLEMENTATION(CMultiplexer)
    CMultiplexer(std::chrono::microseconds const &timeout = DEFAULT_WAITING_TIMEOUT) {
        throw std::runtime_error("not implemented");
    }

    void initialize()   override {
        throw std::runtime_error("not implemented");
    }
    void finalize()     override {
        throw std::runtime_error("not implemented");
    }

    std::list<IEvent::TSharedPtr> waitEvents() override {
        throw std::runtime_error("not implemented");
    }

    void subscribe  (IStream::TSharedPtr const &stream) override {
        throw std::runtime_error("not implemented");
    }
    void unsubscribe(IStream::TSharedPtr const &stream) override {
        throw std::runtime_error("not implemented");
    }
};


} // platform
} // implementation
} // io
} // iridium


//#endif // EMSCRIPTEN_PLATFORM


#endif // HEADER_EVENT_PROVIDER_7E3C9A5C_AD20_4D3F_A2C8_3B4BD3E09DA7
