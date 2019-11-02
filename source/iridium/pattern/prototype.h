// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROTOTYPE_248BDC92_79E1_458D_85F8_A8E49C19564B
#define HEADER_PROTOTYPE_248BDC92_79E1_458D_85F8_A8E49C19564B


#include "iridium/smart_ptr.h"


namespace iridium {
namespace pattern {


/// prototype pattern
template<typename TSubject>
class IPrototype {
public:
    DEFINE_SMART_PTR(IPrototype<TSubject>)
    /// default destructor
    virtual ~IPrototype() = default;
    /// 
    virtual std::shared_ptr<TSubject> clone() const = 0;
};


} // pattern
} // iridium


#endif // HEADER_PROTOTYPE_248BDC92_79E1_458D_85F8_A8E49C19564B
