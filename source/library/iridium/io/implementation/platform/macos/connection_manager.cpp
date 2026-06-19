#include "connection_manager.h"


#ifdef MACOS_PLATFORM


#include "iridium/assert.h"
#include "iridium/convertion/convert.h"
#include "iridium/logging/logger.h"
#include "iridium/threading/implementation/thread.h"
#include "iridium/io/implementation/context.h"

#include <chrono>
#include <vector>
#include <unordered_set>
#include <algorithm>

#include <signal.h>
#include <csignal>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#ifdef MACOS_PLATFORM
#include <sys/ucred.h>
#endif


using iridium::convertion::convert;
using std::chrono::duration_cast;
using std::chrono::system_clock;
using std::chrono::seconds;
using std::chrono::nanoseconds;
using std::string;


// DEFINE_ENUM(
//     TEventFlag,
//     ADD         = EV_ADD,
//     ENABLE      = EV_ENABLE,
//     DISABLE     = EV_DISABLE,
//     DISPATCH    = EV_DISPATCH,
//     DELETE      = EV_DELETE,
//     RECEIPT     = EV_RECEIPT,
//     ONESHOT     = EV_ONESHOT,
//     CLEAR       = EV_CLEAR,
//     EOF_        = EV_EOF,
//     ERROR_      = EV_ERROR
// )
// // IMPLEMENT_ENUM(TEventFlag)

// // struct kevent.filter
// DEFINE_ENUM(
//     TEventFilter,
//     READ        = EVFILT_READ,
//     WRITE       = EVFILT_WRITE,
//     AIO         = EVFILT_AIO,
//     VNODE       = EVFILT_VNODE,
//     PROC        = EVFILT_PROC,
//     SIGNAL      = EVFILT_SIGNAL,
//     TIMER       = EVFILT_TIMER,
//     FS          = EVFILT_FS,
//     USER        = EVFILT_USER,
//     SYSCOUNT    = EVFILT_SYSCOUNT
// )
// // IMPLEMENT_ENUM(TEventFilter)

// // EVFILT_PROC
// DEFINE_ENUM(
//     TEventProcFFlag,
//     EXIT        = NOTE_EXIT,
//     FORK        = NOTE_FORK,
//     EXEC        = NOTE_EXEC,
//     TRACK       = NOTE_TRACK,
//     TRACKERR    = NOTE_TRACKERR
// )
// // IMPLEMENT_ENUM(TEventProcFFlag)

// // EVFILT_VNODE
// DEFINE_ENUM(
//     TEventVNodeFFlag,
//     DELETE      = NOTE_DELETE,
//     WRITE       = NOTE_WRITE,
//     EXTEND      = NOTE_EXTEND,
//     ATTRIB      = NOTE_ATTRIB,
//     LINK        = NOTE_LINK,
//     RENAME      = NOTE_RENAME,
//     REVOKE      = NOTE_REVOKE
// )
// // IMPLEMENT_ENUM(TEventVNodeFFlag)

// // EVFILT_USER
// DEFINE_ENUM(
//     TEventUserFFlag,
//     TRIGGER     = NOTE_TRIGGER,
//     FFNOP       = NOTE_FFNOP,
//     FFAND       = NOTE_FFAND,
//     FFOR        = NOTE_FFOR,
//     FFCOPY      = NOTE_FFCOPY,
//     // FFCTRLMASK  = NOTE_FFCTRLMASK,
//     FFLAGSMASK  = NOTE_FFLAGSMASK
// )
// // IMPLEMENT_ENUM(TEventUserFFlag)


// std::string toString(struct kevent const &event) {
//     std::string fflags;

//     // enum fflags depends filter
//     switch (event.filter) {
//         case EVFILT_PROC:
//             fflags = TEventProcFFlag
//                 (event.fflags).convertToFlagsString();
//             break;
//         case EVFILT_VNODE:
//             fflags = TEventVNodeFFlag
//                 (event.fflags).convertToFlagsString();
//             break;
//         case EVFILT_USER:
//             fflags = TEventUserFFlag
//                 (event.fflags).convertToFlagsString();
//             break;
//         default:
//             fflags = convert<std::string, uint32_t>(event.fflags, 16);
//             break;
//     }

//     return
//         "{ ident: "     + convert<std::string>(event.ident)
//         + ", filter: "  + convert<std::string, TEventFilter>(event.filter)
//         + ", flags: "   + TEventFlag(event.flags).convertToFlagsString()
//         + ", fflags: "  + fflags
//         + ", data: "    + convert<std::string,  intptr_t>(event.data)
//         + ", udata: "   + convert<std::string, uintptr_t>(reinterpret_cast<uintptr_t>(event.udata)) + " }";
// }


