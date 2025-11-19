#pragma once


#include "config.h"
#include "connector.h"
#include "iridium/io/uri.h"


namespace iridium::db {


/// \~english @brief Creates a database configuration object from a URI.
/// \~english     Parses the given URI and extracts database connection parameters (like type, host, port, user, password, database name)
/// \~english     to populate a `config::TDatebase` object.
/// \~russian @brief Создает объект конфигурации базы данных из URI.
/// \~russian     Разбирает данный URI и извлекает параметры подключения к базе данных (такие как тип, хост, порт, пользователь, пароль, имя базы данных)
/// \~russian      для заполнения объекта `config::TDatebase`.
/// \~english @param uri The URI containing database connection information.
/// \~english     The URI scheme might determine the database type (e.g., "mysql://", "postgres://").
/// \~russian @param uri URI, содержащий информацию о подключении к базе данных.
/// \~russian     Схема URI может определять тип базы данных (например, "mysql://", "postgres://").
/// \~english @return A `config::TDatebase` object populated with parameters from the URI.
/// \~russian @return Объект `config::TDatebase`, заполненный параметрами из URI.
/// \~english @throws iridium::db::Exception or other exceptions if URI parsing fails or scheme is unsupported.
/// \~russian @throws iridium::db::Exception или другие исключения, если разбор URI не удался или схема не поддерживается.
config::TDatebase makeConfig(io::URI const &uri);


/// \~english @brief Creates a database connector instance based on the provided configuration.
/// \~english     This function acts as a factory to instantiate a concrete `IConnector` implementation
/// \~english     (e.g., a MySQL connector, PostgreSQL connector) suitable for the given configuration.
/// \~russian @brief Создает экземпляр коннектора базы данных на основе предоставленной конфигурации.
/// \~russian     Эта функция действует как фабрика для создания конкретной реализации `IConnector`
/// \~russian     (например, коннектора MySQL, PostgreSQL), подходящей для данной конфигурации.
/// \~english @param config The database configuration object (`config::TDatebase`) specifying connection parameters.
/// \~english     The `Type` field in the config determines which type of connector is created.
/// \~russian @param config Объект конфигурации базы данных (`config::TDatebase`), указывающий параметры подключения.
/// \~russian     Поле `Type` в конфигурации определяет, какой тип коннектора будет создан.
/// \~english @return A shared pointer (`IConnector::TSharedPtr`) to an `IConnector` instance.
/// \~english     Returns a null pointer or throws an exception if a connector cannot be created for the given configuration.
/// \~russian @return Умный указатель (`IConnector::TSharedPtr`) на экземпляр `IConnector`.
/// \~russian     Возвращает нулевой указатель или выбрасывает исключение, если коннектор не может быть создан для данной конфигурации.
/// \~english @throws iridium::db::Exception or other exceptions on failure to create a connector.
/// \~russian @throws iridium::db::Exception или другие исключения при ошибке создания коннектора.
IConnector::TSharedPtr createConnector(config::TDatebase    const &config);

/// \~english @brief Creates a database connector instance from a URI.
/// \~english     This is a convenience function that first calls `makeConfig(uri)` to create a configuration
/// \~english     object from the URI, and then calls `createConnector(config)` with that configuration.
/// \~russian @brief Создает экземпляр коннектора базы данных из URI.
/// \~russian     Это вспомогательная функция, которая сначала вызывает `makeConfig(uri)` для создания объекта конфигурации
/// \~russian     из URI, а затем вызывает `createConnector(config)` с этой конфигурацией.
/// \~english @param uri The URI containing database connection information.
/// \~russian @param uri URI, содержащий информацию о подключении к базе данных.
/// \~english @return A shared pointer (`IConnector::TSharedPtr`) to an `IConnector` instance.
/// \~english     Returns a null pointer or throws an exception if a connector cannot be created.
/// \~russian @return Умный указатель (`IConnector::TSharedPtr`) на экземпляр `IConnector`.
/// \~russian     Возвращает нулевой указатель или выбрасывает исключение, если коннектор не может быть создан.
/// \~english @throws iridium::db::Exception or other exceptions if URI parsing or connector creation fails.
/// \~russian @throws iridium::db::Exception или другие исключения, если разбор URI или создание коннектора не удалось.
IConnector::TSharedPtr createConnector(io::URI              const &uri);


} // namespace iridium::db
