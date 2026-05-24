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
/// \~english @brief Main logger class for managing log events and sinks.
/// \~russian @brief Основной класс логгера для управления событиями логирования и стоками.
class Logger:
    public pattern::Singleton<Logger>,
    public threading::Synchronized<std::mutex>
{
public:
    virtual ~Logger();

    /// \~english @brief Sets the logger configuration.
    /// \~russian @brief Устанавливает конфигурацию логгера.
    void setConfig(config::TLogger const &config);
    /// \~english @brief Gets the current logger configuration.
    /// \~russian @brief Возвращает текущую конфигурацию логгера.
    config::TLogger getConfig();

    /// \~english @brief Logs a specific event.
    /// \~russian @brief Записывает в лог конкретное событие.
    void log(TEvent::TConstSharedPtr const &event);
    /// \~english @brief Adds a custom sink to the logger.
    /// \~russian @brief Добавляет пользовательский сток в логгер.
    void addCustomSink(ISink::TSharedPtr const &sink);

private:
    friend class pattern::Singleton<Logger>;
    Logger() = default;

    std::list<ISink::TSharedPtr>    m_sinks;
    parsing::INode::TSharedPtr      m_config;
};


/// \~english @brief Dummy log stream that ignores all input.
/// \~russian @brief Фиктивный поток лога, который игнорирует все входные данные.
struct LogStreamDummy {
    template<typename TValue>
    LogStreamDummy const & operator << (TValue v) const;
};


/// \~english @brief Stream-like object for creating and submitting log events.
/// \~russian @brief Потоковый объект для создания и отправки событий лога.
struct LogStream {
    /// \~english @brief Constructs a log stream with a specific severity level.
    /// \~russian @brief Создает поток лога с определенным уровнем важности.
    explicit LogStream(TEvent::TLevel const &level);
    ~LogStream();

    /// \~english @brief Appends a string to the log event.
    /// \~russian @brief Добавляет строку к событию лога.
    LogStream const & operator << (char const * const s) const;

    /// \~english @brief Appends any convertible value to the log event.
    /// \~russian @brief Добавляет любое конвертируемое значение к событию лога.
    template<typename TValue>
    LogStream const & operator << (TValue const &v) const;
    /// \~english @brief Appends a pointer value to the log event.
    /// \~russian @brief Добавляет значение указателя к событию лога.
    template<typename TValue>
    LogStream const & operator << (TValue * const * v) const;

private:
    mutable TEvent::TSharedPtr m_event;
};


template<typename TValue>
LogStreamDummy const &LogStreamDummy::operator << (TValue) const {
    return *this;
}


template<typename TValue>
LogStream const &LogStream::operator << (TValue const &v) const {
    m_event->line += convertion::convert<std::string>(v);
    return std::move(*this); // ----->
}


template<typename TValue>
LogStream const &LogStream::operator << (TValue * const * v) const {
    m_event->line += v ? convertion::convert<std::string>(*v) : "nullptr";
    return std::move(*this); // ----->
}


/// \~english @brief Globally sets the logger configuration.
/// \~russian @brief Глобально устанавливает конфигурацию логгера.
void setConfig(config::TLogger const &config);
/// \~english @brief Converts a raw function name to a cleaner format for logs.
/// \~russian @brief Преобразует имя функции в более чистый формат для логов.
std::string convertFunctionNameToLogFunctionName(std::string const &name);
/// \~english @brief Extracts the file name from a full path for log output.
/// \~russian @brief Извлекает имя файла из полного пути для вывода в лог.
std::string extractFileNameToLog(std::string const &path);


} // namespace iridium::logging


#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif // _MSC_VER


// macros: __func__
#if defined(BUILD_TYPE_DEBUG) || defined(BUILD_FLAG_FORCE_DEBUG_LOG)
#define LOGT \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::TRACE) << \
iridium::logging::extractFileNameToLog(std::string(__FILE__) + ":" + std::to_string(__LINE__)) << " "
#else
#define LOGT \
if (false) iridium::logging::LogStreamDummy()
#endif // BUILD_TYPE_DEBUG


#if defined(BUILD_TYPE_DEBUG) || defined(BUILD_FLAG_FORCE_DEBUG_LOG)
#define LOGD \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::DEBUG)
#else
#define LOGD \
if (false) iridium::logging::LogStreamDummy()
#endif // BUILD_TYPE_DEBUG


#define LOGI \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::INFO)


#define LOGW \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::WARNING)


#define LOGE \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::ERROR_)


#define LOGF \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::FATAL)


#endif // HEADER_LOGGER_68035722_1476_4595_9F55_5338D1576CF9
