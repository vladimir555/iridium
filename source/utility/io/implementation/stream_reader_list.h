#ifndef HEADER_STREAM_LIST_3E262388_342C_4EDD_9ADF_F42774DC3E9C
#define HEADER_STREAM_LIST_3E262388_342C_4EDD_9ADF_F42774DC3E9C


#include "utility/io/stream.h"
#include <list>
#include <memory>


namespace utility {
namespace io {
namespace implementation {


class CStreamReaderList: public IStreamReader {
public:
    DEFINE_IMPLEMENTATION(CStreamReaderList)
    CStreamReaderList();

    void    initialize()    override;
    void    finalize()      override;
    
    Buffer  read(size_t const &size)    override;
    int     getID()     const           override;

    void add(IStreamReader::TSharedPtr const &stream_reader);

private:
    std::list<IStreamReader::TSharedPtr>            m_streams;
    std::list<IStreamReader::TSharedPtr>::iterator  m_stream;
    bool m_is_started;
};


} // implementation
} // io
} // utility


#endif // HEADER_STREAM_LIST_3E262388_342C_4EDD_9ADF_F42774DC3E9C
