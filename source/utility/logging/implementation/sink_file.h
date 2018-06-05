#ifndef HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573
#define HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573


#include "utility/threading/worker.h"
#include "utility/fs/file_writer.h"

#include "sink.h"


namespace utility {
namespace logging {
namespace implementation {


class CSinkFile: public CSink {
public:
    DEFINE_IMPLEMENTATION(CSinkFile)
    CSinkFile(TEvent::TLevel const &level, std::string const &file_name);

private:
    class CWorkerHandler: public threading::IWorkerHandler<TEvent> {
    public:
        DEFINE_IMPLEMENTATION(CWorkerHandler)
        CWorkerHandler(TEvent::TLevel const &level, std::string const &file_name);
        void initialize() override;
        void finalize() override;
        TItems handle(TItems const &e) override;
    private:
        std::string m_file_name;
        std::string m_file_name_original;
        bool        m_is_rotation_by_day;
        std::chrono::system_clock::time_point
                    m_last_initialization_time;
        fs::IFileWriter::TSharedPtr
                    m_text_file_writer;
    };
};


} // implementation
} // logging
} // utility


#endif // HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573
