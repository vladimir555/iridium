//#include "peers.h"

//#include "iridium/threading/implementation/recursive_mutex.h"
//#include "iridium/threading/synchronized_scope.h"
//#include "iridium/io/implementation/transmitter.h"


//using iridium::threading::implementation::CRecursiveMutex;
//using iridium::io::implementation::CTransmitter;


//namespace iridium {
//namespace io {
//namespace net {
//namespace implementation {


//Peers::Peers(
//    protocol::IProtocolFactory::TSharedPtr  const &protocol_factory,
//    IListener::TSharedPtr                   const &listener)
//:
//    Synchronized        (CRecursiveMutex::create()),
//    m_protocol_factory  (protocol_factory),
//    m_listener          (listener)
//{}


//Peer::TSharedPtr Peers::get(IStream::TSharedPtr const &stream) {
//    LOCK_SCOPE
//        auto peer = m_map_stream_peer[stream];
//    if(!peer) {
//        peer = Peer::create();
//        peer->is_in_processing = false;
//    }

//    if(!peer->protocol_handler)
//        peer->protocol_handler = m_protocol_factory->createProtocolHandler();

//    if(!peer->transmitter)
//        peer->transmitter =
//        CPeerTransmitter::create(
//            CTransmitter::create(m_listener), shared_from_this(), peer);

//    return peer; // ----->
//}


//void Peers::set(IStream::TSharedPtr const &stream, Peer::TSharedPtr const &peer) {
//    LOCK_SCOPE
//        m_map_stream_peer[stream] = peer;
//}


//CPeerTransmitter::CPeerTransmitter(
//    ITransmitter::TSharedPtr    const &transmitter,
//    Peers::TSharedPtr           const &peers,
//    Peer::TSharedPtr            const &peer)
//:
//    m_transmitter   (transmitter),
//    m_peers         (peers),
//    m_peer          (peer)
//{}


//void CPeerTransmitter::setReader(IStreamReader::TSharedPtr const &reader) {
//    m_transmitter->setReader(reader);
//    m_peers->set(reader, m_peer);
//}


//void CPeerTransmitter::setWriter(IStreamWriter::TSharedPtr const &writer) {
//    m_transmitter->setWriter(writer);
//    m_peers->set(writer, m_peer);
//}


//IStreamReader::TConstSharedPtr CPeerTransmitter::getReader() const {
//    return m_transmitter->getReader(); // ----->
//}


//IStreamWriter::TConstSharedPtr CPeerTransmitter::getWriter() const {
//    return m_transmitter->getWriter(); // ----->
//}


//bool CPeerTransmitter::transmit(Event::TSharedPtr const &event) {
//    return m_transmitter->transmit(event); // ----->
//}


//} // implementation
//} // net
//} // io
//} // iridium
