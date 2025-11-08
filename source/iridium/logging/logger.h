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


namespace iridium {
namespace logging {


// todo: wait for finalizing async sinks
class Logger :
    public pattern::Singleton<Logger>,
    public threading::Synchronized<std::mutex>
{
public:
    /// \~english @brief Virtual destructor.
    /// \~russian @brief Виртуальный деструктор.
    virtual ~Logger();

    /// \~english @brief Sets the logger configuration.
    /// \~russian @brief Устанавливает конфигурацию логгера.
    /// \~english @param config The logger configuration.
    /// \~russian @param config Конфигурация логгера.
    void setConfig(config::TLogger const &config);

    /// \~english @brief Gets the logger configuration.
    /// \~russian @brief Возвращает конфигурацию логгера.
    /// \~english @return The logger configuration.
    /// \~russian @return Конфигурация логгера.
    config::TLogger getConfig();

    /// \~english @brief Logs an event.
    /// \~russian @brief Логирует событие.
    /// \~english @param event The event to log.
    /// \~russian @param event Событие для логирования.
    void log(TEvent::TConstSharedPtr const &event);

    /// \~english @brief Adds a custom sink to the logger.
    /// \~russian @brief Добавляет пользовательский приемник в логгер.
    /// \~english @param sink The custom sink to add.
    /// \~russian @param sink Пользовательский приемник для добавления.
    void addCustomSink(ISink::TSharedPtr const &sink);

private:
    /// \~english @brief Friend class declaration for Singleton pattern.
    /// \~russian @brief Объявление дружественного класса для паттерна "Одиночка".
    friend class pattern::Singleton<Logger>;

    /// \~english @brief Default constructor.
    /// \~russian @brief Конструктор по умолчанию.
    Logger() = default;

    /// \~english @brief List of logging sinks.
    /// \~russian @brief Список приемников логирования.
    std::list<ISink::TSharedPtr> m_sinks;

    /// \~english @brief The logger configuration as a node.
    /// \~russian @brief Конфигурация логгера в виде узла.
    parsing::INode::TSharedPtr m_config;
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

/// \~english @brief A log stream that builds a log event and sends it to the logger upon destruction.
/// \~russian @brief Поток лога, который создает событие лога и отправляет его в логгер при уничтожении.
struct LogStream
{
    /// \~english @brief Constructs a log stream with a given logging level.
    /// \~russian @brief Конструирует поток лога с заданным уровнем логирования.
    /// \~english @param level The logging level of the event.
    /// \~russian @param level Уровень логирования события.
    explicit LogStream(TEvent::TLevel const &level);

    /// \~english @brief Destructor that sends the log event to the logger.
    /// \~russian @brief Деструктор, который отправляет событие лога в логгер.
    ~LogStream();

    /// \~english @brief Streams a C-style string to the log event.
    /// \~russian @brief Направляет C-строку в событие лога.
    LogStream const &operator << (char const* const s) const;

    /// \~english @brief Streams a value of any type to the log event.
    /// \~russian @brief Направляет значение любого типа в событие лога.
    template<typename TValue>
    LogStream const &operator << (TValue const &v) const;

    /// \~english @brief Streams a pointer to a pointer to a value to the log event.
    /// \~russian @brief Направляет указатель на указатель на значение в событие лога.
    template<typename TValue>
    LogStream const &operator << (TValue* const* v) const;
//    ///
//    template<typename TValue>
//    LogStream const &operator << (std::atomic<TValue> const &v) const;
//    ///
//    template<typename TValue>
//    LogStream const &operator << (std::shared_ptr<TValue> const &v) const;
//    ///
//    template<typename TValue>
//    LogStream const &operator << (std::list<TValue> const &values) const;
//    ///
//    template<typename TValue>
//    LogStream const &operator << (std::vector<TValue> const &values) const;
//    ///
//    template<typename TValue>
//    LogStream const &operator << (std::deque<TValue> const &values) const;
//    ///
//    template<typename TValue, std::size_t size>
//    LogStream const &operator << (std::array<TValue, size> const &values) const;
//    ///
//    template<typename TValue>
//    LogStream const &operator << (std::set<TValue> const &values) const;


//    template<typename TContainer>
//    typename std::enable_if<
//        std::is_same_v<TContainer, std::vector          <typename TContainer::value_type>> ||
//        std::is_same_v<TContainer, std::list            <typename TContainer::value_type>> ||
//        std::is_same_v<TContainer, std::set             <typename TContainer::value_type>> ||
//        std::is_same_v<TContainer, std::unordered_set   <typename TContainer::value_type>>,
//        LogStream const &
//    >::type operator << (TContainer const &values) const {
//        *this << "[ ";
//        bool is_first = true;
//        for (auto const &value: values)
//            *this << (is_first ? "" : ", ") << value, is_first = false;
//        *this << "]";
//        return *this;
//    }


//    ///
//    template<typename TKey, typename TValue>
//    LogStream const &operator << (std::map<TKey, TValue> const &values) const;
//    ///
//    template<typename TValue>
//    LogStream const &operator << (std::unordered_set<TValue> const &values) const;
//    ///
//    template<typename TKey, typename TValue>
//    LogStream const &operator << (std::unordered_map<TKey, TValue> const &values) const;

private:
    ///
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


//template<typename TValue>
//LogStream const &LogStream::operator << (std::atomic<TValue> const &v) const {
//    m_event->line += convertion::convert<std::string>(static_cast<TValue>(v));
//    return std::move(*this); // ----->
//}


template<typename TValue>
LogStream const &
LogStream::operator<<(TValue* const* v) const
{
    m_event->line += v ? convertion::convert<std::string>(*v) : "nullptr";
    return std::move(*this); // ----->
}


//template<typename TValue>
//LogStream const &LogStream::operator << (std::shared_ptr<TValue> const &v) const {
//    m_event->line += v ? convertion::convert<std::string>(*v) : "nullptr";
//    return std::move(*this); // ----->
//}


//template<typename TValue>
//LogStream const &LogStream::operator << (std::list<TValue> const &values) const {
//    *this << "[ ";
//    bool is_first = true;
//    for (auto const &value: values)
//        *this << (is_first ? "" : ", ") << value, is_first = false;
//    *this << " ]";
//    return *this;
//}


//template<typename TValue>
//LogStream const &LogStream::operator << (std::vector<TValue> const &values) const {
//    *this << "[ ";
//    bool is_first = true;
//    for (auto const &value: values)
//        *this << (is_first ? "" : ", ") << value, is_first = false;
//    *this << " ]";
//    return *this;
//}


//template<typename TValue>
//LogStream const &LogStream::operator << (std::deque<TValue> const &values) const {
//    *this << "[ ";
//    bool is_first = true;
//    for (auto const &value: values)
//        *this << (is_first ? "" : ", ") << value, is_first = false;
//    *this << " ]";
//    return *this;
//}


//template<typename TValue, std::size_t size>
//LogStream const &LogStream::operator << (std::array<TValue, size> const &values) const {
//    *this << "[ ";
//    bool is_first = true;
//    for (auto const &value: values)
//        *this << (is_first ? "" : ", ") << value, is_first = false;
//    *this << " ]";
//    return *this;
//}


//template<typename TValue>
//LogStream const &LogStream::operator << (std::set<TValue> const &values) const {
//    *this << "[ ";
//    bool is_first = true;
//    for (auto const &value: values)
//        *this << (is_first ? "" : ", ") << value, is_first = false;
//    *this << " ]";
//    return *this;
//}


//template<typename TValue>
//LogStream const &LogStream::operator << (std::unordered_set<TValue> const &values) const {
//    *this << "[ ";
//    bool is_first = true;
//    for (auto const &value: values)
//        *this << (is_first ? "" : ", ") << value, is_first = false;
//    *this << " ]";
//    return *this;
//}


//template<typename TKey, typename TValue>
//LogStream const &LogStream::operator << (std::map<TKey, TValue> const &m) const {
//    *this << "[ ";
//    bool is_first = true;
//    for (auto const &item: m)
//        *this << (is_first ? "" : ", ") << item.first << ": " << item.second << " ", is_first = false;;
//    *this << " ]";
//    return *this;
//}


//template<typename TKey, typename TValue>
//LogStream const &LogStream::operator << (std::unordered_map<TKey, TValue> const &m) const {
//    *this << "[ ";
//    bool is_first = true;
//    for (auto const &item: m)
//        *this << (is_first ? "" : ", ") << item.first << ": " << item.second << " ", is_first = false;;
//    *this << " ]";
//    return *this;
//}


void setConfig(config::TLogger const &config);


std::string convertFunctionNameToLogFunctionName(std::string const &name);


std::string extractFileNameToLog(std::string const &path);


} // logging
} // iridium


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
