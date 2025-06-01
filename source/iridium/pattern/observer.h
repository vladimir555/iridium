// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_OBSERVER_D355605F_6C18_40F5_A7C1_509CA5AA9BC6
#define HEADER_OBSERVER_D355605F_6C18_40F5_A7C1_509CA5AA9BC6


#include "iridium/smart_ptr.h"


namespace iridium {
namespace pattern {


template <typename TEvent>
class IObserver {
public:
    DEFINE_INTERFACE(IObserver)
    virtual void handleEvent(TEvent const &event) = 0;
};


} // pattern
} // iridium


#endif // HEADER_OBSERVER_D355605F_6C18_40F5_A7C1_509CA5AA9BC6
