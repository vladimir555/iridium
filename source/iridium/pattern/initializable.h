#ifndef HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
#define HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08


#include "iridium/smart_ptr.h"


namespace iridium {
namespace pattern {


///
class IInitializable {
public:
    DEFINE_SMART_PTR(IInitializable)
    virtual ~IInitializable() = default;
    ///
    virtual void initialize() = 0;
    ///
    virtual void finalize() = 0;
};


} // pattern
} // iridium


#endif // HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
