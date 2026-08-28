// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include "multiplexer.h"


#ifdef UNIX_PLATFORM


#include <sys/stat.h>


#include <iridium/logging/logger.h>
#include <iridium/enum.h>
#include <poll.h>


// poll.h events
DEFINE_ENUM(
    TPollEvent,
    IN          = POLLIN,
    PRI         = POLLPRI,
    OUT         = POLLOUT,
    RDNORM      = POLLRDNORM,
    // WRNORM      = POLLWRNORM,
    RDBAND      = POLLRDBAND,
    WRBAND      = POLLWRBAND,
#ifdef MACOS_PLATFORM
    EXTEND      = POLLEXTEND,
    ATTRIB      = POLLATTRIB,
    NLINK       = POLLNLINK,
    WRITE       = POLLWRITE,
#endif // MACOS_PLATFORM
    ERR         = POLLERR,
    HUP         = POLLHUP,
    NVAL        = POLLNVAL
)
// IMPLEMENT_ENUM(TPollEvent)


namespace iridium::io::implementation::platform::unix_ {


CMultiplexer::CMultiplexer(std::chrono::microseconds const &timeout)
:
    m_timeout(timeout)
{}


void CMultiplexer::initialize() {
    LOCK_SCOPE();

    if (m_is_initialized)
        throw std::runtime_error("multiplexer already initialized");

    assertOK(pipe(m_wake_pipe), "pipe(wake)");

    for (int i = 0; i < 2; i++) {
        int flags = fcntl(m_wake_pipe[i], F_GETFL, 0);
        fcntl(m_wake_pipe[i], F_SETFL, flags | O_NONBLOCK);
        fcntl(m_wake_pipe[i], F_SETFD, FD_CLOEXEC);
    }

    m_pollfds.push_back({ m_wake_pipe[0], TPollEvent::IN, 0 });

    m_is_initialized = true;
}


void CMultiplexer::finalize() {
    LOCK_SCOPE();

    if (!m_is_initialized)
        return;

    if (m_wake_pipe[0] >= 0)
        close(m_wake_pipe[0]);
    if (m_wake_pipe[1] >= 0)
        close(m_wake_pipe[1]);

    m_wake_pipe[0] = m_wake_pipe[1] = -1;

    m_pollfds.clear();
    m_map_fd_stream.clear();
    m_map_pid_stream.clear();
    m_streams_to_handle->pop(false);

    m_is_initialized = false;
}


std::list<Event::TSharedPtr> CMultiplexer::applyPendingChanges() {
    std::list<Event::TSharedPtr> events;

    for (auto const &stream_to_handle : m_streams_to_handle->pop(false)) {
        auto const &stream
            = stream_to_handle.stream;
        auto operation
            = stream_to_handle.is_add_action ? Event::TOperation::OPEN : Event::TOperation::CLOSE;
        auto map_type_handle
            = stream->getHandles();

        if (auto pid = static_cast<int>(map_type_handle[IStream::THandleType::PID])) {
            if (stream_to_handle.is_add_action)
                m_map_pid_stream[pid] = stream;
            else
                m_map_pid_stream.erase(pid);
        }

        std::unordered_map<int, short> map_fd_mask;

        if (auto fd = map_type_handle[IStream::THandleType::READER])
            map_fd_mask[fd] |= TPollEvent::IN;

        if (auto fd = map_type_handle[IStream::THandleType::WRITER])
            map_fd_mask[fd] |= TPollEvent::OUT;

        for (auto const &fd_mask: map_fd_mask) {
            if (stream_to_handle.is_add_action) {
                if (m_map_fd_stream.emplace(fd_mask.first, stream).second) {
                    m_pollfds.push_back(
                        { fd_mask.first, fd_mask.second, 0 }
                    );
                }
            } else {
                if (m_map_fd_stream.erase(fd_mask.first)) {
                    auto i = std::find_if(
                        m_pollfds.begin(),
                        m_pollfds.end(),
                        [&fd_mask] (struct pollfd const &p) {
                            return p.fd == fd_mask.first;
                        }
                    );
                    if (i != m_pollfds.end())
                        m_pollfds.erase(i);
                }
            }
        }

        events.push_back(Event::create(stream, operation, Event::TStatus::END));
    }

    return events;
}


void CMultiplexer::processWakePipe(std::list<Event::TSharedPtr> &events) {
    char buffer[64];

    // clean pipe
    while (read(m_wake_pipe[0], buffer, sizeof(buffer)) > 0);

    events.splice(events.end(), m_wake_events->pop(false));
}


std::list<Event::TSharedPtr> CMultiplexer::waitEvents() {
    std::list<Event::TSharedPtr> events;
    std::vector<struct pollfd> pollfds_copy;

    {
        if (!m_is_initialized)
            return events;

        LOCK_SCOPE();
        events.splice(events.end(), applyPendingChanges());
        pollfds_copy = m_pollfds;
    }

    int timeout_ms = static_cast<int>(
        std::chrono::duration_cast<std::chrono::milliseconds>(m_timeout).count());

    if (timeout_ms < 1)
        timeout_ms = 1;

    int result = poll(pollfds_copy.data(), static_cast<nfds_t>(pollfds_copy.size()), timeout_ms);

    {
        LOCK_SCOPE();

        if (result < 0) {
            if (errno == EINTR)
                return events;
            throw std::runtime_error("poll error: " + std::string(strerror(errno)));
        }

        // timeout
        if (result == 0)
            return events;

        for (auto &pfd : pollfds_copy) {
            if (pfd.revents == 0)
                continue;

            // wake pipe
            if (pfd.fd == m_wake_pipe[0]) {
                processWakePipe(events);
                continue;
            }

            auto i = m_map_fd_stream.find(pfd.fd);
            if (i == m_map_fd_stream.end())
                continue;

            auto const &stream = i->second;
            // LOGT << "event fd: " << pfd.fd << ", stream: "
            // << stream->getURI() << ", revents: " << TPollEvent(pfd.revents).convertToFlagsString();

            // process error first (similar to EV_ERROR in kevent)
            if (pfd.revents & (TPollEvent::ERR | TPollEvent::NVAL)) {
                events.push_back(Event::create(stream, Event::TOperation::ERROR_, Event::TStatus::END));
                continue;  // don't process other events if there's an error
            }

            // POLLHUP indicates the other end closed - generate READ event to ensure all data is read
            if (pfd.revents & TPollEvent::HUP)
                events.push_back(Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::BEGIN));

            // always try to read data first, even if POLLHUP is set
            if (pfd.revents & TPollEvent::IN)
                events.push_back(Event::create(stream, Event::TOperation::READ, Event::TStatus::BEGIN));

            if (pfd.revents & TPollEvent::OUT)
                events.push_back(Event::create(stream, Event::TOperation::WRITE, Event::TStatus::BEGIN));
        }
    }

