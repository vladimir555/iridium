#ifndef HEADER_LOGGER_68035722_1476_4595_9F55_5338D1576CF9
#define HEADER_LOGGER_68035722_1476_4595_9F55_5338D1576CF9


#include "utility/pattern/initializable.h"
#include "utility/pattern/singleton.h"
#include "utility/smart_ptr.h"

#include "utility/threading/synchronized.h"
#include "utility/strings.h"

#include <mutex>

#include "channel.h"
#include "event.h"
#include "sink.h"
#include "config.h"


namespace utility {
namespace logging {


// todo: separate logger to other type with singleton
class Logger :
    public pattern::Singleton<Logger>,
    public threading::Synchronized
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
    LogStream const & operator << (char const *s) const;
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
} // utility


#define LOGT \
utility::logging::LogStream(utility::logging::TEvent::TLevel::TRACE_LEVEL) << \
utility::logging::convertFunctionNameToLogFunctionName(__PRETTY_FUNCTION__) << " "
//#define LOGT \
//utility::logging::LogStream(utility::logging::TEvent::TLevel::TRACE_LEVEL) << \
//utility::logging::convertFunctionNameToLogFunctionName(__PRETTY_FUNCTION__) << " "
//#define LOGT \
//utility::logging::LogStream(utility::logging::TEvent::TLevel::TRACE_LEVEL) << \
//utility::split(utility::split(__PRETTY_FUNCTION__, "(").front(), " ").back() << "\n"
#define LOGD \
utility::logging::LogStream(utility::logging::TEvent::TLevel::DEBUG_LEVEL)
#define LOGI \
utility::logging::LogStream(utility::logging::TEvent::TLevel::INFO_LEVEL)
#define LOGW \
utility::logging::LogStream(utility::logging::TEvent::TLevel::WARNING_LEVEL)
#define LOGE \
utility::logging::LogStream(utility::logging::TEvent::TLevel::ERROR_LEVEL)
#define LOGF \
utility::logging::LogStream(utility::logging::TEvent::TLevel::FATAL_LEVEL)


#endif // HEADER_LOGGER_68035722_1476_4595_9F55_5338D1576CF9
