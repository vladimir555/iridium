#include "sink_file.h"

#include "iridium/convertion/convert.h"
#include "iridium/io/fs/implementation/file_stream_writer.h"
#include "iridium/threading/implementation/worker.h"
#include "iridium/items.h"

#include <iostream>


using iridium::convertion::convert;
using iridium::io::Buffer;
using iridium::io::fs::implementation::CFileStreamWriter;
using iridium::threading::implementation::CWorker;

using std::chrono::system_clock;
using std::string;


IMPLEMENT_ENUM(iridium::logging::implementation::CSinkFile::TRotation)


namespace iridium {
namespace logging {
namespace implementation {


CSinkFile::CSinkFile(TEvent::TLevel const &level, string const &path, TRotation const &rotation)
:
    m_level                     (level),
    m_path                      (path),
    m_path_original             (path),
    m_rotation                  (rotation),
    m_last_initialization_time  (),
    m_file_writer               (nullptr)
{}


void CSinkFile::initialize() {
    if (m_file_writer)
        return; // ----->
    
    if (checkOneOf(m_rotation, TRotation::DAILY, TRotation::TIMELY)) {
        string date;

        if (m_rotation == TRotation::DAILY) {
            date = convert<string>(system_clock::now(), static_cast<bool>(is_gmt_time)).substr(0, 10);
            m_last_initialization_time =
                convert<system_clock::time_point>(date + " 00:00:00.000");
        }

        if (m_rotation == TRotation::TIMELY) {
            auto now    = system_clock::now();
            date        = convert<string>(now, static_cast<bool>(is_gmt_time));
            date[10]    = '_';
            date[13]    = '-';
            date[16]    = '-';
            m_last_initialization_time = now;
        }

        m_path = m_path_original;

        m_path.erase(std::remove(m_path.begin(), m_path.end(), '\\'), m_path.end());
        std::replace(m_path.begin(), m_path.end(), '\\', '/');

        auto file_ext_position = m_path.find_last_of('/');
        if  (file_ext_position == string::npos)
             file_ext_position = 0;

        file_ext_position += m_path.substr(file_ext_position).find_last_of('.');

        if  (file_ext_position == string::npos)
            m_path += "-" + date;

        else

        if  (file_ext_position > 0)
            m_path.replace(file_ext_position, 1, "-" + date + ".");
        else
            m_path.replace(file_ext_position, 1, date + ".");
    }

    try {
        m_file_writer = CFileStreamWriter::create(m_path);
        m_file_writer->initialize();
    } catch (std::exception const &e) {
        std::cerr << "initializing logger file sink '" + m_path + "' error: " << e.what() << std::endl;
    }
}


void CSinkFile::finalize() {
    if (m_file_writer) {
        try {
            m_file_writer->finalize();
            m_file_writer.reset();
        } catch (std::exception const &e) {
            std::cerr << "finalizing logger file sink '" + m_path + "' error: " << e.what() << std::endl;
        }
    }
}


void CSinkFile::log(TEvent::TConstSharedPtr const &event) {
    if (!m_file_writer || event->level < m_level)
        return; // ----->

    if (m_rotation == TRotation::DAILY && system_clock::now() > m_last_initialization_time) {
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
