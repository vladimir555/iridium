// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Implements the `CConnector` class.
/// \~russian @file
/// @brief Реализует класс `CConnector`.

#include "connector.h"

namespace iridium::db::implementation {

CConnector::CConnector(config::TDatebase const &config)
:
    m_config(config)
{}

} // iridium::db::implementation
