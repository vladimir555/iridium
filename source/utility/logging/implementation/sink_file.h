#ifndef HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573
#define HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573


#include <string>

#include "utility/threading/implementation/worker.h"
#include "utility/fs/text_writer.h"

#include "sink.h"


namespace utility {
namespace logging {
namespace implementation {


///
class CSinkFile: public CSink {
public:
    DEFINE_CREATE(CSinkFile);
    ///
    CSinkFile(TEvent::TLevel const &level, std::string const &file_name);
    ///
    virtual ~CSinkFile() = default;

private:
    ///
    class CSinkWorker : public threading::implementation::CWorker<TEvent> {
    public:
        DEFINE_CREATE(CSinkWorker);
        ///
        CSinkWorker(std::string const &file_name, bool const &is_daily_rotation = true);
        ///
        virtual ~CSinkWorker() = default;
        ///
        virtual void initialize() override;
        ///
        virtual void finalize() override;

    private:
        ///
        void initializeInternal();
        ///
        void finalizeInternal();
        ///
        virtual void handleItems(TItems const &e) override;
        /////
        //::FILE     *m_file = nullptr;
        ///
        std::string m_file_name;
        ///
        std::string m_file_name_original;
        ///
        bool        m_is_rotation_by_day;
        ///
        time_t      m_last_initialization_time;
        ///
        fs::ITextWriter::TSharedPtr m_text_file_writer;
    };
};


} // implementation
} // logging
} // utility


#endif // HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573
