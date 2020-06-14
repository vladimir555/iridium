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


class ITransmitterStreams {
public:
    DEFINE_INTERFACE(ITransmitterStreams)
    virtual void setReader(IStreamReader::TSharedPtr const &reader) = 0;
    virtual void setWriter(IStreamWriter::TSharedPtr const &writer) = 0;
    virtual IStreamReader::TConstSharedPtr getReader() const = 0;
    virtual IStreamWriter::TConstSharedPtr getWriter() const = 0;
};


class ITransmitter: public ITransmitterStreams {
public:
    DEFINE_INTERFACE(ITransmitter)
    virtual bool transmit(Event::TSharedPtr const &event) = 0;
};


bool operator < (ITransmitter::TSharedPtr const &l, ITransmitter::TSharedPtr const &r);


} // io
} // iridium


#endif // HEADER_TRANSMITTER_77691541_E3E0_49E9_8FEB_C42A3ED7114B
