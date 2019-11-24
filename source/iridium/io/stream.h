// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

//
//  stream.h
//  iridium
//
//  created by volodja on 2018-06-01
//



#ifndef HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358
#define HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358


#include "iridium/smart_ptr.h"
#include "iridium/pattern/initializable.h"

#include "buffer.h"

#include <vector>


namespace iridium {
namespace io {


class IStream: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IStream)
    // todo: for windows int &getID() const;
    virtual int getID() const = 0;
};

    
class IStreamReader: public virtual IStream {
public:
    DEFINE_INTERFACE(IStreamReader)
    virtual Buffer read(size_t const &size) = 0;
};


class IStreamWriter: public virtual IStream {
public:
    DEFINE_INTERFACE(IStreamWriter)
    virtual size_t write(Buffer const &buffer) = 0;
};

    
class IStreamPort:
    public IStreamReader,
    public IStreamWriter
{
public:
    DEFINE_INTERFACE(IStreamPort)
};

    
bool operator < (IStream::TSharedPtr const &l, IStream::TSharedPtr const &r);
    
    
/*
events: accept, read, write, end -> acceptor worker -> initialize -> protocol workers -> finalize
protocol workers:
    transaction: event -> action -> on read: buffer -> protocol -> event on read, event end -> acceptor worker
 */


} // io
} // iridium


#endif // HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358