// DEFINE_CONVERT(std::string, struct kevent);
// IMPLEMENT_CONVERT(std::string, struct kevent, toString);


namespace iridium::io::implementation::platform {


static system_clock::duration
    constexpr DEFAULT_KEVENT_TIMEOUT    = seconds(1);
static int
    constexpr DEFAULT_IDENT_WAKEUP      = 65535;
static size_t
    constexpr DEFAULT_EVENTS_LIMIT      = 65535;
static size_t
    constexpr DEFAULT_BUFFER_SIZE       = 8192;


CConnectionManager::CConnectionManager()
:
    m_thread(
        threading::implementation::CThread::create(
            "kevent", CKEventRunnable::create(this))),
    m_kqueue(0)
{}


void CConnectionManager::initialize() {
    try {
        std::signal(SIGPIPE, SIG_IGN);
        if (m_kqueue)
            throw std::runtime_error("kqueue is not finalized");

        m_kqueue = assertOK(kqueue(), "kqueue create error");

        struct kevent event;

        EV_SET(&event, DEFAULT_IDENT_WAKEUP, EVFILT_USER, EV_ADD | EV_CLEAR, 0, 0, nullptr);

        assertOK(
            kevent(m_kqueue, &event, 1, nullptr, 0, nullptr),
           "kevent user registration error");

        m_thread->initialize();
    } catch (std::exception const &e) {
        throw std::runtime_error(
            std::string("connection manager initialization error: ") + e.what()); // ----->
    }
}


void CConnectionManager::finalize() {
    try {
        if (!m_kqueue)
            throw std::runtime_error("kqueue is not initialized");

        wakeKEvent(1);
        m_thread->finalize();

        for (auto const &fd_acceptor: m_map_fd_acceptor)
            ::close(fd_acceptor.first);

        for (auto const &fd_uri: m_map_fd_uri)
            ::close(fd_uri.first);

    } catch (std::exception const &e) {
        throw std::runtime_error(
            std::string("connection manager finalization error: ") + e.what());
    }
}


void CConnectionManager::manage(
    URI::TSharedPtr
        const &uri,
    IProtocol::TSharedPtr
        const &protocol)
{
    try {
        auto fds = connect(uri);

        {
            LOCK_SCOPE();

            for (int id: fds) {
                if (id != -1)
                    m_map_fd_uri[id] = uri;
            }

            m_map_uri_fd[uri] = fds;
            m_map_uri_protocol[uri] = protocol;
        }

        wakeKEvent(0);
    } catch (std::exception const &e) {
        throw std::runtime_error(
            string("connection manage protocol error: ") + e.what());
    }
}


void CConnectionManager::manage(
    URI::TSharedPtr
        const &uri,
    IAcceptor::TSharedPtr
        const &acceptor)
{
    try {
        int domain      = -1;
        int type        = -1;
        int protocol    =  0;

        struct sockaddr_storage
            address {};
        socklen_t
            address_length = 0;

        if (uri->getProtocol() == URI::TProtocol::IPC) {
            domain  = AF_UNIX;
            type    = SOCK_STREAM;

            auto* address_unix = reinterpret_cast<sockaddr_un*>(&address);
            address_unix->sun_family = AF_UNIX;

            std::string path = uri->getPath();
#ifdef MACOS_PLATFORM
            snprintf(address_unix->sun_path, sizeof(address_unix->sun_path), "%s", path.c_str());
            address_length = sizeof(sockaddr_un);
#else
            if(!path.empty() && path[0] == '\0') {
                size_t n = path.size() - 1;
                address_unix->sun_path[0] = '\0';
                memcpy(address_unix->sun_path + 1, path.c_str() + 1, n);
                address_unix_length = static_cast<socklen_t>(offsetof(sockaddr_un, sun_path) + 1 + n);
            } else {
                snprintf(address_unix->sun_path, sizeof(address_unix->sun_path), "%s", path.c_str());
                address_unix_length = sizeof(sockaddr_un);
            }
#endif
        }

        else

        if (uri->getProtocol() == URI::TProtocol::UDP) {
            domain      = AF_INET;
            type        = SOCK_DGRAM;
            protocol    = IPPROTO_UDP;

            auto* address_in            = reinterpret_cast<sockaddr_in*>(&address);

            address_in->sin_family      = AF_INET;
            address_in->sin_port        = htons(uri->getPort());
            address_in->sin_addr.s_addr = htonl(INADDR_ANY);
            address_length              = sizeof(sockaddr_in);
        }

        else

        {
            domain      = AF_INET;
            type        = SOCK_STREAM;
            protocol    = IPPROTO_TCP;

            auto* address_in            = reinterpret_cast<sockaddr_in*>(&address);

            address_in->sin_family      = AF_INET;
            address_in->sin_port        = htons(uri->getPort() ?: static_cast<uint16_t>(uri->getProtocol()));
            address_in->sin_addr.s_addr = htonl(INADDR_ANY);
            address_length              = sizeof(sockaddr_in);
        }

        int fd = assertOK(::socket(domain, type, protocol), "socket");

        static int const YES = 1;
        assertOK(::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &YES, sizeof(YES)), "setsockopt REUSEADDR");
        assertOK(::setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &YES, sizeof(YES)), "setsockopt REUSEPORT");

