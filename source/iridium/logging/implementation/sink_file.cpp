/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "sink_file.h"

#include "iridium/convertion/convert.h"
#include "iridium/io/fs/implementation/file_stream_writer.h"
#include "iridium/threading/implementation/worker.h"


using std::string;
using iridium::convertion::convert;
using iridium::io::Buffer;
using iridium::io::fs::implementation::CFileStreamWriter;
using iridium::threading::implementation::CWorker;
using std::chrono::system_clock;


namespace iridium {
namespace logging {
namespace implementation {


CSinkFile::CSinkFile(TEvent::TLevel const &level, string const &file_name)
:
    CSink(level, CWorkerHandler::create(level, file_name))
{}


CSinkFile::CWorkerHandler::CWorkerHandler(TEvent::TLevel const &, std::string const &file_name)
:
    m_file_name                 (file_name),
    m_file_name_original        (file_name),
    m_is_rotation_by_day        (true),
    m_last_initialization_time  ()
{}


void CSinkFile::CWorkerHandler::initialize() {
    if (m_is_rotation_by_day) {
        auto date                   = convert<string>(system_clock::now()).substr(0, 10);
        m_last_initialization_time  = convert<system_clock::time_point>(date + " 00:00:00.000");

        m_file_name = m_file_name_original;
        auto file_ext_position = m_file_name.find_last_of('.');
        if (file_ext_position > 0)
            m_file_name.replace(m_file_name.find_last_of('.'), 1, "-" + date + ".");
        else
            m_file_name.replace(m_file_name.find_last_of('.'), 1, date + ".");
    }

    m_file_writer = CFileStreamWriter::create(m_file_name);
    m_file_writer->initialize();
}


void CSinkFile::CWorkerHandler::finalize() {
    m_file_writer->finalize();
}


void CSinkFile::CWorkerHandler::handle(TItems const &events) {
    if (m_is_rotation_by_day && system_clock::now() > m_last_initialization_time) {
        finalize();
        initialize();
    }

    for (auto const &e : events) {
        auto line = makeLine(e);
        line.push_back('\n');
        m_file_writer->write(Buffer::create(line.begin(), line.end()));
    }

    m_file_writer->flush();
}


} // implementation
} // logging
} // iridium
