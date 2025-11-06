#include "logger.h"


#include "implementation/sink_console.h"
#include "implementation/sink_file.h"
#include "implementation/sink.h"

#include "iridium/strings.h"


using iridium::logging::implementation::CSinkConsole;
using iridium::logging::implementation::CSinkFile;
using iridium::logging::implementation::CSinkAsync;
using iridium::convertion::convert;
using std::string;


namespace iridium {
namespace logging {


Logger::~Logger() {
    LOCK_SCOPE();
    for (auto const &sink: m_sinks)
    try {
        sink->finalize();
    } catch (std::exception const &e) {
        std::cerr << "logger finalization error: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "logger finalization error: unknown exception" << std::endl;
    }
}


void Logger::setConfig(config::TLogger const &config) {
    if (!m_is_alive)
        return;

    LOCK_SCOPE();

    for (auto const &sink: m_sinks)
        sink->finalize();
    
    m_sinks.clear();
    ISink::is_gmt_time = config.IsGmtTime;
    
    bool is_console_sink_initialized = false;
    for (auto const &sink_config: config.Sink) {
        auto level = sink_config.Level;
        
        if (level.isDefault())
            level  = config.Level;
        
        ISink::TSharedPtr sink;
        
        switch (sink_config.Type.get()) {
        case config::TLogger::TSink::TSinkType::CONSOLE:

            if (is_console_sink_initialized)
                throw std::runtime_error("only one console sink can be"); // ----->
            else
                is_console_sink_initialized = true;

            sink = CSinkConsole::create(level);

            break; // --->
        case config::TLogger::TSink::TSinkType::FILE:
            sink = CSinkFile::create(level, sink_config.Uri.get());
            break; // --->
        case config::TLogger::TSink::TSinkType::FILE_DAILY:
            sink = CSinkFile::create(level, sink_config.Uri.get(), CSinkFile::TRotation::DAILY);
            break; // --->
        case config::TLogger::TSink::TSinkType::FILE_TIMELY:
            sink = CSinkFile::create(level, sink_config.Uri.get(), CSinkFile::TRotation::TIMELY);
            break; // --->
        default:
            break; // --->
        };
        
        if(!sink)
            throw std::runtime_error(
                "sink type '" + convert<std::string>(sink_config.Type.get()) +
                " uri '" + sink_config.Uri.get() + "' error: not implemented"); // ----->

        if (sink_config.IsAsync.get())
            sink = CSinkAsync::create(sink);
        
        sink->initialize();
        m_sinks.push_back(sink);
    }
    
    m_config = config.getNode();
}


config::TLogger Logger::getConfig() {
    if (!m_is_alive)
        return {};

    LOCK_SCOPE();
    if (m_config)
        return config::TLogger(m_config); // ----->
    else
        throw std::runtime_error("logger config is empty"); // ----->
}


void Logger::log(TEvent::TConstSharedPtr const &e) {
    if (!m_is_alive)
        return;

    LOCK_SCOPE();
    try {
        for (auto const &sink: m_sinks) {
            sink->log(e);
            sink->flush();
        }
    } catch (std::exception const &e) {
        std::cerr << e.what() << std::endl;
        throw; // ----->
    }
}


void Logger::addCustomSink(ISink::TSharedPtr const &sink) {
    if (!m_is_alive)
        return;

    LOCK_SCOPE();
    sink->initialize();
    m_sinks.push_back(sink);
}


// ----------


LogStream::LogStream(TEvent::TLevel const &level)
:
    m_event(
        TEvent::create(
            TEvent {
                std::chrono::system_clock::now(),
                level,
                std::this_thread::get_id(),
                ""
            }
        )
    )
{}


LogStream::~LogStream() {
    Logger::instance().log(m_event);
}


LogStream const &LogStream::operator << (char const * const s) const {
    m_event->line += s;
    return std::move(*this); // ----->
}


//LogStream const &LogStream::operator << (char * s) const {
//    m_event->line += s;
//    return std::move(*this); // ----->
//}


void setConfig(config::TLogger const &config) {
    Logger::instance().setConfig(config);
}


string convertFunctionNameToLogFunctionName(string const &name) {
    static size_t DEFAULT_FUNC_NAME_SIZE = 16;

    size_t rpos = name.find("(");
    size_t lpos = name.find_last_of(" ", rpos) + 1;

    if (lpos == string::npos)
        lpos = 0;

    while (!std::isalpha(name[lpos]) && lpos < name.size())
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
#ifdef WINDOWS_PLATFORM
    static char const split_symbol = '\\';
#else
    static char const split_symbol = '/';
#endif
    string file_name = path.substr(path.find_last_of(split_symbol) + 1, std::string::npos);
    if (file_name.size() < DEFAULT_FILE_NAME_LOG_SIZE)
        file_name.append(DEFAULT_FILE_NAME_LOG_SIZE - file_name.size(), ' ');
    return file_name; // ----->
}


} // logger
} // iridium