        int flags = assertOK(
            ::fcntl(fd, F_GETFL, 0),
             "fcntl GETFL");
        assertOK(
            ::fcntl(fd, F_SETFL, flags | O_NONBLOCK),
             "fcntl SETFL O_NONBLOCK");

        assertOK(
            ::bind(fd, reinterpret_cast<sockaddr*>(&address), address_length),
             "bind");

        if (type == SOCK_STREAM)
            assertOK(
                ::listen(fd, SOMAXCONN),
                 "listen");

        struct kevent event;
        EV_SET(&event, fd, EVFILT_READ, EV_ADD |
            (type == SOCK_DGRAM ? EV_CLEAR : 0), 0, 0, reinterpret_cast<void *>(1));

        assertOK(
            ::kevent(m_kqueue, &event, 1, nullptr, 0, nullptr),
             "kevent");

        {
            LOCK_SCOPE();
            m_map_fd_acceptor[fd] = acceptor;
        }

        // wakeKEvent(0);
        LOGT << "listen: " << uri;
    } catch (std::exception const &e) {
        throw std::runtime_error(
            string("connection manage '" + convert<string>(uri) + "' acceptor error: ") + e.what());
    }
}


std::vector<int> CConnectionManager::connect(
    URI::TSharedPtr
        const &uri,
    bool
        const &is_writer)
{
    std::vector<int> fds;

    if (uri->getProtocol() == URI::TProtocol::PROCESS) {
        int stdin_pipe[2]  = {-1, -1};
        int stdout_pipe[2] = {-1, -1};

        // pipes
        assertOK(pipe(stdin_pipe),  "pipe stdin");
        assertOK(pipe(stdout_pipe), "pipe stdout");

        // non-blocking
        static auto set_nonblock = [] (int fd) {
            int flags = assertOK(::fcntl(fd, F_GETFL, 0), "fcntl GETFL");
            assertOK(::fcntl(fd, F_SETFL, flags | O_NONBLOCK), "fcntl SETFL O_NONBLOCK");
        };
        set_nonblock(stdin_pipe[1]);   // Parent -> Child STDIN
        set_nonblock(stdout_pipe[0]);  // Parent <- Child STDOUT

        // fork
        pid_t pid = assertOK(::fork(), "fork");

        if (pid == 0) {
            // ----- CHILD
            ::close(stdin_pipe[1]);
            ::close(stdout_pipe[0]);

            assertOK(::dup2(stdin_pipe[0],  STDIN_FILENO),  "dup2 stdin");
            assertOK(::dup2(stdout_pipe[1], STDOUT_FILENO), "dup2 stdout");

            ::close(stdin_pipe[0]);
            ::close(stdout_pipe[1]);

            ::execlp(
                uri->getPath().c_str(),
                uri->getPath().c_str(),
                uri->getArguments().empty() ? nullptr :
                uri->getArguments().c_str(),  nullptr);
            _exit(127);
            // -----
        }

        // ----- PARENT
        ::close(stdin_pipe[0]);
        ::close(stdout_pipe[1]);

        fds = { stdout_pipe[0], stdin_pipe[1], static_cast<int>(pid) };

        // // kqueue Registration
        // struct kevent event;
        // void* udata = uri.get();

        // EV_SET(&event, fds[THandleType::READER], EVFILT_READ, EV_ADD | EV_CLEAR, 0, 0, udata);
        // assertOK(::kevent(m_kqueue, &event, 1, nullptr, 0, nullptr), "kevent READER");

        // EV_SET(&event, fds[THandleType::WRITER], EVFILT_WRITE, EV_ADD | EV_CLEAR, 0, 0, udata);
        // assertOK(::kevent(m_kqueue, &event, 1, nullptr, 0, nullptr), "kevent WRITER");

        // EV_SET(&event, static_cast<intptr_t>(pid), EVFILT_PROC, EV_ADD, NOTE_EXIT, 0, udata);
        // assertOK(::kevent(m_kqueue, &event, 1, nullptr, 0, nullptr), "kevent PROC");

        return fds; // ----->
    }

    if (uri->getProtocol() == URI::TProtocol::FILE) {
        // struct kevent event;
        if (is_writer) {
            auto fd = assertOK(::open(uri->getPath().c_str(), O_WRONLY | O_CREAT), "open file WRONLY");
            assertOK(::flock(fd, LOCK_EX | LOCK_NB), "flock");
            fds = { fd };
            // EV_SET(&event, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, nullptr);
            // flock(fd, LOCK_UN);
        } else {
            auto fd = assertOK(::open(uri->getPath().c_str(), O_RDONLY), "open file RDONLY");
            fds = { fd };
            // EV_SET(&event, fd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, nullptr);
        }
        return fds; // ----->
    }

    int domain      = -1;
    int type        = -1;
    int protocol    = 0;

    struct sockaddr_storage address {};
    socklen_t address_length = 0;

    if (uri->getProtocol() == URI::TProtocol::UDP) {
        domain      = AF_INET;
        type        = SOCK_DGRAM;
        protocol    = IPPROTO_UDP;

        auto* address_in = reinterpret_cast<sockaddr_in*>(&address);

        address_in->sin_family  = AF_INET;
        address_in->sin_port    = htons(uri->getPort());

        if (auto ipv4 = uri->getIPv4()) {
            address_in->sin_addr.s_addr =
                (ipv4->at(0) << 24) | (ipv4->at(1) << 16) |
                (ipv4->at(2) << 8)  |  ipv4->at(3);
        } else {
            address_in->sin_addr.s_addr = htonl(INADDR_ANY);
        }

        address_length = sizeof(sockaddr_in);
    }

    else

    if (uri->getProtocol() == URI::TProtocol::IPC) {
        domain  = AF_UNIX;
        type    = SOCK_STREAM;

        auto* address_unix          = reinterpret_cast<sockaddr_un*>(&address);
        address_unix->sun_family    = AF_UNIX;

        std::string path = uri->getPath();

#ifdef MACOS_PLATFORM
        snprintf(address_unix->sun_path, sizeof(address_unix->sun_path), "%s", path.c_str());
        address_length = sizeof(sockaddr_un);
#else
        if (!path.empty() && path[0] == '\0') {
            size_t n = path.size() - 1;
            address_unix->sun_path[0] = '\0';

            memcpy(address_unix->sun_path + 1, path.c_str() + 1, n);

            address_length = static_cast<socklen_t>(offsetof(sockaddr_un, sun_path) + 1 + n);
        } else {
            snprintf(address_unix->sun_path, sizeof(address_unix->sun_path), "%s", path.c_str());

            address_length = sizeof(sockaddr_un);
        }
#endif
    }

    else

    {
        // TCP / HTTP / HTTPS / etc.
        domain      = AF_INET;
        type        = SOCK_STREAM;
        protocol    = IPPROTO_TCP;

        auto* address_in        = reinterpret_cast<sockaddr_in*>(&address);
        address_in->sin_family  = AF_INET;
        address_in->sin_port    = htons(uri->getPort() ? 0 : static_cast<uint16_t>(uri->getProtocol()));

        if (auto ipv4 = uri->getIPv4()) {
            address_in->sin_addr.s_addr =
                (ipv4->at(0) << 24) | (ipv4->at(1) << 16) |
                (ipv4->at(2) << 8)  |  ipv4->at(3);
        } else {
            address_in->sin_addr.s_addr = htonl(INADDR_ANY);
        }
        address_length = sizeof(sockaddr_in);
    }

    int fd = assertOK(::socket(domain, type, protocol), "socket");

    static int const YES = 1;
    assertOK(::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &YES, sizeof(YES)), "setsockopt REUSEADDR");
    assertOK(::setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &YES, sizeof(YES)), "setsockopt REUSEPORT");

    int flags = assertOK(::fcntl(fd, F_GETFL, 0), "fcntl GETFL");
    assertOK(::fcntl(fd, F_SETFL, flags | O_NONBLOCK), "fcntl SETFL O_NONBLOCK");

    int result = ::connect(fd, reinterpret_cast<sockaddr*>(&address), address_length);
    if (result < 0 && errno != EINPROGRESS)
        assertOK(result, "connect");

    // struct kevent event;

    // EV_SET(&event, fd, EVFILT_READ, EV_ADD | (type == SOCK_DGRAM ? EV_CLEAR : 0), 0, 0, nullptr);
    // assertOK(::kevent(m_kqueue, &event, 1, nullptr, 0, nullptr), "kevent READ");

    // if (result < 0 && errno == EINPROGRESS) {
    //     EV_SET(&event, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, nullptr);
    //     assertOK(::kevent(m_kqueue, &event, 1, nullptr, 0, nullptr), "kevent WRITE (pending)");
    // }

    return fds;
}


