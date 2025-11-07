// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONFIG_004DD24D_1205_412D_8CEF_618368AB483A
#define HEADER_CONFIG_004DD24D_1205_412D_8CEF_618368AB483A


#include <string>

#include "event.h"
#include "sink.h"

#include "iridium/parsing/serialization/node.h"
#include "iridium/io/uri.h"


namespace iridium {
namespace logging {
namespace config {


/// \~english @brief Defines the root structure for logger configuration.
///     This structure encapsulates all settings for the logging system,
///     including global settings and a list of sink configurations.
/// \~russian @brief Определяет корневую структуру для конфигурации логгера.
///     Эта структура инкапсулирует все настройки для системы логирования,
///     включая глобальные параметры и список конфигураций приемников (sinks).
    DEFINE_ROOT_NODE_BEGIN(Logger)
        /// \~english @brief Specifies whether to use GMT (UTC) time for log timestamps. If false, local time is used.
        /// \~russian @brief Указывает, использовать ли время GMT (UTC) для временных меток логов. Если false, используется локальное время.
        DEFINE_ATTRIBUTE(bool, IsGmtTime, false)
        /// \~english @brief Default logging level for the logger. Sinks can override this level.
        /// \~russian @brief Уровень логирования по умолчанию для логгера. Приемники могут переопределять этот уровень.
        DEFINE_ATTRIBUTE(TEvent::TLevel, Level, TEvent::TLevel::TRACE)
        /// \~english @brief Defines a list of log sinks, each with its own specific configuration.
        /// \~russian @brief Определяет список приемников логов, каждый со своей специфической конфигурацией.
        DEFINE_NODE_LIST_BEGIN(Sink)
            // todo: ratate (none, hourly, dayly, weekly, onstart, continuously) + url (file://...  , postgres:// ...)
            /// \~english @brief Enumeration for supported logging sink types.
            /// \~russian @brief Перечисление для поддерживаемых типов приемников логирования.
            DEFINE_ENUM(
                TSinkType,
                /// \~english @brief Console sink (e.g., stdout, stderr).
                /// \~russian @brief Консольный приемник (например, stdout, stderr).
                CONSOLE,
                /// \~english @brief Simple file sink. Logs to a single file.
                /// \~russian @brief Простой файловый приемник. Логирует в один файл.
                FILE,
                /// \~english @brief Daily rotating file sink. Creates a new log file each day.
                /// \~russian @brief Файловый приемник с ежедневной ротацией. Создает новый лог-файл каждый день.
                FILE_DAILY, // rotate daily
                /// \~english @brief Timely rotating file sink. Creates a new log file every time the logger starts.
                /// \~russian @brief Файловый приемник с ротацией по времени запуска. Создает новый лог-файл при каждом запуске логгера.
                FILE_TIMELY) // new file every logger starting
            /// \~english @brief The type of this logging sink (e.g., CONSOLE, FILE).
            /// \~russian @brief Тип этого приемника логирования (например, CONSOLE, FILE).
            DEFINE_ATTRIBUTE(TSinkType      , Type      , TSinkType::UNKNOWN)
            /// \~english @brief Specifies whether this sink operates in asynchronous mode.
            /// \~russian @brief Указывает, работает ли этот приемник в асинхронном режиме.
            DEFINE_ATTRIBUTE(bool           , IsAsync   , false)
            /// \~english @brief URI for sink configuration. For file sinks, this is typically the file path. For other types, it might be different (e.g., connection string).
            /// \~russian @brief URI для конфигурации приемника. Для файловых приемников это обычно путь к файлу. Для других типов может отличаться (например, строка подключения).
            DEFINE_ATTRIBUTE(std::string    , Uri       , "")
            /// \~english @brief Specific logging level for this sink. If set to UNKNOWN, it inherits the logger's default level. Otherwise, this overrides the default level for this sink.
            /// \~russian @brief Специфический уровень логирования для этого приемника. Если установлено значение UNKNOWN, он наследует уровень логгера по умолчанию. В противном случае, это значение переопределяет уровень по умолчанию для данного приемника.
            DEFINE_ATTRIBUTE(TEvent::TLevel , Level     , TEvent::TLevel::UNKNOWN)
        DEFINE_NODE_LIST_END(Sink)
    DEFINE_ROOT_NODE_END()

/// \~english @brief Creates a default logger configuration.
///     This configuration typically includes a console sink with a default logging level.
/// \~russian @brief Создает конфигурацию логгера по умолчанию.
///     Эта конфигурация обычно включает консольный приемник с уровнем логирования по умолчанию.
/// \~english @return A `TLogger` structure populated with default settings.
/// \~russian @return Структура `TLogger`, заполненная настройками по умолчанию.
TLogger createDefault();


} // config
} // logging
} // iridium


//DEFINE_ENUM_CONVERT(iridium::logging::config::TLogger::TSink::TSinkType)


#endif // HEADER_CONFIG_004DD24D_1205_412D_8CEF_618368AB483A
