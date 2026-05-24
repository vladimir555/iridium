// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
#define HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C


/// \~english @file
/// @brief Defines the `IConnector` interface and database-specific exceptions.
/// \~russian @file
/// @brief Определяет интерфейс `IConnector` и специфичные для баз данных исключения.


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

    /// \~english @brief Sends a query to the database.
    /// \~russian @brief Отправляет запрос к базе данных.
    /// \~english @param query The SQL query to send.
    /// \~russian @param query SQL-запрос для отправки.
    /// \~english @return A shared pointer to the root node of the result set.
    /// \~russian @return Умный указатель на корневой узел результирующего набора.
    virtual INode::TSharedPtr sendQuery(std::string const &query) = 0;

    /// \~english @brief Extracts the field names from a result set.
    /// \~russian @brief Извлекает имена полей из результирующего набора.
    /// \~english @param rows The root node of the result set.
    /// \~russian @param rows Корневой узел результирующего набора.
    /// \~english @return A vector of field names.
    /// \~russian @return Вектор имен полей.
    static std::vector<std::string> makeFields(parsing::INode::TSharedPtr const &rows);

    /// \~english @brief The name of the field that contains the number of affected rows.
    /// \~russian @brief Имя поля, содержащего количество затронутых строк.
    static std::string const FIELD_NAME_AFFECTED_ROWS;
};


} // namespace iridium::db


#endif // HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
