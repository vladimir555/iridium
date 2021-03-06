// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

//
//  transmitter.h
//  iridium
//
//  created by volodja on 2019-05-29
//


#ifndef HEADER_TRANSMITTER_77691541_E3E0_49E9_8FEB_C42A3ED7114B
#define HEADER_TRANSMITTER_77691541_E3E0_49E9_8FEB_C42A3ED7114B


#include "iridium/smart_ptr.h"
#include "iridium/io/stream.h"
#include "iridium/io/listener.h"


namespace iridium {
namespace io {


class IPipeStreams {
public:
    DEFINE_INTERFACE(IPipeStreams)
    virtual IStreamReader::TConstSharedPtr getReader() const = 0;
    virtual IStreamWriter::TConstSharedPtr getWriter() const = 0;

    virtual void set(
        IStreamReader::TSharedPtr const &reader,
        IStreamWriter::TSharedPtr const &writer
    ) = 0;
};


class IPipe: public IPipeStreams {
public:
    DEFINE_INTERFACE(IPipe)
    // false - buffers are empty
    virtual bool transmit(IListener::Event::TConstSharedPtr const &event) = 0;
};


//bool operator < (IPipeStreams::TSharedPtr const &l, IPipeStreams::TSharedPtr const &r);


} // io
} // iridium


#endif // HEADER_TRANSMITTER_77691541_E3E0_49E9_8FEB_C42A3ED7114B
