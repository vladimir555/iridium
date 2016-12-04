#ifndef HEADER_SOCKET_268F1DCB_ADF2_4B30_A901_3F22D2C3774E
#define HEADER_SOCKET_268F1DCB_ADF2_4B30_A901_3F22D2C3774E


namespace utility {
namespace networking {
namespace server {
namespace socket {


class ISocket {
public:
    virtual void initialize() = 0;
    virtual void finalize() = 0;

    //todo:
    virtual void handle() = 0;

};


} // socket
} // server
} // networking
} // utility


#endif // HEADER_SOCKET_268F1DCB_ADF2_4B30_A901_3F22D2C3774E
