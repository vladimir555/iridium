#ifndef HEADER_CHANNEL_5FD4CE02_783F_4C83_8BE8_804EC068D52A
#define HEADER_CHANNEL_5FD4CE02_783F_4C83_8BE8_804EC068D52A


#include "sink.h"
#include "utility/smart_ptr.h"


namespace utility {
namespace logging {


///
class IChannel : public pattern::IInitializable {
public:
    DEFINE_SMART_PTR(IChannel)
    ///
    virtual ~IChannel() = default;
    ///
    virtual void attach(ISink::TSharedPtr sink) = 0;
    ///
    virtual void detach(ISink::TSharedPtr sink) = 0;
    ///
    virtual void log(TEvent &&e) = 0;
};


} // logging
} // utility


#endif // HEADER_CHANNEL_5FD4CE02_783F_4C83_8BE8_804EC068D52A
