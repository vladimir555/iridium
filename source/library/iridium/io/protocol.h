// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROTOCOL_1092ADC0_796B_48CD_A2C7_B461837C9682
#define HEADER_PROTOCOL_1092ADC0_796B_48CD_A2C7_B461837C9682


#include "event.h"


namespace iridium {
namespace io {


class IProtocol {
public:
    DEFINE_INTERFACE(IProtocol)
    // returns false - need rm context
    virtual bool control(
        TEvent::TSharedPtr
            const &event) = 0;
};


} // io
} // iridium


#endif // HEADER_PROTOCOL_1092ADC0_796B_48CD_A2C7_B461837C9682