    return events;
}


void CMultiplexer::subscribe(IStream::TSharedPtr const &stream) {
    m_streams_to_handle->push(
        { stream, true }
    );

    LOCK_SCOPE();
    if (m_wake_pipe[1] >= 0) {
        char byte = 0;
        write(m_wake_pipe[1], &byte, 1);
    }
}


void CMultiplexer::unsubscribe(IStream::TSharedPtr const &stream) {
    m_streams_to_handle->push(
        { stream, false }
    );

    LOCK_SCOPE();
    if (m_wake_pipe[1] >= 0) {
        char byte = 0;
        write(m_wake_pipe[1], &byte, 1);
    }
}


void CMultiplexer::wake(Event::TSharedPtr const &event) {
    m_wake_events->push(event);

    LOCK_SCOPE();
    if (m_wake_pipe[1] >= 0) {
        char byte = 0;
        write(m_wake_pipe[1], &byte, 1);
    }
}


void CMultiplexer::wake(std::list<Event::TSharedPtr> const &events) {
    LOCK_SCOPE();

    m_wake_events->push(events);

    if (m_wake_pipe[1] >= 0) {
        char byte = 0;
        write(m_wake_pipe[1], &byte, 1);
    }
}


} // namespace iridium::io::implementation::platform::unix_


#endif // UNIX_PLATFORM
