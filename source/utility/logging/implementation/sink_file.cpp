#include "sink_file.h"

#include "utility/convertion/convert.h"
#include "utility/io/fs/implementation/file_stream_writer.h"
#include "utility/threading/implementation/worker.h"


using std::string;
using utility::convertion::convert;
using utility::io::Buffer;
using utility::io::fs::implementation::CFileStreamWriter;
using utility::threading::implementation::CWorker;
using std::chrono::system_clock;


namespace utility {
namespace logging {
namespace implementation {


CSinkFile::CSinkFile(TEvent::TLevel const &level, string const &file_name)
:
    CSink(level, CWorkerHandler::create(level, file_name))
{}


CSinkFile::CWorkerHandler::CWorkerHandler(TEvent::TLevel const &level, std::string const &file_name)
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


CSinkFile::CWorkerHandler::TItems CSinkFile::CWorkerHandler::handle(TItems const &events) {
    if (m_is_rotation_by_day && system_clock::now() > m_last_initialization_time) {
        finalize();
        initialize();
    }

    for (auto const &e : events) {
        auto line = makeLine(e);
        line.push_back('\n');
        m_file_writer->write(Buffer(line.begin(), line.end()));
    }

    m_file_writer->flush();

    return TItems(); // ----->
}


} // implementation
} // logging
} // utility
