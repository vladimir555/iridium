#include "socket_client.h"

#include "socket.h"

#include "iridium/threading/implementation/async_queue.h"
#include "iridium/threading/implementation/thread.h"
#include "iridium/threading/implementation/mutex.h"
#include "iridium/threading/implementation/worker.h"
#include "iridium/io/implementation/listener.h"
#include "iridium/convertion/convert.h"

#include "iridium/logging/logger.h"


using iridium::threading::implementation::CAsyncQueue;
using iridium::threading::implementation::CThread;
using iridium::threading::implementation::CMutex;
using iridium::threading::implementation::CWorker;
using iridium::threading::Synchronized;
using iridium::io::implementation::CListener;
using iridium::convertion::convert;
using std::string;


static size_t const DEFAULT_BUFFER_SIZE = 8192;


namespace iridium {
namespace io {
namespace net {
namespace implementation {


CStreamPortSynchronized::CStreamPortSynchronized(IStreamPort::TSharedPtr const &stream)
:
    Synchronized(CMutex::create()),
    m_stream    (stream)
{}


Buffer::TSharedPtr CStreamPortSynchronized::read(size_t const &size) {
    LOCK_SCOPE_FAST;
    return m_stream->read(size);
}


size_t CStreamPortSynchronized::write(Buffer::TSharedPtr const &buffer) {
    LOCK_SCOPE_FAST;
    return m_stream->write(buffer);
}


int CStreamPortSynchronized::getID() const {
    LOCK_SCOPE_FAST;
    return m_stream->getID();
}


void CStreamPortSynchronized::initialize() {
    LOCK_SCOPE_FAST;
    m_stream->initialize();
}


void CStreamPortSynchronized::finalize() {
    LOCK_SCOPE_FAST;
    m_stream->finalize();
}


CSocketClient::CSocketClient(URL const &url)
:
    m_socket            (CStreamPortSynchronized::create(CSocket::create(url, false))),
    m_read_buffers      (CAsyncQueue<Buffer::TSharedPtr>::create()),
    m_socket_worker     (CWorker<Command>::create(
                            "client socket listener " + convert<string>(url),
                            CSocketHandler::create(m_socket, m_read_buffers))),
    m_listener_thread   (CThread::create(
                            CListenerHandler::create(m_socket, m_socket_worker),
                            "client socket listener " + convert<string>(url)))
{}


Buffer::TSharedPtr CSocketClient::read(size_t const &size) {
    return Buffer::create(m_read_buffers->pop(size > 0));
}


size_t CSocketClient::write(Buffer::TSharedPtr const &buffer) {
    Command command;
    command.event = Event::create(Event::TType::WRITE, m_socket);
    command.write_buffer = buffer;
    m_socket_worker->push(command);
    return buffer->size();
}


void CSocketClient::initialize() {
    m_socket->initialize();
    m_socket_worker->initialize();
    m_listener_thread->initialize();
}


void CSocketClient::finalize() {
    m_listener_thread->finalize();
    m_socket_worker->finalize();
    m_socket->finalize();
}


int CSocketClient::getID() const {
    return m_socket->getID();
}


CSocketClient::CSocketHandler::CSocketHandler(
    IStreamPort::TSharedPtr const &socket,
    TBufferQueuePtr         const &read_buffers)
:
    m_socket        (socket),
    m_read_buffers  (read_buffers)
{}


void CSocketClient::CSocketHandler::initialize() {
}


void CSocketClient::CSocketHandler::finalize() {
}


CSocketClient::CSocketHandler::TItems
CSocketClient::CSocketHandler::handle(TItems const &commands) {
    TItems commands_;
    for (auto const &command: commands) {
        if (command.event->stream->getID() == m_socket->getID()) {
            LOGT << "event: " << command.event->type << " fd " << command.event->stream->getID();
            if (command.event->type == Event::TType::READ) {
                auto buffer = m_socket->read(command.read_size);
                if  (buffer)
                    m_read_buffers->push(buffer);
            }
            if (command.event->type == Event::TType::WRITE) {
                auto size = m_socket->write(command.write_buffer);
                if (size > 0 && command.write_buffer->size() > size) {
                    auto command_ = command;
                    command_.write_buffer = Buffer::create(command.write_buffer->begin() + size, command.write_buffer->end());
                    commands_.push_back(command_);
                }
            }
        }
    }
    return commands_;
}


CSocketClient::CListenerHandler::CListenerHandler(
    IStreamPort::TSharedPtr const &socket,
    TSocketWorkerPtr        const &socket_worker)
:
    m_socket        (socket),
    m_listener      (CListener::create()),
    m_socket_worker (socket_worker)
{}


void CSocketClient::CListenerHandler::run(std::atomic<bool> &is_running) {
    while(is_running) {
        for (auto const &event: m_listener->wait()) {
            Command command;
            command.event = event;
            m_socket_worker->push(command);
        }
    }
}


void CSocketClient::CListenerHandler::initialize() {
    m_listener->initialize();
    m_listener->add(m_socket);
}


void CSocketClient::CListenerHandler::finalize() {
    m_listener->del(m_socket);
    m_listener->finalize();
}


} // implementation
} // net
} // io
} // iridium
