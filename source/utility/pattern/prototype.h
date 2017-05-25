#ifndef HEADER_PROTOTYPE_248BDC92_79E1_458D_85F8_A8E49C19564B
#define HEADER_PROTOTYPE_248BDC92_79E1_458D_85F8_A8E49C19564B


#include "utility/smart_ptr.h"


namespace utility {
namespace pattern {


/// prototype pattern
template<typename TSubject>
class IPrototype {
public:
    DEFINE_SMART_PTR(IPrototype<TSubject>);
    /// default destructor
    virtual ~IPrototype() = default;
    /// 
    virtual std::shared_ptr<TSubject> clone() const = 0;
};


} // pattern
} // utility


#endif // HEADER_PROTOTYPE_248BDC92_79E1_458D_85F8_A8E49C19564B
