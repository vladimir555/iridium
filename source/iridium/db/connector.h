// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
#define HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C


#include "iridium/pattern/initializable.h"
#include "iridium/parsing/node.h"

#include <stdexcept>
#include <string>
#include <vector>


namespace iridium::db {


/// \~english @brief Represents an exception specific to database operations.
/// \~russian @brief Представляет исключение, специфичное для операций с базами данных.
class Exception : public std::runtime_error {
public:
    /// \~english @brief Construct a new Exception object.
    /// \~russian @brief Создает новый объект исключения.
    /// \~english @param e The error message.
    /// \~russian @param e Сообщение об ошибке.
    explicit Exception(std::string const &e);
    virtual ~Exception() = default;
};

/// \~english @brief An interface for a database connector.
/// \~english This interface defines the contract for classes that provide a connection to a database.
/// \~english It includes methods for sending queries and processing results.
/// \~russian @brief Интерфейс для коннектора к базе данных.
/// \~russian Этот интерфейс определяет контракт для классов, обеспечивающих подключение к базе данных.
/// \~russian Он включает методы для отправки запросов и обработки результатов.
class IConnector: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IConnector)
    using INode = parsing::INode;
    virtual INode::TSharedPtr sendQuery(std::string const &query) = 0;

    static std::vector<std::string> makeFields(parsing::INode::TSharedPtr const &rows);
    static std::string const FIELD_NAME_AFFECTED_ROWS;
};


} // namespace iridium::db


#endif // HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
