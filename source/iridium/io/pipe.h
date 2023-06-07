// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_TRANSMITTER_77691541_E3E0_49E9_8FEB_C42A3ED7114B
#define HEADER_TRANSMITTER_77691541_E3E0_49E9_8FEB_C42A3ED7114B


#include "iridium/io/stream.h"
#include "iridium/io/event.h"


namespace iridium {
namespace io {


class IPipeStreams {
public:
    DEFINE_INTERFACE(IPipeStreams)
    virtual IStreamReader::TSharedPtr getReader() const = 0;
    virtual IStreamWriter::TSharedPtr getWriter() const = 0;

    // todo:
//    virtual IStreamReader::TConstSharedPtr setReader(IStreamReader::TSharedPtr const &reader) const = 0;
//    virtual IStreamWriter::TConstSharedPtr setWriter(IStreamWriter::TSharedPtr const &writer) const = 0;

    virtual void set(
        IStreamReader::TSharedPtr const &reader,
        IStreamWriter::TSharedPtr const &writer
    ) = 0;
};


class IPipeTransmitter {
public:
    DEFINE_INTERFACE(IPipeTransmitter)
    // false - buffers are empty
    virtual bool transmit(Event::TConstSharedPtr const &event) = 0;
};


class IPipe: public IPipeStreams, public IPipeTransmitter {
public:
    DEFINE_INTERFACE(IPipe)
};


//bool operator < (IPipeStreams::TSharedPtr const &l, IPipeStreams::TSharedPtr const &r);


} // io
} // iridium


#endif // HEADER_TRANSMITTER_77691541_E3E0_49E9_8FEB_C42A3ED7114B
