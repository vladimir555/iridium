// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Implements the `IConnector` interface.
/// \~russian @file
/// @brief Реализует интерфейс `IConnector`.

#include "connector.h"

using iridium::db::IConnector;
using std::vector;
using std::string;

namespace iridium::db {

string const iridium::db::IConnector::FIELD_NAME_AFFECTED_ROWS = "affected_rows";

iridium::db::Exception::Exception(std::string const &e)
:
    std::runtime_error(e.c_str())
{}

vector<string> IConnector::makeFields(INode::TSharedPtr const &rows) {
    vector<string> fields;

    if (rows && rows->size() > 0) {
        for (auto const &field: *rows)
            fields.push_back(field->getName());
    }

    return fields; // ----->
}

} // namespace iridium::db
