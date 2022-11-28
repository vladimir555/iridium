#ifndef HEADER_SESSION_BB5946FC_0AA1_4747_AE53_07DB1F105A72
#define HEADER_SESSION_BB5946FC_0AA1_4747_AE53_07DB1F105A72


#include "iridium/smart_ptr.h"
#include "iridium/pattern/initializable.h"

#include "pipe.h"
#include "event.h"

#include <unordered_map>


namespace iridium {
namespace io {


//class ISession {
//public:
//    DEFINE_INTERFACE(ISession)
//    virtual void set(int const &index, IPipeStreams::TSharedPtr const &pipe) = 0;
//    virtual void del(int const &index) = 0;
//};


//class ISessionHandler: public ISession {
//public:
//    virtual void process(IEvent::TSharedPtr const &event) = 0;
//};


// todo:
class IProtocol {
public:
    DEFINE_INTERFACE(IProtocol)
    virtual bool control(IEvent::TSharedPtr const &event, std::unordered_map<int, IPipe::TSharedPtr> &pipes) = 0;
};


class ISessionManager: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(ISessionManager)
    ///
    virtual void manage(
        IStreamPort::TSharedPtr const &stream,
        IProtocol::TSharedPtr const &protocol) = 0;
    ///
    virtual bool wait(std::chrono::nanoseconds const &timeout) = 0;
};


} // io
} // iridium


#endif // HEADER_SESSION_BB5946FC_0AA1_4747_AE53_07DB1F105A72
