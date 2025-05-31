// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
#define HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C


#include "iridium/pattern/initializable.h"
//#include "iridium/caching/cached_value.h" // Commented out include

#include <stdexcept>
#include <string>
#include <vector>
#include <map>


namespace iridium {
/// \~english @brief Namespace for database related functionalities such as connection and querying.
/// \~russian @brief Пространство имен для функциональности, связанной с базами данных, такой как соединение и выполнение запросов.
namespace db {


/// \~english @brief Custom exception class for database-related errors.
///     Derives from `std::runtime_error` to provide a standard way of reporting errors
///     encountered during database operations.
/// \~russian @brief Пользовательский класс исключений для ошибок, связанных с базами данных.
///     Наследуется от `std::runtime_error` для предоставления стандартного способа сообщения
///     об ошибках, возникших во время операций с базой данных.
class Exception : public std::runtime_error {
public:
    /// \~english @brief Constructs an Exception object with a specific error message.
    /// \~russian @brief Конструирует объект Exception с определенным сообщением об ошибке.
    /// \~english @param e The error message string.
    /// \~russian @param e Строка сообщения об ошибке.
    explicit Exception(std::string const &e);

    /// \~english @brief Default virtual destructor.
    /// \~russian @brief Виртуальный деструктор по умолчанию.
    virtual ~Exception() = default;
};


/// \~english @brief Interface for a database connector.
///     Defines the contract for classes that provide connectivity to a database
///     and allow for query execution. It inherits from `iridium::pattern::IInitializable`,
///     suggesting that connectors need to be initialized before use (e.g., with configuration).
/// \~russian @brief Интерфейс для коннектора базы данных.
///     Определяет контракт для классов, обеспечивающих подключение к базе данных
///     и позволяющих выполнять запросы. Наследуется от `iridium::pattern::IInitializable`,
///     что предполагает необходимость инициализации коннекторов перед использованием (например, с конфигурацией).
class IConnector: public pattern::IInitializable {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IConnector)

    /// \~english @brief Represents a single row of data retrieved from the database.
    ///     It's a map where keys are column names (as strings) and values are the corresponding
    ///     cell values (also as strings).
    /// \~russian @brief Представляет одну строку данных, полученную из базы данных.
    ///     Это карта, где ключами являются имена столбцов (в виде строк), а значениями -
    ///     соответствующие значения ячеек (также в виде строк).
    typedef std::map<std::string, std::string> TRow;

    /// \~english @brief Represents a collection of rows (a dataset) retrieved from the database.
    ///     It's a vector of `TRow` objects.
    /// \~russian @brief Представляет коллекцию строк (набор данных), полученную из базы данных.
    ///     Это вектор объектов `TRow`.
    typedef std::vector< TRow > TRows;

    /// \~english @brief Sends a query string to the database for execution.
    ///     This is a pure virtual method that must be implemented by concrete connector classes.
    ///     It's responsible for executing SQL queries, DML, DDL, or other database commands.
    /// \~russian @brief Отправляет строку запроса в базу данных для выполнения.
    ///     Это чисто виртуальный метод, который должен быть реализован конкретными классами коннекторов.
    ///     Он отвечает за выполнение SQL-запросов, DML, DDL или других команд базы данных.
    /// \~english @param query The SQL query or database command to be executed.
    /// \~russian @param query SQL-запрос или команда базы данных для выполнения.
    /// \~english @return A `TRows` object containing the results of the query. For DML/DDL statements
    ///     that do not return a dataset, the returned `TRows` might be empty or contain
    ///     information like affected row counts (see `FIELD_NAME_AFFECTED_ROWS`).
    /// \~russian @return Объект `TRows`, содержащий результаты запроса. Для операторов DML/DDL,
    ///     которые не возвращают набор данных, возвращаемый `TRows` может быть пустым или содержать
    ///     информацию, такую как количество затронутых строк (см. `FIELD_NAME_AFFECTED_ROWS`).
    /// \~english @throws iridium::db::Exception or derived classes if a database error occurs.
    /// \~russian @throws iridium::db::Exception или производные классы в случае ошибки базы данных.
    virtual TRows sendQuery(std::string const &query) = 0;

    /// \~english @brief A utility function to extract unique field (column) names from a dataset.
    ///     It iterates through the first row of the provided `TRows` (if not empty)
    ///     and collects all distinct column names.
    /// \~russian @brief Вспомогательная функция для извлечения уникальных имен полей (столбцов) из набора данных.
    ///     Она итерирует по первой строке предоставленного `TRows` (если он не пуст)
    ///     и собирает все различные имена столбцов.
    /// \~english @param rows The dataset (vector of TRow) from which to extract field names.
    /// \~russian @param rows Набор данных (вектор TRow), из которого извлекаются имена полей.
    /// \~english @return A vector of strings, where each string is a unique field name found in the dataset.
    ///     The order might depend on the map iteration order of the first row.
    /// \~russian @return Вектор строк, где каждая строка - это уникальное имя поля, найденное в наборе данных.
    ///     Порядок может зависеть от порядка итерации по карте первой строки.
    static std::vector<std::string> makeFields(TRows const &rows);

    /// \~english @brief A constant string representing the field name used to report the number of affected rows.
    ///     After DML operations (INSERT, UPDATE, DELETE), some database connectors might return
    ///     this information as a pseudo-column in the result set.
    /// \~russian @brief Константная строка, представляющая имя поля, используемое для сообщения о количестве затронутых строк.
    ///     После операций DML (INSERT, UPDATE, DELETE) некоторые коннекторы баз данных могут возвращать
    ///     эту информацию как псевдо-столбец в результирующем наборе.
    static std::string const FIELD_NAME_AFFECTED_ROWS;
};


} // db
} // iridium


#endif // HEADER_CONNECTOR_D4211A6B_FEA8_449C_9D30_6F854241610C
