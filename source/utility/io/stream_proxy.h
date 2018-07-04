#ifndef HEADER_STREAM_PROXY_C7F618BD_D59A_4F56_A75F_FA2D6A7B51C7
#define HEADER_STREAM_PROXY_C7F618BD_D59A_4F56_A75F_FA2D6A7B51C7


#include "stream.h"


namespace utility {
namespace io {


class IStreamProxy {
public:
    DEFINE_INTERFACE(IStreamProxy)
    virtual void        setReader(IStreamReader::TSharedPtr const &reader) = 0;
    virtual void        setWriter(IStreamWriter::TSharedPtr const &writer) = 0;
    virtual void        transmit() = 0;
    virtual TBuffer    &getBuffer() = 0;
};


} // io
} // utility


#endif // HEADER_STREAM_PROXY_C7F618BD_D59A_4F56_A75F_FA2D6A7B51C7
