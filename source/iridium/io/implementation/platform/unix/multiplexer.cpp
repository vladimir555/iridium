#include "multiplexer.h"


#ifdef UNIX_PLATFORM


#include <sys/stat.h>


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

    m_pollfds.push_back({ m_wake_pipe[0], POLLIN, 0 });

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
        auto const &stream      = stream_to_handle.stream;
        auto operation          = stream_to_handle.is_add_action ? Event::TOperation::OPEN : Event::TOperation::CLOSE;
        auto map_type_handle    = stream->getHandles();

        if (auto pid = static_cast<int>(map_type_handle[IStream::THandleType::PID])) {
            if (stream_to_handle.is_add_action)
                m_map_pid_stream[pid] = stream;
            else
                m_map_pid_stream.erase(pid);
        }

        std::unordered_map<int, short> map_fd_mask;

        if (auto fd = map_type_handle[IStream::THandleType::READER])
            map_fd_mask[fd] |= POLLIN;

        if (auto fd = map_type_handle[IStream::THandleType::WRITER])
            map_fd_mask[fd] |= POLLOUT;

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

    {
        LOCK_SCOPE();

        if (!m_is_initialized)
            return events;

        events.splice(events.end(), applyPendingChanges());
    }

    int timeout_ms = static_cast<int>(
        std::chrono::duration_cast<std::chrono::milliseconds>(m_timeout).count());

    if (timeout_ms < 1)
        timeout_ms = 1;

    int result = poll(m_pollfds.data(), static_cast<nfds_t>(m_pollfds.size()), timeout_ms);

    {
        LOCK_SCOPE();

        if (result < 0) {
            if (errno == EINTR)
                return events;
            throw std::runtime_error("poll error: " + std::string(strerror(errno)));
        }

        if (result == 0)
            return events; // Таймаут

        for (auto &pfd : m_pollfds) {
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

            if (pfd.revents & (POLLERR | POLLNVAL))
                events.push_back(Event::create(stream, Event::TOperation::ERROR_, Event::TStatus::BEGIN));

            if (pfd.revents & POLLHUP)
                events.push_back(Event::create(stream, Event::TOperation::CLOSE, Event::TStatus::BEGIN));

            if (pfd.revents & POLLIN)
                events.push_back(Event::create(stream, Event::TOperation::WRITE, Event::TStatus::BEGIN));

            if (pfd.revents & POLLOUT)
                events.push_back(Event::create(stream, Event::TOperation::READ, Event::TStatus::BEGIN));
        }
    }

    return events;
}


void CMultiplexer::subscribe(IStream::TSharedPtr const &stream) {
    LOCK_SCOPE();

    m_streams_to_handle->push(
        { stream, true }
    );

    if (m_wake_pipe[1] >= 0) {
        char byte = 0;
        write(m_wake_pipe[1], &byte, 1);
    }
}


void CMultiplexer::unsubscribe(IStream::TSharedPtr const &stream) {
    LOCK_SCOPE();

    m_streams_to_handle->push(
        { stream, false }
    );

    if (m_wake_pipe[1] >= 0) {
        char byte = 0;
        write(m_wake_pipe[1], &byte, 1);
    }
}


void CMultiplexer::wake(Event::TSharedPtr const &event) {
    LOCK_SCOPE();

    m_wake_events->push(event);

    if (m_wake_pipe[1] >= 0) {
        char byte = 0;
        write(m_wake_pipe[1], &byte, 1);
    }
}


void CMultiplexer::wake(std::list<Event::TSharedPtr> const &events) {
    LOCK_SCOPE();

    for (auto const &event : events)
        m_wake_events->push(event);

    if (m_wake_pipe[1] >= 0) {
        char byte = 0;
        write(m_wake_pipe[1], &byte, 1);
    }
}


} // namespace iridium::io::implementation::platform::unix_


#endif // UNIX_PLATFORM