void CConnectionManager::wakeKEvent(int const &code) {
    struct kevent trigger;

    EV_SET(&trigger, DEFAULT_IDENT_WAKEUP, EVFILT_USER, 0, NOTE_TRIGGER, static_cast<intptr_t>(code), nullptr);

    kevent(m_kqueue, &trigger, 1, nullptr, 0, nullptr);
    int result = kevent(m_kqueue, &trigger, 1, nullptr, 0, nullptr);
    if (result < 0 && errno != EBADF)
        LOGW << "wakeKEvent failed: " << string(strerror(errno));
}


URI::TSharedPtr CConnectionManager::getPeerURI(sockaddr_storage const &address) {
    char ip[INET6_ADDRSTRLEN];
    uint16_t port;

    if (address.ss_family == AF_INET) {
        const auto* in = reinterpret_cast<const sockaddr_in*>(&address);
        inet_ntop(AF_INET, &in->sin_addr, ip, sizeof(ip));
        port = ntohs(in->sin_port);
        return URI::create("tcp://" + string(ip) + ":" + convert<string>(port));
    }

    else

    if (address.ss_family == AF_INET6) {
        const auto* in6 = reinterpret_cast<const sockaddr_in6*>(&address);
        inet_ntop(AF_INET6, &in6->sin6_addr, ip, sizeof(ip));
        port = ntohs(in6->sin6_port);
        return URI::create("tcp://[" + string(ip) + "]:" + convert<string>(port));
    }

    throw std::runtime_error("get peer uri error: unknown sockaddr type");
}


