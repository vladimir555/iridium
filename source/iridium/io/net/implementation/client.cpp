//#include "client.h"


//#include "iridium/io/implementation/stream_pool.h"
//#include "iridium/io/implementation/listener.h"
//#include "iridium/io/net/implementation/socket_client.h"


//using iridium::io::implementation::CStreamPool;
//using iridium::io::implementation::CListener;


//namespace iridium {
//namespace io {
//namespace net {
//namespace implementation {


//CClient::CClient(URL const &url, protocol::ISession::TSharedPtr const &session)
//:
////    m_listener      (CListener::create()),
//    m_stream_pool   (CStreamPool::create()),
////    m_stream        (CSocketClient::create(url, m_listener)),
//    m_session       (session)
//{}


//void CClient::initialize() {
//    m_stream_pool->initialize();
//    m_stream_pool->add(m_session);
//}


//void CClient::finalize() {
//    m_stream_pool->del(m_session);
//    m_stream_pool->finalize();
//}


//} // implementation
//} // net
//} // io
//} // iridium
