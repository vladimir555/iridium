// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
#define HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD


#include "mutex.h"

#include "implementation/mutex.h"
#include "implementation/condition.h"


namespace iridium {
namespace threading {


class Synchronized {
protected:
    friend class SynchronizedScope;
    friend class SynchronizedScopeSimple;

    Synchronized(IMutex::TSharedPtr const &mutex);
    Synchronized();
   ~Synchronized() = default;

    IMutex::TSharedPtr      getMutex() const;
    ICondition::TSharedPtr  getCondition() const;

private:
    IMutex::TSharedPtr      m_mutex;
};


} // threading
} // iridium


#endif // HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
