#include "logger.h"


#include "implementation/channel.h"
#include "implementation/sink_console.h"
#include "implementation/sink_file.h"

#include "iridium/threading/thread.h"
#include "iridium/assert.h"


using iridium::logging::implementation::CChannel;
using iridium::logging::implementation::CSinkConsole;
using iridium::logging::implementation::CSinkFile;
using std::string;


namespace iridium {
namespace logging {


Logger::~Logger() {
    LOCK_SCOPE();
    if (m_channel) {
        m_channel->finalize();
        m_channel.reset();
    }
}


// todo: factory 
// todo: refactoring for external sinks via convertion or factory ! one sub config for one sink
void Logger::update(config::TLogger const &config) {
    LOCK_SCOPE();

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
    LOCK_SCOPE();
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
    m_event({ level, threading::getThreadID() + " " })
{}


LogStream::~LogStream() {
    Logger::instance().log(std::move(m_event));
}


LogStream const &LogStream::operator << (char const * const s) const {
    m_event.line = m_event.line + s;
    return std::move(*this); // ----->
}


LogStream const &LogStream::operator << (char * s) const {
    m_event.line = m_event.line + s;
    return std::move(*this); // ----->
}


void update(config::TLogger const &config) {
    Logger::instance().update(config);
}


string convertFunctionNameToLogFunctionName(string const &name) {
    static size_t DEFAULT_FUNC_NAME_SIZE = 16;

    size_t rpos = name.find("(");
    size_t lpos = name.find_last_of(" ", rpos) + 1;

    if (lpos == string::npos)
        lpos = 0;

    while ( !std::isalpha(name[lpos]) && lpos < name.size())
        lpos++;

    string result = name.substr(lpos, rpos - lpos);

    if (result.size() < DEFAULT_FUNC_NAME_SIZE)
        result.append(DEFAULT_FUNC_NAME_SIZE - result.size(), ' ');

    if (result.size() > 1) {
        std::list<string> words = split(result, "::");
        result = words.back();
        words.pop_back();
        if (!words.empty())
            result = words.back() + "::" + result;
    }

    return result; // ----->
}


string extractFileNameToLog(string const &path) {
    static size_t const DEFAULT_FILE_NAME_LOG_SIZE = 24;
    string file_name = path.substr(path.find_last_of('/') + 1, std::string::npos);
    if (file_name.size() < DEFAULT_FILE_NAME_LOG_SIZE)
        file_name.append(DEFAULT_FILE_NAME_LOG_SIZE - file_name.size(), ' ');
    return file_name; // ----->
}


} // logger
} // iridium
