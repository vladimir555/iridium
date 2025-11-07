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


namespace iridium {
namespace db {


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
    /// \~english @param query The SQL query to be executed.
    /// \~russian @param query SQL-запрос для выполнения.
    /// \~english @return A shared pointer to the root INode of the result set.
    /// \~english         The result is expected to be a tree structure representing the query output.
    /// \~english         For example, a list of rows, where each row is a list of fields.
    /// \~russian @return Умный указатель на корневой узел INode результирующего набора.
    /// \~russian         Результат ожидается в виде древовидной структуры, представляющей вывод запроса.
    /// \~russian         Например, список строк, где каждая строка - это список полей.
    virtual INode::TSharedPtr sendQuery(std::string const &query) = 0;

    /// \~english @brief A helper function to extract field names from a query result.
    /// \~russian @brief Вспомогательная функция для извлечения имен полей из результата запроса.
    /// \~english @param rows The root node of the query result, typically representing a list of rows.
    /// \~russian @param rows Корневой узел результата запроса, обычно представляющий список строк.
    /// \~english @return A vector of strings containing the field names.
    /// \~russian @return Вектор строк, содержащий имена полей.
    static std::vector<std::string> makeFields(parsing::INode::TSharedPtr const &rows);

    /// \~english @brief The name of the field that contains the number of affected rows
    /// \~english        for queries like INSERT, UPDATE, or DELETE.
    /// \~russian @brief Имя поля, которое содержит количество затронутых строк
    /// \~russian        для запросов типа INSERT, UPDATE или DELETE.
    static std::string const FIELD_NAME_AFFECTED_ROWS;
};


} // db
} // iridium


#endif // HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
