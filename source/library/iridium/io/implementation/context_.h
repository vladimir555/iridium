#ifndef HEADER_CONTEXT_3763EB60_69EF_4930_9328_F8727441990E
#define HEADER_CONTEXT_3763EB60_69EF_4930_9328_F8727441990E


// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "iridium/io/event_.h"
#include "iridium/io/protocol_.h"
#include "iridium/io/context_.h"
#include "iridium/io/pipe_.h"
#include "iridium/threading/synchronized.h"
#include "iridium/threading/async_queue.h"

#include <chrono>


namespace iridium::io::implementation {


class CContext_:
    public IContext,
    public IPipeManager,
    public threading::Synchronized<std::mutex>,
    public std::enable_shared_from_this<CContext_>
{
public:
    DEFINE_CREATE(CContext_)

    CContext_(IStream::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol);

    void pushEvent(Event::TSharedPtr const &event) override;
    std::list<Event::TSharedPtr>
         popEvents() override;

    std::list<Event::TSharedPtr>
         checkOutdatedStreams() override;

    bool update(Event::TSharedPtr const &event) override;
    // bool transmit  (Event::TSharedPtr const &event) override;
    IPipe::TSharedPtr
        getPipe(Event::TSharedPtr const &event) override;

    void createPipe(std::string const &name) override;
    void removePipe(std::string const &name) override;
    void updatePipe(std::string const &name, IStreamReader::TSharedPtr const &reader) override;
    void updatePipe(std::string const &name, IStreamWriter::TSharedPtr const &writer) override;
    void updatePipe(std::string const &name, IStreamReader::TSharedPtr const &reader,
                                             IStreamWriter::TSharedPtr const &writer) override;
    void remove() override;

private:
    void removePipe(IPipe::TSharedPtr const &pipe);
    void removeStream(IStream::TSharedPtr const &stream, bool const &is_send_close_event = true);

    threading::IAsyncQueue<Event::TSharedPtr>::TSharedPtr
        m_events;
    IProtocol::TSharedPtr
        m_protocol;

    std::unordered_map<std::string, IPipe::TSharedPtr>
        m_map_name_pipe;
    std::unordered_map<IStream::TSharedPtr, IPipe::TSharedPtr>
        m_map_stream_pipe;
    std::unordered_map<IStream::TSharedPtr, std::chrono::system_clock::time_point>
        m_map_stream_timestamp;
};


} // iridium::io::implementation


#endif // HEADER_CONTEXT_3763EB60_69EF_4930_9328_F8727441990E
