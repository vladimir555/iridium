// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `TEvent` structure for log messages within the Iridium logging framework.
///     This structure encapsulates log event data such as timestamp, level, thread ID, and message content.
/// \~russian @file
/// @brief Определяет структуру `TEvent` для сообщений журнала во фреймворке логирования Iridium.
///     Эта структура инкапсулирует данные события журнала, такие как временная метка, уровень, идентификатор потока и содержимое сообщения.

#ifndef HEADER_EVENT_7923AB72_4A05_48BD_8490_D2C40F047299
#define HEADER_EVENT_7923AB72_4A05_48BD_8490_D2C40F047299


#include <string>

#include "iridium/enum.h"
#include "iridium/smart_ptr.h"


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
/// \~english @brief Contains classes, structures, and enumerations related to the logging framework.
/// \~russian @brief Содержит классы, структуры и перечисления, связанные с фреймворком логирования.
namespace logging {

/// \~english @brief Represents a single log event or message.
///     This structure encapsulates all the information associated with a logging event,
///     such as the timestamp, severity level, thread ID, and the log message itself.
/// \~russian @brief Представляет одно событие или сообщение журнала.
///     Эта структура инкапсулирует всю информацию, связанную с событием логирования,
///     такую как временная метка, уровень серьезности, идентификатор потока и само сообщение журнала.
struct TEvent {
    /// \~english @brief Macro call to define a static `create` method for `TEvent`, enabling shared pointer instantiation.
    /// \~russian @brief Вызов макроса для определения статического метода `create` для `TEvent`, обеспечивающего создание экземпляров через разделяемые указатели.
    DEFINE_CREATE(TEvent)
    /// \~english @brief Enumerates the different severity levels for log events.
    /// \~russian @brief Перечисляет различные уровни серьезности для событий журнала.
    DEFINE_ENUM(
        TLevel,
        /// \~english @brief Trace level, typically for detailed debugging information.
        /// \~russian @brief Уровень трассировки, обычно для подробной отладочной информации.
        TRACE,
        /// \~english @brief Debug level, for debugging information.
        /// \~russian @brief Уровень отладки, для отладочной информации.
        DEBUG,
        /// \~english @brief Informational level, for general operational messages.
        /// \~russian @brief Информационный уровень, для общих сообщений о работе.
        INFO,
        /// \~english @brief Warning level, for potential issues or unusual conditions.
        /// \~russian @brief Уровень предупреждения, для потенциальных проблем или необычных условий.
        WARNING,
        /// \~english @brief Error level, for errors that occurred but might not be fatal. Note the underscore to avoid conflict with potential macros.
        /// \~russian @brief Уровень ошибки, для произошедших ошибок, которые могут не быть фатальными. Обратите внимание на подчеркивание во избежание конфликта с возможными макросами.
        ERROR_,
        /// \~english @brief Fatal error level, for critical errors that might lead to application termination.
        /// \~russian @brief Уровень фатальной ошибки, для критических ошибок, которые могут привести к завершению приложения.
        FATAL
    )
    /// \~english @brief The exact time at which the log event occurred.
    /// \~russian @brief Точное время, когда произошло событие журнала.
    std::chrono::system_clock::time_point const
        time;
    /// \~english @brief The severity level of this log event (e.g., TRACE, INFO, ERROR_).
    /// \~russian @brief Уровень серьезности этого события журнала (например, TRACE, INFO, ERROR_).
    TLevel const
        level;
    /// \~english @brief The identifier of the thread that generated this log event.
    /// \~russian @brief Идентификатор потока, который сгенерировал это событие журнала.
    std::thread::id const
        thread_id;
    /// \~english @brief The actual content of the log message.
    /// \~russian @brief Фактическое содержание сообщения журнала.
    std::string
        line;
};


} // logging
} // iridium


//DEFINE_ENUM_CONVERT(iridium::logging::TEvent::TLevel)


#endif // HEADER_EVENT_7923AB72_4A05_48BD_8490_D2C40F047299
