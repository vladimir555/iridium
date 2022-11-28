#ifndef HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347
#define HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347


#include "iridium/enum.h"

#include "stream.h"


namespace iridium {
namespace io {


class IEvent {
public:
    DEFINE_INTERFACE(IEvent)
    DEFINE_ENUM(TType, NONE, OPEN, CLOSE, READ, WRITE, EOF_, ERROR)
    virtual void                setType(TType const &type) = 0;
    virtual TType               getType()   const = 0;
    virtual IStream::TSharedPtr getStream() const = 0;
};


} // io
} // iridium


#endif // HEADER_EVENT_4E4FC1CA_35F4_4CDC_9A74_D42BEB643347
