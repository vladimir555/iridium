#ifndef HEADER_SINK_3A3CF594_E847_4869_B666_7DF6E46ADCCA
#define HEADER_SINK_3A3CF594_E847_4869_B666_7DF6E46ADCCA


#include "utility/smart_ptr.h"
#include "utility/pattern/initializable.h"

#include "utility/enum.h"

#include "event.h"


namespace utility {
namespace logging {


///
class ISink : public pattern::IInitializable {
public:
    DEFINE_SMART_PTR(ISink);
    ///
    DEFINE_ENUM(
        TType,
        FILE,
        CONSOLE
    )
    ///
    virtual ~ISink() = default;
    ///
    virtual void log(TEvent const &e) = 0;
    ///
    static std::string makeLine(TEvent const &e);
};


} // logger
} // utility


#endif // HEADER_SINK_3A3CF594_E847_4869_B666_7DF6E46ADCCA
