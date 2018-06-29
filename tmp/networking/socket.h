#ifndef HEADER_SOCKET_423B8F1C_F93C_43CC_99D9_06CB1A1062CF
#define HEADER_SOCKET_423B8F1C_F93C_43CC_99D9_06CB1A1062CF


#include "utility/smart_ptr.h"
#include "utility/pattern/initializable.h"
#include "utility/convertion/convert.h"
#include "utility/pattern/fsm.h"

#include "url.h"
#include "types.h"

#include <vector>
#include <string>
#include <list>
#include <map>


namespace utility {
namespace net {


class ISocketStream {
public:
    DEFINE_INTERFACE(ISocketStream)

    typedef std::vector<uint8_t> TPacket;

    virtual size_t write(TPacket const &packet) = 0;
    // todo: expected pacet size parameter
    virtual TPacket read()      = 0;
    virtual void close()        = 0;
    virtual URL getURL() const  = 0;
};


class ISocket : public ISocketStream {
public:
    DEFINE_INTERFACE(ISocket)

    struct TEvent {
        DEFINE_SMART_PTR(TEvent)
        DEFINE_CREATE(TEvent)
        DEFINE_ENUM(TAction, ACCEPT, READ, WRITE, CLOSE)

        TEvent(TAction const &action, ISocketStream::TSharedPtr const &socket) : action(action), socket(socket) {};

        TAction action;
        ISocketStream::TSharedPtr socket;
    };

    typedef std::list<TEvent::TSharedPtr> TEvents;

    virtual void    open()          = 0;
    virtual void    connect()       = 0;
    virtual void    listen()        = 0;
    virtual TEvents accept()        = 0;
    virtual void    interrupt()     = 0;
};


} // net
} // utility


//DEFINE_CONVERT(utility::net::ISocketStream::TPacket, std::string)
//DEFINE_CONVERT(std::string, utility::net::ISocketStream::TPacket)


#endif // HEADER_SOCKET_423B8F1C_F93C_43CC_99D9_06CB1A1062CF
