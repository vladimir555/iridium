#include "sink_file.h"

#include "utility/convertion/convert.h"
#include "utility/fs/implementation/fast_text_writer.h"

#include <chrono>

using std::string;
using utility::convertion::convert;
using utility::fs::implementation::CFastTextWriter;
using utility::threading::implementation::CWorker;
using std::chrono::high_resolution_clock;


namespace utility {
namespace logging {
namespace implementation {


CSinkFile::CSinkFile(TEvent::TLevel const &level, string const &file_name)
:
    CSink                       (level, getSharedThis(this)),
    m_file_name                 (file_name),
    m_file_name_original        (file_name),
    m_is_rotation_by_day        (true),
    m_last_initialization_time  ()
{}


void CSinkFile::handleStart() {
    if (m_is_rotation_by_day) {
        auto date                   = convert<string>(high_resolution_clock::now()).substr(0, 10);
        m_last_initialization_time  = convert<high_resolution_clock::time_point>(date + " 00:00:00.000");

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


void CSinkFile::handleStop() {
    m_text_file_writer->finalize();
}


void CSinkFile::handleItems(TItems const &events) {
    if (m_is_rotation_by_day && high_resolution_clock::now() > m_last_initialization_time) {
        handleStop();
        handleStart();
    }

    for (auto const &e : events)
        m_text_file_writer->writeLine(makeLine(e));

    m_text_file_writer->flush();
}


} // implementation
} // logging
} // utility
