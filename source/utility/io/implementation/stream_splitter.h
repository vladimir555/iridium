#ifndef HEADER_STREAM_SPLITTER_C24580CF_71FF_4EDA_85E9_B04CF0F84810
#define HEADER_STREAM_SPLITTER_C24580CF_71FF_4EDA_85E9_B04CF0F84810


#include "utility/io/stream_splitter.h"


namespace utility {
namespace io {
namespace implementation {
    
    
class CStreamSplitter: public IStreamSplitter {
public:
    DEFINE_IMPLEMENTATION(CStreamSplitter)
    CStreamSplitter(
        IStreamReader::TSharedPtr const &reader,
        IStreamWriter::TSharedPtr const &writer);
    
    int     getID() const override;
    
    void    initialize()    override;
    void    finalize()      override;
    
    void    set(IStreamReader::TSharedPtr const &reader) override;
    void    set(IStreamWriter::TSharedPtr const &writer) override;
    
    Buffer  read (size_t const &size)    override;
    size_t  write(Buffer const &buffer)  override;
    
private:
    IStreamReader::TSharedPtr   m_reader;
    IStreamWriter::TSharedPtr   m_writer;
};
    
    
} // implementation
} // io
} // utility


#endif // HEADER_STREAM_SPLITTER_C24580CF_71FF_4EDA_85E9_B04CF0F84810
