#include "sink_file.h"

#include "utility/convertion/convert.h"
#include "utility/fs/implementation/fast_text_writer.h"

#include <chrono>

using std::string;
using utility::convertion::convert;
using utility::fs::implementation::CFastTextWriter;
using std::chrono::system_clock;


namespace utility {
namespace logging {
namespace implementation {


CSinkFile::CSinkFile(TEvent::TLevel const &level, string const &file_name)
:
    CSink(level, CSinkWorker::create(file_name))
{}


CSinkFile::CSinkWorker::CSinkWorker(string const &file_name, bool const &is_daily_rotation)
:
    CWorker<TEvent>             ("FileSink"),
    m_file_name                 (file_name),
    m_file_name_original        (file_name),
    m_is_rotation_by_day        (is_daily_rotation),
    m_last_initialization_time  (0)
{}


void CSinkFile::CSinkWorker::initialize() {
    initializeInternal();
    CWorker<TEvent>::initialize();
}


void CSinkFile::CSinkWorker::finalize() {
    CWorker<TEvent>::finalize();
    finalizeInternal();
}


void CSinkFile::CSinkWorker::initializeInternal() {
    if (m_is_rotation_by_day) {
        auto date                   = convert<string>(system_clock::now()).substr(0, 10);
        m_last_initialization_time  = system_clock::to_time_t(convert<system_clock::time_point>(date + "T00:00:00.000"));

        m_file_name = m_file_name_original;
        auto file_ext_position = m_file_name.find_last_of('.');
        if (file_ext_position > 0)
            m_file_name.replace(m_file_name.find_last_of('.'), 1, "-" + date + ".");
        else
            m_file_name.replace(m_file_name.find_last_of('.'), 1, date + ".");
    }

    m_text_file_writer = CFastTextWriter::create(m_file_name);
    m_text_file_writer->initialize();
}


void CSinkFile::CSinkWorker::finalizeInternal() {
    m_text_file_writer->finalize();
}


void CSinkFile::CSinkWorker::handleItems(TItems const &events) {
    if (m_is_rotation_by_day && system_clock::to_time_t(system_clock::now()) > m_last_initialization_time) {
        finalizeInternal();
        initializeInternal();
    }

    for (auto const &e : events)
        m_text_file_writer->writeLine(makeLine(e));

    m_text_file_writer->flush();
};


} // implementation
} // logging
} // utility
