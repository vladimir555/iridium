#ifndef HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573
#define HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573


#include <string>

#include "utility/threading/implementation/worker.h"
#include "utility/fs/text_writer.h"

#include "sink.h"


namespace utility {
namespace logging {
namespace implementation {


class CSinkFile:
    public std::enable_shared_from_this<CSinkFile>,
    public CSink,
    public threading::IWorkerHandler<TEvent>
{
public:
    DEFINE_CREATE(CSinkFile)
    CSinkFile(TEvent::TLevel const &level, std::string const &file_name);
    virtual ~CSinkFile() = default;

private:
    void handleStart() override;
    void handleStop() override;
    void handleItems(TItems const &e) override;

    std::string m_file_name;
    std::string m_file_name_original;
    bool        m_is_rotation_by_day;
    std::chrono::high_resolution_clock::time_point
                m_last_initialization_time;
    fs::ITextWriter::TSharedPtr
                m_text_file_writer;
};


} // implementation
} // logging
} // utility


#endif // HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573
