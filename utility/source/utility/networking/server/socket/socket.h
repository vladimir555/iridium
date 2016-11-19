

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
