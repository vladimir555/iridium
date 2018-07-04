#include <gtest/gtest.h>

#include <utility/strings.h>

#include <iostream>
using namespace std;

namespace utility {


TEST(strings, trim) {
    ASSERT_EQ(""        , trim(" "));
    ASSERT_EQ(""        , trim("     "));
    ASSERT_EQ("A"       , trim("A"));
    ASSERT_EQ("a"       , trim(" a"));
    ASSERT_EQ("abcde"   , trim("     \t\r\n\tabcde     \t\n\r"));
}


TEST(strings, split) {
    ASSERT_EQ( list<string>( { "1", "22", "333", "4444", "5" } ), split("1 22  333   4444    5  ", " ") );
}
    
    
} // utility


#include "utility/smart_ptr.h"
#include "utility/pattern/initializable.h"
#include "utility/io/buffer.h"
#include "utility/enum.h"


using utility::io::Buffer;


namespace test {

    
class IStream:
    public utility::pattern::IInitializable
{
public:
    DEFINE_INTERFACE(IStream)
    virtual int getID() const = 0;
};

    
class IStreamReader: public IStream {
public:
    DEFINE_INTERFACE(IStreamReader)
    virtual Buffer read(size_t const &size) = 0;
};


class IStreamWriter: public IStream {
public:
    DEFINE_INTERFACE(IStreamWriter)
    virtual size_t write(Buffer const &buffer) = 0;
};


class IFileStreamWriter: public IStreamWriter {
public:
    DEFINE_INTERFACE(IFileStreamWriter)
    virtual void flush() = 0;
};


class IFileStreamReader: public IStreamReader {
public:
    DEFINE_INTERFACE(IFileStreamReader)
    virtual size_t getSize() = 0;
};


//class IFileStream: public IFileReader, public IFileWriter {
//public:
//    DEFINE_INTERFACE(IFileStream)
//};

    
class CFileStream: public IStream {
protected:
    DEFINE_ENUM(TOpenMode, READ, WRITE)
    DEFINE_CREATE(CFileStream)
    // todo: fix classes CFileStream is only base class for reader and writer
    explicit CFileStream(std::string const &path, TOpenMode const &open_mode) {};
    virtual ~CFileStream() {};
    
public:
    void initialize()   override {};
    void finalize()     override {};
    
//    size_t write(Buffer const &buffer) override {return {};};
//    Buffer read (size_t const &size)   override {return {};};
    
//    void flush() override {};
    virtual int getID() const override {return {};};
};
    
    
class CFileStreamReader: public CFileStream, public IFileStreamReader {
public:
    DEFINE_CREATE(CFileStreamReader)
    CFileStreamReader(std::string const &path): CFileStream(path, TOpenMode::READ) {};
    virtual ~CFileStreamReader() = default;
    Buffer read(size_t const &size) override {return {};};
    size_t getSize() override {return {};}
    int getID() const override {
        return CFileStream::getID();
    }
    
    void initialize() override { CFileStream::initialize(); };
    void finalize() override   { CFileStream::finalize(); };
};


//    class Transmitter {
//    public:
//        Transmitter();
//    }

    
TEST(io, stream) {
    IFileStreamReader *stream = new CFileStreamReader(""); //CFileStreamReader::create("");
    stream->initialize();
    stream->getID();
    stream->read(0);
//    stream->write(Buffer());
//    stream->flush();
    stream->finalize();
    
    IStream::TSharedPtr i1(stream);
    IFileStreamReader::TSharedPtr i2 = std::dynamic_pointer_cast<IFileStreamReader>(i1);
    i2->getSize();
}

    
} // test
