#ifndef HEADER_CONTAINER_EE72B9D8_E36F_48F2_BF16_F9AF024EAE2A
#define HEADER_CONTAINER_EE72B9D8_E36F_48F2_BF16_F9AF024EAE2A


#include "iridium/smart_ptr.h"


namespace iridium {
namespace pattern {


template<typename TItem>
class IContainer {
public:
    DEFINE_INTERFACE(IContainer)
    virtual void add(TItem const &item) = 0;
    virtual void del(TItem const &item) = 0;
};


} // pattern
} // iridium


#endif // HEADER_CONTAINER_EE72B9D8_E36F_48F2_BF16_F9AF024EAE2A
