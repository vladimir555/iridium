// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CLIENT_E242039D_4C9B_4227_97AA_5179FBC55F33
#define HEADER_CLIENT_E242039D_4C9B_4227_97AA_5179FBC55F33


#include "iridium/pattern/initializable.h"


namespace iridium {
namespace io {


class IClient: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IClient)
};


} // io
} // iridium


#endif // HEADER_CLIENT_E242039D_4C9B_4227_97AA_5179FBC55F33
