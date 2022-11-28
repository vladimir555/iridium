//#ifndef HEADER_REQUEST_HANDLER_43E16DA0_43FF_4E9E_A7AA_DDBAF520DE8A
//#define HEADER_REQUEST_HANDLER_43E16DA0_43FF_4E9E_A7AA_DDBAF520DE8A


//#include "iridium/io/protocol/session.h"
//#include "iridium/io/url.h"
//#include "iridium/io/buffer.h"


//#include "iridium/threading/synchronized.h"
//namespace iridium {
//namespace io {
//namespace protocol {
//namespace http {
//namespace implementation {


//class CClientSession: public ISession {
//public:
//    DEFINE_IMPLEMENTATION(CClientSession)
//    CClientSession(URL const &url);
//    IStream::TSharedPtr getStream   (IListener::TSharedPtr const &listener) override;
//    IPipe::TSharedPtr   getPipe     (IListener::Event::TConstSharedPtr const &event) override;
//private:
//    IListener::Event::TConstSharedPtr   m_event;
//    IStreamPort::TSharedPtr             m_socket;
//    IPipe::TSharedPtr                   m_pipe;
//    URL                                 m_url;

//    IStreamReader::TSharedPtr           m_request_stream;
//    IStreamWriter::TSharedPtr           m_response_stream;

//    Buffer::TSharedPtr                  m_request_buffer;
//    Buffer::TSharedPtr                  m_response_buffer;

//    int count = 0;
//};


//} // implementation
//} // http
//} // protocol
//} // io
//} // iridium


//#endif // HEADER_REQUEST_HANDLER_43E16DA0_43FF_4E9E_A7AA_DDBAF520DE8A
