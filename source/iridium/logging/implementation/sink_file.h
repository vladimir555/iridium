// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573
#define HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573


#include "iridium/threading/worker.h"
#include "iridium/io/fs/file_stream.h"

#include "sink.h"


namespace iridium {
namespace logging {
namespace implementation {


class CSinkFile: public ISink {
public:
    DEFINE_ENUM(
        TRotation,
        DAILY,
        TIMELY
    );

    DEFINE_IMPLEMENTATION(CSinkFile)
    CSinkFile(TEvent::TLevel const &level, std::string const &path, TRotation const &rotation = TRotation::UNKNOWN);

    void initialize() override;
    void finalize() override;
    void log(TEvent::TConstSharedPtr const &event) override;

private:
    TEvent::TLevel  m_level;
    std::string     m_path;
    std::string     m_path_original;
    TRotation       m_rotation;
    std::chrono::system_clock::time_point
                    m_last_initialization_time;
    io::fs::IFileStreamWriter::TSharedPtr
                    m_file_writer;
};


} // implementation
} // logging
} // iridium


#endif // HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573