URI::TSharedPtr CConnectionManager::getPeerURI(int const &fd) {
    struct sockaddr_storage address{};
    socklen_t address_length = sizeof(address);

    assertOK(::getpeername(fd, reinterpret_cast<sockaddr*>(&address), &address_length), "getpeername");

    if (address.ss_family == AF_INET) {
        auto* address_in = reinterpret_cast<const sockaddr_in*>(&address);
        char buffer[INET_ADDRSTRLEN];
        const char* ip = inet_ntop(AF_INET, &address_in->sin_addr, buffer, sizeof(buffer));
        return URI::create(
            "tcp://" + std::string(ip ?: "0.0.0.0") +
            ":" + convert<std::string>(ntohs(address_in->sin_port)));
    }

    else

    if (address.ss_family == AF_INET6) {
        auto *address_in6 = reinterpret_cast<const sockaddr_in6*>(&address);
        char buffer[INET6_ADDRSTRLEN];
        const char* ip = inet_ntop(AF_INET6, &address_in6->sin6_addr, buffer, sizeof(buffer));
        return URI::create(
            "tcp://[" + std::string(ip ?: "::1") +
            "]:" + convert<std::string>(ntohs(address_in6->sin6_port)));
    }

    else

    if (address.ss_family == AF_UNIX) {
        auto *address_unix = reinterpret_cast<const sockaddr_un*>(&address);

        if (address_unix->sun_path[0] == '\0') {
            size_t length = strnlen(address_unix->sun_path + 1, sizeof(address_unix->sun_path) - 1);
            return URI::create(
                "ipc://abstract/" + std::string(address_unix->sun_path + 1, length) +
                "?fd=" + convert<std::string>(fd));
        }

        std::string path = address_unix->sun_path;

#ifdef MACOS_PLATFORM
        // PID (macOS >= 10.8)
        pid_t pid = -1;
        socklen_t length = sizeof(pid);
        if (::getsockopt(fd, SOL_LOCAL, LOCAL_PEERPID, &pid, &length) == 0 && pid > 0)
            return URI::create(
                "ipc://" + path + "?pid=" + convert<std::string>(pid) +
                "?fd=" + convert<std::string>(fd));

        // UID/GID (POSIX)
        uid_t uid = -1; gid_t gid = -1;
        if (::getpeereid(fd, &uid, &gid) == 0)
            return URI::create(
                "ipc://" + path + "?uid=" + convert<std::string>(uid) +
                "?fd=" + convert<std::string>(fd));
#elif LINUX_PLATFORM
        struct ucred cred{};
        socklen_t length = sizeof(cred);
        if (::getsockopt(fd, SOL_SOCKET, SO_PEERCRED, &cred, &length) == 0)
            return URI::create(
                "ipc://" + path + "?pid=" + convert<std::string>(cred.pid) +
                "?fd=" + convert<std::string>(fd));
#endif
        return URI::create(
            "ipc://" + path + "?fd=" + convert<std::string>(fd));
    }

    throw std::runtime_error("get peer error: unknown type of fd " + convert<std::string>(fd));
}


