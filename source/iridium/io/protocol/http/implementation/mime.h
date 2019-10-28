#ifndef HEADER_MIME_B406ABE9_2B4C_4F52_A8E9_CA92EA867A09
#define HEADER_MIME_B406ABE9_2B4C_4F52_A8E9_CA92EA867A09


#include <list>
#include <string>

#include "iridium/pattern/singleton.h"


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace implementation {


// not synchronized - only readable
class MIME: public pattern::Singleton<MIME> {
public:
    virtual ~MIME() = default;
    std::string getByFileNameExtension(std::string const &extension) const;

private:
    MIME();
    friend class pattern::Singleton<MIME>;

    struct TMIME {
        std::list<std::string>  extensions;
        std::string             type;
    };

    std::list<TMIME> m_mimes;
};


} // implementation
} // http
} // protocol
} // io
} // iridium


#endif // HEADER_MIME_B406ABE9_2B4C_4F52_A8E9_CA92EA867A09
