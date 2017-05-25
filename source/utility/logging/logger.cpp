#include "logger.h"


#include "implementation/channel.h"
#include "implementation/sink_console.h"
#include "implementation/sink_file.h"

#include "utility/threading/synchronized_scope.h"
#include "utility/assert.h"

#include <iostream>


using utility::logging::implementation::CChannel;
using utility::logging::implementation::CSinkConsole;
using utility::logging::implementation::CSinkFile;


namespace utility {
namespace logging {


Logger::~Logger() {
    LOCK_SCOPE;
    if (m_channel) {
        m_channel->finalize();
        m_channel.reset();
    }
}


// todo: factory 
// todo: refactoring for external sinks via convertion or factory ! one sub config for one sink
void Logger::update(config::TLogger const &config) {
    LOCK_SCOPE;
    if (m_channel)
        m_channel->finalize();

    m_channel = CChannel::create();

    // console sink
    bool is_console_sink_activated = false;
    for (auto const &sink : config.ConsoleSink) {
        if (is_console_sink_activated)
            throw std::runtime_error("only one console sink can be"); // ----->
        else {
            auto level = sink.Level;
            if (level == TEvent::TLevel::UNKNOWN)
                level  = config.Level;
            m_channel->attach(CSinkConsole::create(sink.Level));
            is_console_sink_activated = true;
        }
    }

    for (auto const &sink : config.FileSink) {
        auto level = sink.Level;
        if (level == TEvent::TLevel::UNKNOWN)
            level  = config.Level;
        m_channel->attach(CSinkFile::create(sink.Level, sink.FileName));
    }

    m_channel->initialize();
}


void Logger::log(TEvent &&e) {
    if (m_channel)
        m_channel->log(std::move(e));
}


void Logger::addCustomSink(ISink::TSharedPtr const &sink) {
    LOCK_SCOPE;
    if (m_channel)
        m_channel->finalize();
    else
        m_channel = CChannel::create();

    m_channel->attach(sink);
    m_channel->initialize();
}


// ----------


LogStream::LogStream(TEvent::TLevel const &level)
:
    m_event({ level, "" }) 
{}


LogStream::~LogStream() {
    Logger::instance().log(std::move(m_event));
}


LogStream const & LogStream::operator << (char const *s) const {
    m_event.line = m_event.line + s;
    return std::move(*this); // ----->
}


} // logger
} // utility