std::list<CConnectionManager::THandle::TSharedPtr>
CConnectionManager::getHandles(std::vector<struct kevent> const &events) {
    std::list<CConnectionManager::THandle::TSharedPtr>
        result;

    LOCK_SCOPE();

    for (auto const &event: events) {
        if (event.filter == EVFILT_PROC)
            continue; // <---

        auto const &fd = event.ident;
        auto handle_info = THandle::create();

        auto fd_uri  = m_map_fd_uri.find(fd);
        if ( fd_uri == m_map_fd_uri.end()) {
            LOGW << "connection manager: uri not found, not registered fd: " << fd;
            ::close(fd);
            continue; // <---
        } else {
            handle_info->uri = fd_uri->second;
        }

        auto uri_protocol  = m_map_uri_protocol.find(handle_info->uri);
        if ( uri_protocol == m_map_uri_protocol.end()) {
            LOGW
                << "connection manager: protocol not found by uri '" << handle_info->uri
                << "' not registered fd: " << fd;
            ::close(fd);
            continue; // <---
        } else {
            handle_info->protocol = uri_protocol->second;
        }

        auto uri_context  = m_map_uri_context.find(handle_info->uri);
        if ( uri_context == m_map_uri_context.end()) {
            handle_info->context = m_map_uri_context[handle_info->uri] = CContext::create();
        } else {
            handle_info->context = uri_context->second;
        }

        result.push_back(handle_info);
    }

    return result; // ----->
}


CConnectionManager::CKEventRunnable::CKEventRunnable(CConnectionManager * const manager)
:
    m_manager(manager)
{}


void CConnectionManager::CKEventRunnable::run(std::atomic<bool> &is_running) {
    struct timespec
        timeout {};

    timeout.tv_sec  =
        duration_cast<seconds>(DEFAULT_KEVENT_TIMEOUT).count();
    timeout.tv_nsec =
        duration_cast<nanoseconds>(
            DEFAULT_KEVENT_TIMEOUT % seconds(1)).count();

    auto kqueue = m_manager->m_kqueue.load();
    std::vector<struct kevent>
        triggered_events(DEFAULT_EVENTS_LIMIT);

    while (is_running) {
        auto count = assertOK(
            kevent(
                kqueue, nullptr, 0, triggered_events.data(),
                static_cast<int>(triggered_events.size()), &timeout),
           "kevent wait error");

        if (count == 0)
            continue;

        auto handles = m_manager->getHandles(triggered_events);

        for (int i = 0; i < count; ++i) {
            auto const &event = triggered_events[i];

            LOGT << "event, ident: " << event.ident << ", filter: " << event.filter;

            if (event.filter == EVFILT_USER && event.ident == DEFAULT_IDENT_WAKEUP) {
                int code = static_cast<int>(event.data);
                if (code == 1) {
                    is_running = false;
                    break;
                }
                continue;
            }

            // acceptor event
            if (event.udata == reinterpret_cast<void *>(1)) {
                std::vector<TTCPPeer::TSharedPtr> peers;

                while (true) {
                    sockaddr_storage
                        address {};
                    socklen_t
                        address_length = sizeof(address);

                    int peer_fd = ::accept(
                        event.ident,
                        reinterpret_cast<struct sockaddr *>(&address),
                        &address_length);

                    if (peer_fd == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break;
                        if (errno == EINTR)
                            continue;
                        LOGW << "peer accept error: " << string(std::strerror(errno));
                        break;
                    }

                    auto peer = TTCPPeer::create();
                    peer->fd  = peer_fd;
                    peer->uri = getPeerURI(address);

                    struct kevent peer_event;
                    EV_SET(&peer_event, peer_fd, EVFILT_READ, EV_ADD | EV_CLEAR,
                        0, 0, reinterpret_cast<void *>(static_cast<uintptr_t>(peer_fd)));

                    if (::kevent(kqueue, &peer_event, 1, nullptr, 0, nullptr) == -1) {
                        LOGE << "kevent registration peer fd " << peer_fd << " error: " << string(std::strerror(errno));
                        ::close(peer_fd);
                        continue;
                    }

                    peers.push_back(peer);
                }

                // m_manager->handleEvent(event.ident, std::move(peers));
                continue;
            }

            Buffer::TSharedPtr write_buffer;
            // peer / client event
            if (event.filter == EVFILT_READ) {
                // drain loop until end, EV_CLEAR
                while (true) {
                    auto buffer = Buffer::create(DEFAULT_BUFFER_SIZE);
                    ssize_t n = ::read(event.ident, buffer->data(), buffer->capacity());

                    if (n > 0) {
                        buffer->resize(n);
                        // write_buffer = m_manager->handleEvent(event.ident, TEvent::TOperation::READ, buffer, 0);
                    }

                    else

                    if (n == 0) {
                        // write_buffer = m_manager->handleEvent(event.ident, TEvent::TOperation::CLOSE, nullptr, 0);
                        break;
                    }

                    else

                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        // write_buffer = m_manager->handleEvent(event.ident, TEvent::TOperation::READ_EOF, nullptr, 0);
                        break;
                    } else {
                        buffer = Buffer::create(std::strerror(errno));
                        // m_manager->handleEvent(event.ident, TEvent::TOperation::ERROR_, buffer, 0);
                        break;
                    }
                }
            }

            // else

            if (event.filter == EVFILT_WRITE || write_buffer) {
                if (event.flags & EV_EOF) {
                    // m_manager->handleEvent(event.ident, TEvent::TOperation::ERROR_, nullptr, 0);
                } else {
                    Buffer::TSharedPtr buffer;

                    if (write_buffer) {
                        buffer = write_buffer;
                    } else {
                        // buffer = m_manager->handleEvent(event.ident, TEvent::TOperation::WRITE, nullptr, 0);

                        ssize_t n = 0;

                        if  (buffer)
                            n = ::write(event.ident, buffer->data(), buffer->size());

                        LOGT << "wrote, n: " << n;
                    }
                    // m_manager->handleEvent(event.ident, TEvent::TOperation::WRITE, nullptr, n);

                }
            }

            else

            if (event.filter == EVFILT_PROC && (event.fflags & NOTE_EXIT)) {
                // m_manager->handleEvent(event.ident, TEvent::TOperation::CLOSE, nullptr, 0);
                // clean zombie
                ::waitpid(static_cast<pid_t>(event.ident), nullptr, 0);
            }
        }
    }
}


