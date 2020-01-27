#ifndef HEADER_PEERS_A9AA2AC0_C232_4281_BBC0_E3CF8E92FD4A
#define HEADER_PEERS_A9AA2AC0_C232_4281_BBC0_E3CF8E92FD4A


#include "iridium/threading/synchronized.h"
#include "iridium/io/stream.h"
#include "iridium/io/transmitter.h"
#include "iridium/io/protocol/protocol_factory.h"

#include <map>
#include <atomic>


namespace iridium {
namespace io {
namespace net {
namespace implementation {


struct Peer {
    DEFINE_CREATE(Peer)
    protocol::IProtocolHandler::TSharedPtr  protocol_handler;
    io::ITransmitter::TSharedPtr            transmitter;
    Event::TSharedPtr                       event;
    std::atomic<bool>                       is_in_processing;
};


class Peers:
    public threading::Synchronized,
    public std::enable_shared_from_this<Peers> {
public:
    DEFINE_IMPLEMENTATION(Peers)
        Peers(
            protocol::IProtocolFactory::TSharedPtr  const &protocol_factory,
            IListener::TSharedPtr                   const &listener
        );
    Peer::TSharedPtr    get(IStream::TSharedPtr const &stream);
    void                set(IStream::TSharedPtr const &stream, Peer::TSharedPtr const &peer);
private:
    std::map<IStream::TSharedPtr, Peer::TSharedPtr> m_map_stream_peer;
    protocol::IProtocolFactory::TSharedPtr          m_protocol_factory;
    IListener::TSharedPtr                           m_listener;
};


class CPeerTransmitter: public ITransmitter {
public:
    DEFINE_IMPLEMENTATION(CPeerTransmitter)
    CPeerTransmitter(
        ITransmitter::TSharedPtr    const &transmitter,
        Peers::TSharedPtr           const &peers,
        Peer::TSharedPtr            const &peer
    );

    void setReader(IStreamReader::TSharedPtr const &reader) override;
    void setWriter(IStreamWriter::TSharedPtr const &writer) override;
    IStreamReader::TSharedPtr getReader() const override;
    IStreamWriter::TSharedPtr getWriter() const override;

    bool transmit(Event::TSharedPtr const &event) override;

private:
    ITransmitter::TSharedPtr m_transmitter;
    Peers::TSharedPtr        m_peers;
    Peer::TSharedPtr         m_peer;
};


} // implementation
} // net
} // io
} // iridium


#endif // HEADER_PEERS_A9AA2AC0_C232_4281_BBC0_E3CF8E92FD4A
