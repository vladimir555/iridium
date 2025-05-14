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
class Logger:
    public pattern::Singleton<Logger>,
    public threading::Synchronized<std::mutex>
{
public:
    ///
    virtual ~Logger();
    ///
    void setConfig(config::TLogger const &config);
    ///
    config::TLogger getConfig();
    ///
    void log(TEvent::TConstSharedPtr const &event);
    ///
    void addCustomSink(ISink::TSharedPtr const &sink);

private:
    ///
    friend class pattern::Singleton<Logger>;
    ///
    Logger() = default;
    ///
    std::list<ISink::TSharedPtr> m_sinks;
    ///
    parsing::INode::TSharedPtr m_config;
};


struct LogStreamDummy {
    template<typename TValue>
    LogStreamDummy const & operator << (TValue v) const;
};


///
struct LogStream {
    ///
    explicit LogStream(TEvent::TLevel const &level);
    ///
    ~LogStream();
    ///
    LogStream const & operator << (char const * const s) const;
    ///
    LogStream const & operator << (char       *       s) const;
    ///
    template<typename TValue>
    LogStream const & operator << (TValue const &v) const;
    ///
    template<typename TValue>
    LogStream const & operator << (TValue * const * v) const;
    ///
    template<typename TValue>
    LogStream const & operator << (std::atomic<TValue> const &v) const;
    ///
    template<typename TValue>
    LogStream const & operator << (std::shared_ptr<TValue> const &v) const;
    ///
    template<typename TValue>
    LogStream const & operator << (std::list<TValue> const &values) const;
    ///
    template<typename TValue>
    LogStream const & operator << (std::vector<TValue> const &values) const;
    ///
    template<typename TValue>
    LogStream const & operator << (std::deque<TValue> const &values) const;
    ///
    template<typename TValue, std::size_t size>
    LogStream const & operator << (std::array<TValue, size> const &values) const;
    ///
    template<typename TValue>
    LogStream const & operator << (std::set<TValue> const &values) const;


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


    ///
    template<typename TKey, typename TValue>
    LogStream const & operator << (std::map<TKey, TValue> const &values) const;
    ///
    template<typename TValue>
    LogStream const & operator << (std::unordered_set<TValue> const &values) const;
    ///
    template<typename TKey, typename TValue>
    LogStream const & operator << (std::unordered_map<TKey, TValue> const &values) const;

private:
    ///
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
LogStream const &LogStream::operator << (std::atomic<TValue> const &v) const {
    m_event->line += convertion::convert<std::string>(static_cast<TValue>(v));
    return std::move(*this); // ----->
}


template<typename TValue>
LogStream const &LogStream::operator << (TValue * const * v) const {
    m_event->line += v ? convertion::convert<std::string>(*v) : "nullptr";
    return std::move(*this); // ----->
}


template<typename TValue>
LogStream const &LogStream::operator << (std::shared_ptr<TValue> const &v) const {
    m_event->line += v ? convertion::convert<std::string>(*v) : "nullptr";
    return std::move(*this); // ----->
}


template<typename TValue>
LogStream const & LogStream::operator << (std::list<TValue> const &values) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &value: values)
        *this << (is_first ? "" : ", ") << value, is_first = false;
    *this << " ]";
    return *this;
}


template<typename TValue>
LogStream const &LogStream::operator << (std::vector<TValue> const &values) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &value: values)
        *this << (is_first ? "" : ", ") << value, is_first = false;
    *this << " ]";
    return *this;
}


template<typename TValue>
LogStream const &LogStream::operator << (std::deque<TValue> const &values) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &value: values)
        *this << (is_first ? "" : ", ") << value, is_first = false;
    *this << " ]";
    return *this;
}


template<typename TValue, std::size_t size>
LogStream const &LogStream::operator << (std::array<TValue, size> const &values) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &value: values)
        *this << (is_first ? "" : ", ") << value, is_first = false;
    *this << " ]";
    return *this;
}


template<typename TValue>
LogStream const &LogStream::operator << (std::set<TValue> const &values) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &value: values)
        *this << (is_first ? "" : ", ") << value, is_first = false;
    *this << " ]";
    return *this;
}


template<typename TValue>
LogStream const &LogStream::operator << (std::unordered_set<TValue> const &values) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &value: values)
        *this << (is_first ? "" : ", ") << value, is_first = false;
    *this << " ]";
    return *this;
}


template<typename TKey, typename TValue>
LogStream const &LogStream::operator << (std::map<TKey, TValue> const &m) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &item: m)
        *this << (is_first ? "" : ", ") << item.first << ": " << item.second << " ", is_first = false;;
    *this << " ]";
    return *this;
}


template<typename TKey, typename TValue>
LogStream const &LogStream::operator << (std::unordered_map<TKey, TValue> const &m) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &item: m)
        *this << (is_first ? "" : ", ") << item.first << ": " << item.second << " ", is_first = false;;
    *this << " ]";
    return *this;
}


void setConfig(config::TLogger const &config);


std::string convertFunctionNameToLogFunctionName(std::string const &name);


std::string extractFileNameToLog(std::string const &path);


} // logging
} // iridium


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
iridium::logging::LogStreamDummy()
#endif // BUILD_TYPE_DEBUG

#if defined(BUILD_TYPE_DEBUG) || defined(BUILD_FLAG_FORCE_DEBUG_LOG)
#define LOGD \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::DEBUG)
#else
#define LOGD \
iridium::logging::LogStreamDummy()
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
