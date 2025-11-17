// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
#define HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08


#include "iridium/smart_ptr.h"


namespace iridium::pattern {


class IInitializable {
public:
    DEFINE_SMART_PTR(IInitializable)
    virtual ~IInitializable() = default;

    virtual void initialize() = 0;
    virtual void finalize() = 0;
};


} // namespace iridium::pattern


#endif // HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