void CConnectionManager::CKEventRunnable::initialize() {}


void CConnectionManager::CKEventRunnable::finalize() {}


} // iridium::io::implementation::platform


#endif // MACOS_PLATFORM


// IContextActions::TSharedPtr CConnectionManager::handleEvent(
//     int const &fd,
//     TEvent::TOperation
//         const &operation,
//     Buffer::TSharedPtr
//         const &read_buffer,
//     size_t
//         const &written_bytes_count)
// {
//     LOGT << "handleEvent, fd: " << fd
//          << ", operation: " << operation
//          << ", buffer: " << read_buffer;

//     try {
//         auto handle_info = getHandles(fd);

//         URI::TSharedPtr
//             uri = handle_info->uri;
//         IProtocol::TSharedPtr
//             protocol = handle_info->protocol;
//         IContextActions::TSharedPtr
//             context = handle_info->context;

//         auto event = TEvent::create();
//         event->operation = operation;
//         event->uri = uri;
//         event->context = context;

//         if (operation == TEvent::TOperation::READ && read_buffer) {
//             context->addBuffer(uri, IContext::TStreamType::READER, read_buffer);
//             context->addBuffer(uri, IContext::TStreamType::WRITER, read_buffer);
//         }
//         context->setPosition(uri, IContext::TStreamType::WRITER, written_bytes_count);

//         // update context pipes by protocol
//         auto result = protocol->control(event);

//         auto buffers = context->getBuffers(uri, IContext::TStreamType::WRITER);
//         Buffer::TSharedPtr write_buffer = buffers.empty() ? nullptr : buffers.back();

//         if (!result) {
//             LOGT << "destroy context, result: " << result << ", event: " << event;
//             event->context->delPipe("");
//         }

//         auto actions = context->getActions();
//         LOGT << "actions: " << actions;

//         if (actions.empty())
//             return write_buffer; // ----->

//         std::unordered_map<URI::TSharedPtr, int>
//             map_uri_new_fd;

//         for (auto const &action : actions) {
//             if (action.action_type == IContextActions::TActionType::OPEN) {
//                 try {
//                     bool is_writer = (action.stream_type == IContextActions::TStreamType::WRITER);
//                     auto new_fds = connect(action.uri, is_writer);

//                     if (!new_fds.empty()) {
//                         int primary_fd = new_fds[0];
//                         map_uri_new_fd[action.uri] = primary_fd;
//                         m_map_uri_fd[action.uri] = new_fds;
//                     }
//                 } catch (std::exception const &e) {
//                     LOGE << "OPEN failed for " << action.uri << ": " << e.what();
//                 }
//             }

//             else

//             if (action.action_type == IContextActions::TActionType::CLOSE) {
//                 auto uri_fd_it = m_map_uri_fd.find(action.uri);
//                 if (uri_fd_it != m_map_uri_fd.end()) {
//                     for (int old_fd : uri_fd_it->second) {
//                         ::close(old_fd);
//                         m_map_fd_uri.erase(old_fd);
//                     }
//                     m_map_uri_fd.erase(uri_fd_it);
//                 }
//             }
//         }

//             // {
//             //     for (int new_fd : new_fds) {
//             //         m_map_fd_uri[new_fd] = action.uri;
//             //     }
//             // }

//         // === 8. Проход 2: Обрабатываем SUBSCRIBE / UNSUBSCRIBE (строят kevent batch) ===
//         std::vector<struct kevent> batch;
//         batch.reserve(actions.size());

