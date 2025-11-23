// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_LOGGER_68035722_1476_4595_9F55_5338D1576CF9
#define HEADER_LOGGER_68035722_1476_4595_9F55_5338D1576CF9


#include "iridium/pattern/singleton.h"

#include "iridium/threading/synchronized.h"
#include "iridium/platform.h"

#include <mutex>
#include <list>
#include <vector>
#include <deque>
#include <array>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include "event.h"
#include "sink.h"
#include "config.h"


namespace iridium::logging {


// todo: wait for finalizing async sinks
/// \~english @brief Main logger class, implemented as a thread-safe singleton.
/// \~english @details This class manages the logging configuration, a collection of sinks, and provides the central point for log message submission.
/// \~russian @brief Основной класс логгера, реализованный как потокобезопасный синглтон.
/// \~russian @details Этот класс управляет конфигурацией логирования, коллекцией приемников и предоставляет центральную точку для отправки лог-сообщений.
class Logger :
    public pattern::Singleton<Logger>,
    public threading::Synchronized<std::mutex>
{
public:
    virtual ~Logger();

    /// \~english @brief Sets the logger's configuration.
    /// \~russian @brief Устанавливает конфигурацию логгера.
    /// \~english @param config The configuration to apply.
    /// \~russian @param config Конфигурация для применения.
    void setConfig(config::TLogger const &config);

    /// \~english @brief Gets the current logger configuration.
    /// \~russian @brief Возвращает текущую конфигурацию логгера.
    /// \~english @return The current configuration.
    /// \~russian @return Текущая конфигурация.
    config::TLogger getConfig();

    /// \~english @brief Logs a log event.
    /// \~russian @brief Логирует событие журнала.
    /// \~english @param event The log event to be processed.
    /// \~russian @param event Событие журнала для обработки.
    void log(TEvent::TConstSharedPtr const &event);

    /// \~english @brief Adds a custom sink to the logger.
    /// \~russian @brief Добавляет пользовательский приемник в логгер.
    /// \~english @param sink The sink to add.
    /// \~russian @param sink Приемник для добавления.
    void addCustomSink(ISink::TSharedPtr const &sink);

private:
    friend class pattern::Singleton<Logger>;
    Logger() = default;

    std::list<ISink::TSharedPtr>    m_sinks;
    parsing::INode::TSharedPtr      m_config;
};

/// \~english @brief A dummy log stream that does nothing. Used to disable logging at compile time.
/// \~russian @brief "Пустой" поток лога, который ничего не делает. Используется для отключения логирования на этапе компиляции.
struct LogStreamDummy
{
    /// \~english @brief Ignores any value streamed to it.
    /// \~russian @brief Игнорирует любое значение, направленное в поток.
    template<typename TValue>
    LogStreamDummy const &operator << (TValue v) const;
};


/// \~english @brief A temporary stream-like object for constructing log messages.
/// \~english @details This object is created by the LOGX macros. It collects the streamed data, and upon destruction, submits the complete log event to the Logger singleton.
/// \-russian @brief Временный потокоподобный объект для конструирования лог-сообщений.
/// \~russian @details Этот объект создается макросами LOGX. Он собирает потоковые данные и при уничтожении отправляет завершенное событие журнала в синглтон Logger.
struct LogStream {
    /// \~english @brief Constructs a LogStream for a specific log level.
    /// \~russian @brief Конструирует LogStream для определенного уровня логирования.
    /// \~english @param level The severity level of the log message.
    /// \~russian @param level Уровень серьезности лог-сообщения.
    explicit LogStream(TEvent::TLevel const &level);

    /// \~english @brief Destructor that submits the log event.
    /// \~russian @brief Деструктор, который отправляет событие журнала.
    ~LogStream();

    /// \~english @brief Appends a C-style string to the log message.
    /// \~russian @brief Добавляет строку в стиле C к лог-сообщению.
    LogStream const & operator << (char const * const s) const;

    /// \~english @brief Appends a value of any type to the log message.
    /// \~russian @brief Добавляет значение любого типа к лог-сообщению.
    template<typename TValue>
    LogStream const & operator << (TValue const &v) const;
    template<typename TValue>
    LogStream const & operator << (TValue * const * v) const;

private:
    mutable TEvent::TSharedPtr m_event;
};


template<typename TValue>
LogStreamDummy const &
LogStreamDummy::operator<<(TValue) const
{
    return *this;
}


template<typename TValue>
LogStream const &
LogStream::operator<<(TValue const &v) const
{
    m_event->line += convertion::convert<std::string>(v);
    return std::move(*this); // ----->
}


template<typename TValue>
LogStream const &
LogStream::operator<<(TValue* const* v) const
{
    m_event->line += v ? convertion::convert<std::string>(*v) : "nullptr";
    return std::move(*this); // ----->
}


/// \~english @brief Sets the global logger configuration.
/// \~russian @brief Устанавливает глобальную конфигурацию логгера.
/// \~english @param config The configuration to apply.
/// \~russian @param config Конфигурация для применения.
void setConfig(config::TLogger const &config);

/// \~english @brief Converts a C++ function name (e.g., from `__PRETTY_FUNCTION__`) into a cleaner format for logging.
/// \~russian @brief Преобразует имя функции C++ (например, из `__PRETTY_FUNCTION__`) в более чистый формат для логирования.
/// \~english @param name The raw function name.
/// \~russian @param name "Сырое" имя функции.
/// \~english @return A cleaned-up function name.
/// \~russian @return Очищенное имя функции.
std::string convertFunctionNameToLogFunctionName(std::string const &name);

/// \~english @brief Extracts the filename from a full path for concise logging.
/// \~russian @brief Извлекает имя файла из полного пути для краткого логирования.
/// \~english @param path The full file path.
/// \~russian @param path Полный путь к файлу.
/// \~english @return The filename component of the path.
/// \~russian @return Компонент имени файла из пути.
std::string extractFileNameToLog(std::string const &path);


} // namespace iridium::logging


#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif


#if defined(BUILD_TYPE_DEBUG) || defined(BUILD_FLAG_FORCE_DEBUG_LOG)
#define LOGT \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::TRACE) << \
iridium::logging::extractFileNameToLog(std::string(__FILE__) + ":" + std::to_string(__LINE__)) << " "
#else
#define LOGT \
if (false) iridium::logging::LogStreamDummy()
#endif


#if defined(BUILD_TYPE_DEBUG) || defined(BUILD_FLAG_FORCE_DEBUG_LOG)
#define LOGD \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::DEBUG)
#else
#define LOGD \
if (false) iridium::logging::LogStreamDummy()
#endif


#define LOGI \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::INFO)


#define LOGW \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::WARNING)


#define LOGE \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::ERROR_)


#define LOGF \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::FATAL)


#endif // HEADER_LOGGER_68035722_1476_4595_9F55_5338D1576CF9
