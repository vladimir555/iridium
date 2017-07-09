#ifndef HEADER_SINK_175E1AB3_5BEE_4AFC_A1E2_DB31099E39EB
#define HEADER_SINK_175E1AB3_5BEE_4AFC_A1E2_DB31099E39EB


#include "utility/pattern/non_copyable.h"
#include "utility/threading/worker.h"
#include "utility/logging/sink.h"


namespace utility {
namespace logging {
namespace implementation {


class CSink : public ISink, pattern::NonCopyable {
public:
    CSink(TEvent::TLevel const &level, threading::IWorkerHandler<TEvent>::TSharedPtr const &worker);
    virtual ~CSink() = default;
    virtual void initialize() override;
    virtual void finalize() override;
    virtual void log(TEvent const &event) override;
protected:
    template<typename T>
    static threading::IWorkerHandler<TEvent>::TSharedPtr getSharedThis(T this_) {
        typedef typename std::remove_pointer<T>::type TClass;
        std::shared_ptr<TClass const> ptr( this_, [](T){} );
        return std::const_pointer_cast<typename std::remove_const<TClass>::type>(this_->shared_from_this()); // ----->
    }

private:
    TEvent::TLevel const m_level;
    threading::IWorker<TEvent>::TSharedPtr m_worker;
};


} // implementation
} // logger
} // utility


#endif // HEADER_SINK_175E1AB3_5BEE_4AFC_A1E2_DB31099E39EB
