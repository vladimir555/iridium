// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SINK_175E1AB3_5BEE_4AFC_A1E2_DB31099E39EB
#define HEADER_SINK_175E1AB3_5BEE_4AFC_A1E2_DB31099E39EB


#include "iridium/pattern/non_copyable.h"
#include "iridium/threading/worker.h"
#include "iridium/logging/sink.h"


namespace iridium {
namespace logging {
namespace implementation {


// todo: CSinkAsync(ISink::TSharedPtr ... ) wrapper
// cached async sink
class CSinkAsync: public ISink, pattern::NonCopyable {
public:
    DEFINE_IMPLEMENTATION(CSinkAsync)
    CSinkAsync(ISink::TSharedPtr const &sink);
    void initialize() override;
    void finalize() override;
    void log(TEvent::TConstSharedPtr const &event) override;

private:
    typedef threading::IWorkerPusher<TEvent::TConstSharedPtr> IWorkerPusher;
    class CWorkerHandler: public IWorkerPusher::IHandler {
    public:
        DEFINE_IMPLEMENTATION(CWorkerHandler)
        CWorkerHandler(ISink::TSharedPtr const &sink);

        void initialize() override;
        void finalize() override;

        typedef IWorkerPusher::TInputItems TInputItems;
        void handle(TInputItems const &e) override;

    private:
        ISink::TSharedPtr m_sink;
    };
    
    IWorkerPusher::TSharedPtr m_worker;
};


} // implementation
} // logger
} // iridium


#endif // HEADER_SINK_175E1AB3_5BEE_4AFC_A1E2_DB31099E39EB
