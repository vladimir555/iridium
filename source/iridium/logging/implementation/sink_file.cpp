#include "sink_file.h"

#include "iridium/convertion/convert.h"
#include "iridium/io/fs/implementation/file_stream_writer.h"
#include "iridium/threading/implementation/worker.h"


using iridium::convertion::convert;
using iridium::io::Buffer;
using iridium::io::fs::implementation::CFileStreamWriter;
using iridium::threading::implementation::CWorker;

using std::chrono::system_clock;
using std::string;


namespace iridium {
namespace logging {
namespace implementation {


CSinkFile::CSinkFile(TEvent::TLevel const &level, string const &file_name)
:
    m_level                     (level),
    m_file_name                 (file_name),
    m_file_name_original        (file_name),
    m_is_rotation_by_day        (true), // todo: rotation time 1h, 1d, 1w ...
    m_last_initialization_time  (),
    m_file_writer               (nullptr)
{}


void CSinkFile::initialize() {
    if (m_file_writer)
        return; // ----->
    
    if (m_is_rotation_by_day) {
        auto date                   = convert<string>(system_clock::now()).substr(0, 10);
        m_last_initialization_time  = convert<system_clock::time_point>(date + " 00:00:00.000");

        // todo: bugfix for path "./"
        m_file_name = m_file_name_original;
        auto file_ext_position = m_file_name.find_last_of('.');
        if  (file_ext_position > 0)
            m_file_name.replace (m_file_name.find_last_of('.'), 1, "-" + date + ".");
        else
            m_file_name.replace (m_file_name.find_last_of('.'), 1, date + ".");
    }

    m_file_writer = CFileStreamWriter::create(m_file_name);
    m_file_writer->initialize();
}


void CSinkFile::finalize() {
    if (m_file_writer) {
        m_file_writer->finalize();
        m_file_writer.reset();
    }
}


void CSinkFile::log(TEvent::TConstSharedPtr const &event) {
    if (!m_file_writer || event->level < m_level)
        return; // ----->

    if (m_is_rotation_by_day && system_clock::now() > m_last_initialization_time) {
        finalize();
        initialize();
    }

    auto line = makeLine(event);
    line.push_back('\n');
    m_file_writer->write(Buffer::create(line.begin(), line.end()));
//    m_file_writer->flush();
}


} // implementation
} // logging
} // iridium
