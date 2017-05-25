#ifndef HEADER_TEXT_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
#define HEADER_TEXT_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0


#include <string>
#include <stdio.h>

#include "utility/smart_ptr.h"
#include "utility/pattern/initializable.h"


namespace utility {
namespace fs {


class ITextWriter: public pattern::IInitializable {
public:
    DEFINE_SMART_PTR(ITextWriter);
    ///
    ~ITextWriter() = default;
    ///
    virtual void writeLine(std::string const &line) = 0;
    ///
    virtual void flush() = 0;
private:
    ///
    std::string m_file_name;
};


} // fs
} // utility


#endif // HEADER_TEXT_WRITER_F3E80B53_9C20_48EC_9242_9B22CED298F0
