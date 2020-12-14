// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_LOGGER_68035722_1476_4595_9F55_5338D1576CF9
#define HEADER_LOGGER_68035722_1476_4595_9F55_5338D1576CF9


#include "iridium/pattern/initializable.h"
#include "iridium/pattern/singleton.h"
#include "iridium/smart_ptr.h"

#include "iridium/threading/synchronized.h"
#include "iridium/strings.h"

#include <mutex>

#include "channel.h"
#include "event.h"
#include "sink.h"
#include "config.h"


namespace iridium {
namespace logging {


// todo: separate logger to other type with singleton
class Logger :
    public pattern::Singleton<Logger>,
    public threading::CSynchronized
{
public:
    ///
    virtual ~Logger();
    /// logger factory method
    void update(config::TLogger const &config);
    ///
    void log(TEvent &&event);
    ///
    void addCustomSink(ISink::TSharedPtr const &sink);

private:
    friend class pattern::Singleton<Logger>;
    ///
    Logger();
    ///
    IChannel::TSharedPtr m_channel;
};


// todo: logger by pattern parameter
///
struct LogStream {
    ///
    explicit LogStream(TEvent::TLevel const &level);
    ///
    ~LogStream();
    ///
    LogStream const& operator << (char const * const s) const;
    LogStream const& operator << (char       *       s) const;
    ///
    template<typename TValue>
    LogStream const & operator << (TValue const &v) const;

private:
    ///
    mutable TEvent m_event;
};


//todo: thread safe
template<typename TValue>
LogStream const & LogStream::operator << (TValue const &v) const {
    m_event.line += convertion::convert<std::string>(v);
    return std::move(*this); // ----->
}


void update(config::TLogger const &config);


std::string convertFunctionNameToLogFunctionName(std::string const &name);


} // logger
} // iridium

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif // _MSC_VER


#define LOGT \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::TRACE_LEVEL) << \
iridium::logging::convertFunctionNameToLogFunctionName(__PRETTY_FUNCTION__) << " "
/*
#define LOGT \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::TRACE_LEVEL) << \
iridium::logging::convertFunctionNameToLogFunctionName(__PRETTY_FUNCTION__) << " "
#define LOGT \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::TRACE_LEVEL) << \
iridium::split(iridium::split(__PRETTY_FUNCTION__, "(").front(), " ").back() << "\n"
*/
#define LOGD \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::DEBUG_LEVEL)
#define LOGI \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::INFO_LEVEL)
#define LOGW \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::WARNING_LEVEL)
#define LOGE \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::ERROR_LEVEL)
#define LOGF \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::FATAL_LEVEL)


#endif // HEADER_LOGGER_68035722_1476_4595_9F55_5338D1576CF9
