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


/// \~english @brief Base class for database-related exceptions.
/// \~russian @brief Базовый класс для исключений, связанных с базой данных.
class Exception : public std::runtime_error {
public:
    explicit Exception(std::string const &e);
    virtual ~Exception() = default;
};


/// \~english @brief Interface for database connectors.
/// \~russian @brief Интерфейс для коннекторов баз данных.
class IConnector: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IConnector)
    using INode = parsing::INode;
    /// \~english @brief Sends a SQL query to the database and returns the result as a node tree.
    /// \~russian @brief Отправляет SQL-запрос в базу данных и возвращает результат в виде дерева узлов.
    virtual INode::TSharedPtr sendQuery(std::string const &query) = 0;

    /// \~english @brief Extracts field names from the result rows.
    /// \~russian @brief Извлекает имена полей из строк результата.
    static std::vector<std::string> makeFields(parsing::INode::TSharedPtr const &rows);
    /// \~english @brief Field name used for representing affected rows count in query results.
    /// \~russian @brief Имя поля, используемое для представления количества затронутых строк в результатах запроса.
    static std::string const FIELD_NAME_AFFECTED_ROWS;
};


} // namespace iridium::db


#endif // HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