//         {
//             LOCK_SCOPE();
//             for (auto const &action : actions) {
//                 if (action.action_type != IContextActions::TActionType::SUBSCRIBE &&
//                     action.action_type != IContextActions::TActionType::UNSUBSCRIBE)
//                 {
//                     continue;
//                 }

//                 auto uri_fd_it = m_map_uri_fd.find(action.uri);

//                 // КРИТИЧЕСКАЯ ЛОГИКА: Если у URI больше 1 FD — это процесс.
//                 // Процесс уже полностью подписан (READ + WRITE) при создании через connect().
//                 // Менять подписку с reader на writer и наоборот для него нет смысла.
//                 if (uri_fd_it != m_map_uri_fd.end() && uri_fd_it->second.size() > 1) {
//                     LOGT << "skip subscribe/unsubscribe for process uri (multiple FDs): " << action.uri << " fds: " << uri_fd_it->second;
//                     continue;
//                 }

//                 // Ищем целевой FD: сначала в новых (от OPEN в этом же батче), потом в мапе
//                 int target_fd = -1;
//                 auto new_fd_it = map_uri_new_fd.find(action.uri);
//                 if (new_fd_it != map_uri_new_fd.end()) {
//                     target_fd = new_fd_it->second;
//                 } else if (uri_fd_it != m_map_uri_fd.end() && !uri_fd_it->second.empty()) {
//                     target_fd = uri_fd_it->second[0];
//                 }

//                 if (target_fd < 0) {
//                     LOGW << "FD not found for action on " << action.uri;
//                     continue;
//                 }

//                 uint16_t filter =
//                     (action.stream_type == IContextActions::TStreamType::READER)
//                         ? EVFILT_READ
//                         : EVFILT_WRITE;

//                 auto &ev = batch.emplace_back();
//                 if (action.action_type == IContextActions::TActionType::SUBSCRIBE) {
//                     EV_SET(&ev, target_fd, filter, EV_ADD | EV_CLEAR, 0, 0, uri.get());
//                     m_map_fd_uri[target_fd] = action.uri;
//                 } else { // UNSUBSCRIBE
//                     EV_SET(&ev, target_fd, filter, EV_DELETE, 0, 0, nullptr);
//                     m_map_fd_uri.erase(target_fd);
//                 }
//             }
//         }

//         // === 9. Атомарный вызов kevent batch ===
//         if (!batch.empty()) {
//             assertOK(
//                 kevent(m_kqueue, batch.data(), static_cast<int>(batch.size()), nullptr, 0, nullptr),
//                "kevent batch"
//             );
//         }

//         LOGT << "handle event, write_buffer: " << write_buffer;
//         return write_buffer;

//     } catch (std::exception const &e) {
//         throw std::runtime_error(
//             "handle event " + convert<std::string>(operation) +
//             " fd " + convert<std::string>(fd) + " error: " + e.what());
//     }
// }


// IContextActions::TSharedPtr CConnectionManager::handleEvent(
//     int const &acceptor_fd,
//     std::vector<TTCPPeer::TSharedPtr>
//         const &peers)
// {
//     IAcceptor::TSharedPtr
//         acceptor;

//     LOGT << "handleEvent, acceptor fd: " << acceptor_fd;

//     try {
//         {
//             LOCK_SCOPE();

//             auto fd_acceptor  = m_map_fd_acceptor.find(acceptor_fd);
//             if  (fd_acceptor == m_map_fd_acceptor.end()) {
//                 LOGW << "connection manager, not registered acceptor fd: " << acceptor_fd;
//                 ::close(acceptor_fd);
//                 return;
//             } else {
//                 acceptor = fd_acceptor->second;
//             }
//         }

//         for(auto const &peer: peers) {
//             auto peer_protocol = acceptor->accept(peer->uri);
//             if (!peer_protocol)
//                 continue;

//             auto context = CContext::create();
//             {
//                 auto event = TEvent::create();
//                 event->uri = peer->uri;
//                 event->operation = TEvent::TOperation::OPEN;
//                 event->context = context;

//                 peer_protocol->control(event);
//                 auto actions = std::dynamic_pointer_cast<IContextActions>(event->context)->getActions();
//                 // todo: connect uris if uri != peer uri
//             }

//             std::vector<int> fds;
//             fds = { peer->fd };
//             {
//                 LOCK_SCOPE();

//                 m_map_fd_uri[peer->fd]  = peer->uri;
//                 m_map_uri_fd[peer->uri] = fds;
//                 m_map_uri_protocol[peer->uri] = peer_protocol;
//                 m_map_uri_context [peer->uri] = context;
//             }
//         }
//     } catch (std::exception const &e) {
//         throw std::runtime_error(
//             "handle acceptor event fd " +
//             convert<string>(acceptor_fd) + " error: " + e.what());
//     }
// }
