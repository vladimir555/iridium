// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "process.h"


using iridium::convertion::convert;
using std::string;


string toString(iridium::system::IProcess::TState const &state) {
    return
        string("{ exit_code: ") + convert<string>(state.exit_code) +
        ", condition: "         + convert<string>(state.condition) + " }";
}


// IMPLEMENT_ENUM(iridium::system::IProcess::TState::TCondition)
// IMPLEMENT_ENUM(iridium::system::IProcess::TSignal)
IMPLEMENT_CONVERT(string, iridium::system::IProcess::TState, toString);
