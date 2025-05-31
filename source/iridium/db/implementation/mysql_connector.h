// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155
#define HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155


#include "iridium/build_flags.h"

/// \~english @brief This entire header defines the MySQL connector implementation, compiled only if `BUILD_FLAG_MYSQL` is defined.
/// \~russian @brief Весь этот заголовочный файл определяет реализацию коннектора MySQL, компилируемую только если определен макрос `BUILD_FLAG_MYSQL`.
#ifdef BUILD_FLAG_MYSQL


#include "iridium/smart_ptr.h"
#include "iridium/db/connector.h"
#include "iridium/io/uri.h"

#include "connector.h" // For CConnector base

#include <string>
#include <mysql.h> // MySQL C API header


namespace iridium {
namespace db {
namespace implementation {


/// \~english @brief Concrete implementation of a database connector for MySQL.
///     This class implements the `IConnector` interface and uses the `CConnector` base class
///     to provide connectivity and query execution capabilities for MySQL databases.
///     It utilizes the MySQL C API for database interactions.
/// \~russian @brief Конкретная реализация коннектора базы данных для MySQL.
///     Этот класс реализует интерфейс `IConnector` и использует базовый класс `CConnector`
///     для предоставления возможностей подключения и выполнения запросов для баз данных MySQL.
///     Он использует MySQL C API для взаимодействия с базой данных.
class CMySQLConnector:
    public IConnector,
    public CConnector
{
public:
    /// \~english @brief Macro likely defining a static `create` method or similar factory functionality
    ///     for creating shared pointers (`TSharedPtr`) to `CMySQLConnector` instances.
    /// \~russian @brief Макрос, вероятно, определяющий статический метод `create` или аналогичную фабричную функциональность
    ///     для создания умных указателей (`TSharedPtr`) на экземпляры `CMySQLConnector`.
    DEFINE_CREATE(CMySQLConnector)

    /// \~english @brief Constructs a `CMySQLConnector` instance with the specified database configuration.
    ///     Initializes the base `CConnector` with the configuration. The actual database connection
    ///     is typically established in the `initialize()` method.
    /// \~russian @brief Конструирует экземпляр `CMySQLConnector` с указанной конфигурацией базы данных.
    ///     Инициализирует базовый `CConnector` с конфигурацией. Фактическое соединение с базой данных
    ///     обычно устанавливается в методе `initialize()`.
    /// \~english @param config The database configuration object (`config::TDatebase`) containing connection details for MySQL.
    /// \~russian @param config Объект конфигурации базы данных (`config::TDatebase`), содержащий детали подключения для MySQL.
    CMySQLConnector(config::TDatebase const &config);

    /// \~english @brief Virtual destructor. Ensures the MySQL connection is properly closed upon destruction.
    ///     Typically calls `finalize()` if the connection is still open.
    /// \~russian @brief Виртуальный деструктор. Гарантирует надлежащее закрытие соединения MySQL при уничтожении объекта.
    ///     Обычно вызывает `finalize()`, если соединение все еще открыто.
    virtual        ~CMySQLConnector();

    /// \~english @brief Initializes the MySQL connector by establishing a connection to the database.
    ///     Uses the parameters from the `m_config` member (e.g., host, user, password, database, port)
    ///     to connect to the MySQL server via the MySQL C API.
    /// \~russian @brief Инициализирует коннектор MySQL, устанавливая соединение с базой данных.
    ///     Использует параметры из члена `m_config` (например, хост, пользователь, пароль, база данных, порт)
    ///     для подключения к серверу MySQL через MySQL C API.
    /// \~english @throws iridium::db::Exception if the connection fails or configuration is invalid.
    /// \~russian @throws iridium::db::Exception в случае сбоя подключения или неверной конфигурации.
    virtual void    initialize() override;

    /// \~english @brief Finalizes the MySQL connector by closing the established database connection.
    ///     Releases any resources associated with the MySQL connection.
    /// \~russian @brief Завершает работу коннектора MySQL, закрывая установленное соединение с базой данных.
    ///     Освобождает все ресурсы, связанные с соединением MySQL.
    /// \~english @throws iridium::db::Exception if errors occur during disconnection.
    /// \~russian @throws iridium::db::Exception в случае ошибок при отключении.
    virtual void    finalize() override;

    /// \~english @brief Sends an SQL query to the connected MySQL database for execution.
    ///     Uses the MySQL C API to execute the query. Parses the results from MySQL
    ///     into the generic `TRows` format. For DML/DDL statements, it may populate
    ///     `TRows` with affected row count information using `FIELD_NAME_AFFECTED_ROWS`.
    /// \~russian @brief Отправляет SQL-запрос в подключенную базу данных MySQL для выполнения.
    ///     Использует MySQL C API для выполнения запроса. Разбирает результаты от MySQL
    ///     в общий формат `TRows`. Для операторов DML/DDL он может заполнять
    ///     `TRows` информацией о количестве затронутых строк, используя `FIELD_NAME_AFFECTED_ROWS`.
    /// \~english @param query The SQL query string to execute.
    /// \~russian @param query Строка SQL-запроса для выполнения.
    /// \~english @return A `TRows` object containing the result set of the query, or information about affected rows.
    /// \~russian @return Объект `TRows`, содержащий результирующий набор запроса или информацию о затронутых строках.
    /// \~english @throws iridium::db::Exception if the query execution fails or if the connector is not initialized.
    /// \~russian @throws iridium::db::Exception в случае сбоя выполнения запроса или если коннектор не инициализирован.
    virtual TRows   sendQuery(std::string const &query) override;

private:
    /// \~english @brief The MySQL connection handle.
    ///     This is the core C API structure representing the connection to the MySQL server.
    ///     It is initialized by `mysql_init()` and used in subsequent MySQL API calls.
    /// \~russian @brief Хэндл соединения MySQL.
    ///     Это основная структура C API, представляющая соединение с сервером MySQL.
    ///     Она инициализируется функцией `mysql_init()` и используется в последующих вызовах MySQL API.
    ::MYSQL         m_connection;
};


} // implementation
} // db
} // iridium


#endif // BUILD_FLAG_MYSQL


#endif // HEADER_MYSQL_CONNECTOR_1DD931FC_0862_4845_8CB9_DF0E40666155
