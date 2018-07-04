//
//  transmitter.h
//  utility
//
//  created by volodja on 2019-05-29
//


#ifndef HEADER_TRANSMITTER_77691541_E3E0_49E9_8FEB_C42A3ED7114B
#define HEADER_TRANSMITTER_77691541_E3E0_49E9_8FEB_C42A3ED7114B


#include "utility/smart_ptr.h"
#include "utility/io/stream.h"


namespace utility {
namespace io {


class ITransmitter {
public:
    DEFINE_INTERFACE(ITransmitter)
//    virtual void setReader(IStream::TSharedPtr const &reader) = 0;
//    virtual void setWriter(IStream::TSharedPtr const &writer) = 0;
//    virtual IStream::TSharedPtr getReader() const = 0;
//    virtual IStream::TSharedPtr getWriter() const = 0;
    virtual bool transmit() = 0;
};


} // io
} // utility


#endif // HEADER_TRANSMITTER_77691541_E3E0_49E9_8FEB_C42A3ED7114B
