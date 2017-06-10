#ifndef HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
#define HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08


#include "utility/smart_ptr.h"


namespace utility {
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
} // utility


#endif // HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
