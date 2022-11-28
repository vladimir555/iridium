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


class CSinkFile: public CSink {
public:
    DEFINE_IMPLEMENTATION(CSinkFile)
    CSinkFile(TEvent::TLevel const &level, std::string const &file_name);

private:
    class CWorkerHandler: public threading::IWorkerPusher<TEvent>::IHandler {
    public:
        DEFINE_IMPLEMENTATION(CWorkerHandler)
        CWorkerHandler(TEvent::TLevel const &level, std::string const &file_name);
        void initialize() override;
        void finalize() override;
        typedef threading::IWorkerPusher<TEvent>::TInputItems TInputItems;
        void handle(TInputItems const &e) override;
    private:
        std::string m_file_name;
        std::string m_file_name_original;
        bool        m_is_rotation_by_day;
        std::chrono::system_clock::time_point
                    m_last_initialization_time;
        io::fs::IFileStreamWriter::TSharedPtr
                    m_file_writer;
    };
};


} // implementation
} // logging
} // iridium


#endif // HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573
