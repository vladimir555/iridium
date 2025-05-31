// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C
#define HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C


#include "iridium/build_flags.h"

/// \~english @brief This entire header defines the PostgreSQL connector implementation, compiled only if `BUILD_FLAG_POSTGRES` is defined.
/// \~russian @brief Весь этот заголовочный файл определяет реализацию коннектора PostgreSQL, компилируемую только если определен макрос `BUILD_FLAG_POSTGRES`.
#ifdef BUILD_FLAG_POSTGRES


#include "iridium/smart_ptr.h"
#include "iridium/db/connector.h"
#include "iridium/io/uri.h"

#include "connector.h" // For CConnector base

#include <string>
#include <libpq-fe.h> // PostgreSQL C API header


namespace iridium {
namespace db {
namespace implementation {


/// \~english @brief Concrete implementation of a database connector for PostgreSQL.
///     This class implements the `IConnector` interface and uses the `CConnector` base class
///     to provide connectivity and query execution capabilities for PostgreSQL databases.
///     It utilizes the `libpq` C library for database interactions.
/// \~russian @brief Конкретная реализация коннектора базы данных для PostgreSQL.
///     Этот класс реализует интерфейс `IConnector` и использует базовый класс `CConnector`
///     для предоставления возможностей подключения и выполнения запросов для баз данных PostgreSQL.
///     Он использует C-библиотеку `libpq` для взаимодействия с базой данных.
class CPostgresConnector:
    public IConnector,
    public CConnector
{
public:
    /// \~english @brief Macro likely defining a static `create` method or similar factory functionality
    ///     for creating shared pointers (`TSharedPtr`) to `CPostgresConnector` instances.
    /// \~russian @brief Макрос, вероятно, определяющий статический метод `create` или аналогичную фабричную функциональность
    ///     для создания умных указателей (`TSharedPtr`) на экземпляры `CPostgresConnector`.
    DEFINE_CREATE(CPostgresConnector)

    /// \~english @brief Constructs a `CPostgresConnector` instance with the specified database configuration.
    ///     Initializes the base `CConnector` with the configuration. The actual database connection
    ///     is typically established in the `initialize()` method. The connection handle `m_connection` is initialized to `nullptr`.
    /// \~russian @brief Конструирует экземпляр `CPostgresConnector` с указанной конфигурацией базы данных.
    ///     Инициализирует базовый `CConnector` с конфигурацией. Фактическое соединение с базой данных
    ///     обычно устанавливается в методе `initialize()`. Хэндл соединения `m_connection` инициализируется `nullptr`.
    /// \~english @param config The database configuration object (`config::TDatebase`) containing connection details for PostgreSQL.
    /// \~russian @param config Объект конфигурации базы данных (`config::TDatebase`), содержащий детали подключения для PostgreSQL.
    CPostgresConnector(config::TDatebase const &config);

    /// \~english @brief Virtual destructor. Ensures the PostgreSQL connection is properly closed upon destruction.
    ///     Calls `finalize()` to release the connection if it's still active.
    /// \~russian @brief Виртуальный деструктор. Гарантирует надлежащее закрытие соединения PostgreSQL при уничтожении объекта.
    ///     Вызывает `finalize()` для освобождения соединения, если оно все еще активно.
    virtual        ~CPostgresConnector() override;

    /// \~english @brief Initializes the PostgreSQL connector by establishing a connection to the database.
    ///     Uses the parameters from the `m_config` member (e.g., host, user, password, database name, port)
    ///     to construct a connection string and connect to the PostgreSQL server via `PQconnectdb`.
    /// \~russian @brief Инициализирует коннектор PostgreSQL, устанавливая соединение с базой данных.
    ///     Использует параметры из члена `m_config` (например, хост, пользователь, пароль, имя базы данных, порт)
    ///     для формирования строки подключения и соединения с сервером PostgreSQL через `PQconnectdb`.
    /// \~english @throws iridium::db::Exception if the connection fails (e.g., `PQstatus` is `CONNECTION_BAD`) or configuration is invalid.
    /// \~russian @throws iridium::db::Exception в случае сбоя подключения (например, `PQstatus` равен `CONNECTION_BAD`) или неверной конфигурации.
    virtual void    initialize() override;

    /// \~english @brief Finalizes the PostgreSQL connector by closing the established database connection.
    ///     Calls `PQfinish` to close the connection if it is open and resets the connection handle.
    /// \~russian @brief Завершает работу коннектора PostgreSQL, закрывая установленное соединение с базой данных.
    ///     Вызывает `PQfinish` для закрытия соединения, если оно открыто, и сбрасывает хэндл соединения.
    virtual void    finalize() override;

    /// \~english @brief Sends an SQL query to the connected PostgreSQL database for execution.
    ///     Uses `PQexec` from the `libpq` C API to execute the query. Parses the `PGresult`
    ///     into the generic `TRows` format. For DML/DDL statements, it may populate
    ///     `TRows` with affected row count information using `FIELD_NAME_AFFECTED_ROWS` by checking `PQcmdTuples`.
    /// \~russian @brief Отправляет SQL-запрос в подключенную базу данных PostgreSQL для выполнения.
    ///     Использует `PQexec` из C API `libpq` для выполнения запроса. Разбирает `PGresult`
    ///     в общий формат `TRows`. Для операторов DML/DDL он может заполнять
    ///     `TRows` информацией о количестве затронутых строк, используя `FIELD_NAME_AFFECTED_ROWS`, проверяя `PQcmdTuples`.
    /// \~english @param query The SQL query string to execute.
    /// \~russian @param query Строка SQL-запроса для выполнения.
    /// \~english @return A `TRows` object containing the result set of the query, or information about affected rows.
    /// \~russian @return Объект `TRows`, содержащий результирующий набор запроса или информацию о затронутых строках.
    /// \~english @throws iridium::db::Exception if the query execution fails (e.g., result status is not `PGRES_COMMAND_OK` or `PGRES_TUPLES_OK`) or if the connector is not initialized.
    /// \~russian @throws iridium::db::Exception в случае сбоя выполнения запроса (например, статус результата не `PGRES_COMMAND_OK` или `PGRES_TUPLES_OK`) или если коннектор не инициализирован.
    virtual TRows   sendQuery(std::string const &query) override;

private:
    /// \~english @brief Executes a PostgreSQL command that does not return a result set.
    ///     This is a helper method typically used for commands like SET, BEGIN, COMMIT, ROLLBACK.
    ///     It uses `PQexec` and checks for successful command completion.
    /// \~russian @brief Выполняет команду PostgreSQL, которая не возвращает результирующий набор.
    ///     Это вспомогательный метод, обычно используемый для таких команд, как SET, BEGIN, COMMIT, ROLLBACK.
    ///     Он использует `PQexec` и проверяет успешное завершение команды.
    /// \~english @param command The SQL command string to execute.
    /// \~russian @param command Строка SQL-команды для выполнения.
    /// \~english @throws iridium::db::Exception if the command execution fails.
    /// \~russian @throws iridium::db::Exception в случае сбоя выполнения команды.
    void            executeCommand(std::string const &command);

    /// \~english @brief The PostgreSQL connection handle.
    ///     This pointer (`PGconn*`) is obtained from `PQconnectdb` and represents the active
    ///     connection to the PostgreSQL server. It is `nullptr` if no connection is established.
    /// \~russian @brief Хэндл соединения PostgreSQL.
    ///     Этот указатель (`PGconn*`) получается из `PQconnectdb` и представляет активное
    ///     соединение с сервером PostgreSQL. Он равен `nullptr`, если соединение не установлено.
    PGconn         *m_connection;
};


} // implementation
} // db
} // iridium


#endif // BUILD_FLAG_POSTGRES


#endif // HEADER_POSTGRES_CONNECTOR_750EE9B7_825A_426E_91B4_C860C650CB1C
