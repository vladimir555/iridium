//
//  stream.hpp
//  utility
//
//  Created by volodja on 01.06.2018.
//





#ifndef HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358
#define HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358


#include "utility/smart_ptr.h"
#include "utility/pattern/initializable.h"

#include <vector>


namespace utility {
namespace io {
    

typedef std::vector<unsigned char> TBuffer;


class IStreamReader {
public:
    DEFINE_INTERFACE(IStreamReader)
    virtual TBuffer read(size_t const &size) = 0;
};


class IStreamWriter {
public:
    DEFINE_INTERFACE(IStreamWriter)
    virtual size_t write(TBuffer const &buffer) = 0;
};


class IStreamID {
public:
    DEFINE_INTERFACE(IStreamID)
    virtual int getID() const = 0;
};

    
class IStream:
    public IStreamReader,
    public IStreamWriter,
    public IStreamID
{
public:
    DEFINE_INTERFACE(IStream)
};


/*
events: accept, read, write, end -> acceptor worker -> initialize -> protocol workers -> finalize
protocol workers:
    transaction: event -> action -> on read: buffer -> protocol -> event on read, event end -> acceptor worker
 */


} // io
} // utility


#endif // HEADER_STREAM_ABFAF627_623F_4585_BCB8_CCC1FADF7358


