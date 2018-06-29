//#ifndef HEADER_HTTP_83EE5E1A_5D87_40B6_8887_5E602ECCEE3D
//#define HEADER_HTTP_83EE5E1A_5D87_40B6_8887_5E602ECCEE3D


//#include "utility/net/server/http.h"
//#include "utility/parsing/parser.h"
//#include "socket.h"


//namespace utility {
//namespace net {
//namespace server {
//namespace implementation {


//class CHTTP: public IHTTP {
//public:
//    DEFINE_CREATE(CHTTP)
//    virtual ~CHTTP() = default;

//    CHTTP(URL const &url, IHTTP::THTTPHandlers const &http_handlers);

//    void initialize()   override;
//    void finalize()     override;

//private:
//    class CSocketHandler: public ISocket::TSocketStreamsHandler {
//    public:
//        DEFINE_CREATE(CSocketHandler)
//        virtual ~CSocketHandler() = default;

//        typedef ISocket::TSocketStreamsHandler::TItem   TSocketEvent;
//        typedef ISocket::TSocketStreamsHandler::TItems  TSocketsEvents;

//        CSocketHandler(IHTTPHandler::TSharedPtr const &http_handler);

//        TSocketsEvents    handle(TSocketsEvents const &items) override;
//        void        initialize() override;
//        void        finalize()  override;
//    private:
//        IHTTPHandler::TSharedPtr        m_http_handler;
//        parsing::IParser::TSharedPtr    m_parser;
//    };

//    ISocket::TSharedPtr m_socket;
//};


//} // implementation
//} // server
//} // net
//} // utility


//#endif // HEADER_HTTP_83EE5E1A_5D87_40B6_8887_5E602ECCEE3D
