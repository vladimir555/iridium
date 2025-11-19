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
class Logger :
    public pattern::Singleton<Logger>,
    public threading::Synchronized<std::mutex>
{
public:
    virtual ~Logger();

    void setConfig(config::TLogger const &config);
    config::TLogger getConfig();

    void log(TEvent::TConstSharedPtr const &event);
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


struct LogStream {
    explicit LogStream(TEvent::TLevel const &level);
    ~LogStream();

    LogStream const & operator << (char const * const s) const;

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


void setConfig(config::TLogger const &config);
std::string convertFunctionNameToLogFunctionName(std::string const &name);
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
