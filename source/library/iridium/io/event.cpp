// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "event.h"


using iridium::convertion::convert;


std::string convert_(iridium::io::TEvent const &event) {
    return "{ operation: " + convert<std::string>(event.operation) + ", uri: " + convert<std::string>(event.uri) + " }";
}


// IMPLEMENT_ENUM(iridium::io::TEvent::TOperation);
IMPLEMENT_CONVERT(std::string, iridium::io::TEvent, convert_);
