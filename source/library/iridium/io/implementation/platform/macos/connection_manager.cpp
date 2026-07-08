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

        LOCK_SCOPE();
        for (auto const &fd_handle: m_map_fd_handle)
            ::close(fd_handle.first);

        m_map_fd_handle.clear();
        m_map_uri_fd.clear();
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
        std::vector<int> fds;

        THandle::TSharedPtr handle;
        {
            LOCK_SCOPE();
            auto uri_fd  = m_map_uri_fd.find(uri);
            // peer
            // todo: fix multi fd
            if ( uri_fd != m_map_uri_fd.end()) {
                fds.push_back(uri_fd->second);

                auto fd_handle  = m_map_fd_handle.find(uri_fd->second);
                if ( fd_handle != m_map_fd_handle.end())
                    handle = fd_handle->second;
            }
        }

        LOGT << "fds: " << fds;
        if (fds.empty())
            fds = connect(uri, IContext::TStreamType::WRITER);

        if(!handle) {
            handle              = THandle::create();
            handle->context     = CContext::create();
            handle->protocol    = protocol;
        }

        handle->uri         = uri;
        handle->fd          = fds.empty() ? -1 : fds.front();

        // todo: pid, stdin, stdout
        {
            LOCK_SCOPE();

            for (int const &fd: fds) {
                if (fd < 0)
                    continue; // <---
                m_map_fd_handle[fd] = handle;
                m_map_uri_fd[uri] = fd;
            }
        }

        wakeKEvent(0);
    } catch (std::exception const &e) {
        throw std::runtime_error(
            string("connection manage " + convert<string>(uri) + " protocol error: ") + e.what());
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

            auto *address_unix = reinterpret_cast<sockaddr_un*>(&address);
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

            auto *address_in            = reinterpret_cast<sockaddr_in*>(&address);

            address_in->sin_family      = AF_INET;
            address_in->sin_port        = htons(uri->getPort() ?: static_cast<uint16_t>(uri->getProtocol()));
            address_in->sin_addr.s_addr = htonl(INADDR_ANY);
            address_length              = sizeof(sockaddr_in);
        }

        int fd = assertOK(::socket(domain, type, protocol), "socket");

        static int const YES = 1;
        assertOK(
            ::setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &YES, sizeof(YES)),
             "setsockopt REUSEADDR");
        assertOK(
            ::setsockopt(fd, SOL_SOCKET, SO_REUSEPORT, &YES, sizeof(YES)),
             "setsockopt REUSEPORT");

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

        auto handle = THandle::create();
        handle->acceptor = acceptor;
        handle->uri = uri;
        handle->fd = fd;

        {
            LOCK_SCOPE();
            LOGT << "add acceptor, fd: " << fd << ", uri: " << uri;
            m_map_fd_handle[fd] = handle;
            m_map_uri_fd[handle->uri] = fd;
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
    IContextActions::TStreamType
        const &stream_type)
{
    std::vector<int> fds;

    if (uri->getProtocol() == URI::TProtocol::PROCESS) {
        int stdin_pipe[2]  = { -1, -1 };
        int stdout_pipe[2] = { -1, -1 };

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
        if (stream_type == IContextActions::TStreamType::WRITER) {
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
    int protocol    =  0;

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

        auto *address_in        = reinterpret_cast<sockaddr_in *>(&address);
        address_in->sin_family  = AF_INET;
        address_in->sin_port    = htons(uri->getPort() ? 0 : static_cast<uint16_t>(uri->getProtocol()));

        if (auto ipv4 = uri->getIPv4()) {
            address_in->sin_addr.s_addr =
                (ipv4->at(0) << 24) |
                (ipv4->at(1) << 16) |
                (ipv4->at(2) <<  8) |
                 ipv4->at(3);
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
        auto const *in = reinterpret_cast<const sockaddr_in*>(&address);
        inet_ntop(AF_INET, &in->sin_addr, ip, sizeof(ip));
        port = ntohs(in->sin_port);
        return URI::create("tcp://" + string(ip) + ":" + convert<string>(port));
    }

    else

    if (address.ss_family == AF_INET6) {
        auto const *in6 = reinterpret_cast<const sockaddr_in6*>(&address);
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
        auto *address_in = reinterpret_cast<const sockaddr_in *>(&address);
        char buffer[INET_ADDRSTRLEN];
        const char *ip = inet_ntop(AF_INET, &address_in->sin_addr, buffer, sizeof(buffer));

        return URI::create(
            "tcp://" + std::string(ip ?: "0.0.0.0") +
            ":" + convert<std::string>(ntohs(address_in->sin_port)));
    }

    else

    if (address.ss_family == AF_INET6) {
        auto *address_in6 = reinterpret_cast<const sockaddr_in6*>(&address);
        char buffer[INET6_ADDRSTRLEN];
        const char *ip = inet_ntop(AF_INET6, &address_in6->sin6_addr, buffer, sizeof(buffer));
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

    throw std::runtime_error(
        "get peer error: unknown type of fd " + convert<std::string>(fd));
}


std::list<CConnectionManager::THandle::TSharedPtr>
CConnectionManager::getHandles(
    std::vector<struct kevent>
        const &events,
    size_t
        const &count)
{
    std::list<CConnectionManager::THandle::TSharedPtr>
        result;

    LOCK_SCOPE();

    for (size_t i = 0; i < count; i++) {
        auto const &event = events[i];
        if (event.filter == EVFILT_PROC || event.ident <= 0 || event.ident == DEFAULT_IDENT_WAKEUP)
            continue; // <---

        auto fd_handle  = m_map_fd_handle.find(static_cast<int>(event.ident));
        if ( fd_handle == m_map_fd_handle.end()) {
            LOGW << "connection manager: get handle error: not registered fd: " << event.ident;
            ::close(static_cast<int>(event.ident));
            continue; // <---
        } else {
            if (fd_handle->second->protocol) {
                if (event.filter == EVFILT_READ)
                    fd_handle->second->stream_type = IContextActions::TStreamType::READER;
                if (event.filter == EVFILT_WRITE)
                    fd_handle->second->stream_type = IContextActions::TStreamType::WRITER;
            }
            result.push_back(fd_handle->second);
        }
    }

    return result; // ----->

    // if (event.filter == EVFILT_PROC && (event.fflags & NOTE_EXIT)) {
    //     // m_manager->handleEvent(event.ident, TEvent::TOperation::CLOSE, nullptr, 0);
    //     // clean zombie
    //     ::waitpid(static_cast<pid_t>(event.ident), nullptr, 0);
    // }
}


void CConnectionManager::updateHandles(std::list<THandle::TSharedPtr> const &handles) {
    LOCK_SCOPE();
    for (auto const &handle: handles) {
        m_map_fd_handle[handle->fd] = handle;
        m_map_uri_fd[handle->uri] = handle->fd;
    }
}


void CConnectionManager::updateHandles(
    std::map<IProtocol::TSharedPtr, std::list<IContextActions::TAction> >
        const &map_protocol_actions)
{
    std::vector<struct kevent> batch;

    for(auto const &protocol_actions: map_protocol_actions) {
        auto const &protocol = protocol_actions.first;
        for (auto const &action: protocol_actions.second) {
            if (action.action_type == IContextActions::TActionType::OPEN) {
                manage(action.uri, protocol);
                continue; // <---
            }

            // todo: optimize
            int fd = -1;
            {
                LOCK_SCOPE();

                auto uri_fd  = m_map_uri_fd.find(action.uri);
                if ( uri_fd == m_map_uri_fd.end()) {
                    throw std::runtime_error(
                        "update handle error: fd not found by " +
                        convert<string>(action.uri)); // ----->
                } else {
                    fd = uri_fd->second;
                }

                if (action.action_type == IContextActions::TActionType::CLOSE) {
                    ::close(fd);
                    m_map_fd_handle.erase(fd);
                    m_map_uri_fd.erase(action.uri);
                    continue; // <---
                }
            }

            short filter = 0;
            if (action.stream_type == IContextActions::TStreamType::READER)
                filter = EVFILT_READ;
            if (action.stream_type == IContextActions::TStreamType::WRITER)
                filter = EVFILT_WRITE;

            short flags = 0;
            if (action.action_type == IContextActions::TActionType::SUBSCRIBE)
                flags = EV_ADD | EV_CLEAR;
            if (action.action_type == IContextActions::TActionType::UNSUBSCRIBE)
                flags = EV_DELETE;

            struct kevent event;
            EV_SET(&event, fd, filter, flags, 0, 0, nullptr);
            batch.push_back(event);

            LOGT << "EV_SET: " << fd << " " << filter << " " << flags;
        }
    }

    if (batch.empty())
        return; // ----->

    assertOK(
        ::kevent(m_kqueue, batch.data(), static_cast<int>(batch.size()), nullptr, 0, nullptr),
         "kevent actions batch");
}


void CConnectionManager::releaseHandle(THandle::TSharedPtr const &handle) {
    LOCK_SCOPE();
    for (auto fd_handle = m_map_fd_handle.begin(); fd_handle != m_map_fd_handle.end(); ) {
        if (fd_handle->second == handle) {
            fd_handle = m_map_fd_handle.erase(fd_handle);
            ::close(fd_handle->first);
        } else {
            fd_handle++;
        }
    }

    m_map_uri_fd.erase(handle->uri);
}


CConnectionManager::CKEventRunnable::CKEventRunnable(
    CConnectionManager
        * const manager)
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
           "kevent waiting error");

        if (count == 0)
            continue;

        std::list<CConnectionManager::THandle::TSharedPtr>
            peers;

        std::map<IProtocol::TSharedPtr, std::list<IContextActions::TAction> >
            map_protocol_actions;

        LOGT << "NEXT";
        for (auto const &handle: m_manager->getHandles(triggered_events, count)) {
            LOGT << "event handle: " << handle->uri << " " << handle->stream_type << ", fd: " << handle->fd;
            if (handle->acceptor) {
                while (true) {
                    sockaddr_storage
                        address {};
                    socklen_t
                        address_length = sizeof(address);

                    int peer_fd = ::accept(
                        handle->fd,
                        reinterpret_cast<struct sockaddr *>(&address),
                        &address_length);

                    if (peer_fd == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break; // --->
                        if (errno == EINTR)
                            continue; // <---
                        LOGW << "peer accept error: " << string(std::strerror(errno));
                        break; // --->
                    }

                    try {
                        auto peer_handle = CConnectionManager::THandle::create();
                        peer_handle->fd         = peer_fd;
                        peer_handle->uri        = getPeerURI(address);
                        peer_handle->protocol   = handle->acceptor->accept(peer_handle->uri);
                        peer_handle->context    = CContext::create();

                        auto peer_event         = TEvent::create();
                        peer_event->context     = peer_handle->context;
                        peer_event->uri         = peer_handle->uri;
                        peer_event->operation   = TEvent::TOperation::OPEN;

                        // handle peer
                        bool result = false;
                        try {
                            result = peer_handle->protocol->control(peer_event);
                        } catch (std::exception const &e) {
                            LOGE << "peer " << peer_handle->uri << " opening error: " << e.what();
                        } catch (...) {
                            LOGE << "peer " << peer_handle->uri << " opening unknown error";
                        }

                        if (result) {
                            // add peer fd to map
                            auto &target_list = map_protocol_actions[peer_handle->protocol];
                            target_list.splice(target_list.end(), peer_handle->context->getActions());
                        } else {
                            // close peer after accepting
                            ::close(peer_handle->fd);
                            continue; // <---
                        }

                        // struct kevent event;
                        // EV_SET(&event, peer_fd, EVFILT_READ, EV_ADD | EV_CLEAR,
                        //     0, 0, reinterpret_cast<void *>(static_cast<uintptr_t>(peer_fd)));

                        // if (::kevent(kqueue, &event, 1, nullptr, 0, nullptr) == -1) {
                        //     LOGE << "kevent registration peer fd " << peer_fd << " error: " << string(std::strerror(errno));
                        //     ::close(peer_fd);
                        //     continue; // <---
                        // }

                        peers.push_back(peer_handle);

                    } catch (std::exception const &e) {
                        LOGE << "accepting on " << handle->uri << " error: " << e.what();
                        continue; // <---
                    } catch (...) {
                        LOGE << "accepting on " << handle->uri << " unknown error";
                        continue; // <---
                    }
                }
            }

            if (handle->protocol) {
                auto event      = TEvent::create();
                event->context  = handle->context;
                event->uri      = handle->uri;

                if (handle->stream_type == IContextActions::TStreamType::READER) {
                    auto buffer = Buffer::create(DEFAULT_BUFFER_SIZE);
                    auto n      = ::read(handle->fd, buffer->data(), buffer->capacity());

                    LOGT << "read, n: " << n;

                    if (n > 0) {
                        buffer->resize(n);
                        // handle->context->addBuffer(handle->uri, handle->stream_type, buffer);

                        handle->context->addBuffer(handle->uri, IContextActions::TStreamType::READER, buffer);
                        handle->context->addBuffer(handle->uri, IContextActions::TStreamType::WRITER, buffer);

                        event->operation = TEvent::TOperation::READ;
                    }

                    else

                    if (n == 0) {
                        event->operation = TEvent::TOperation::CLOSE;
                    }

                    else

                    {
                        handle->context->addBuffer(handle->uri, handle->stream_type, Buffer::create(std::strerror(errno)));
                        event->operation = TEvent::TOperation::ERROR_;
                    }
                }

                if (handle->stream_type == IContextActions::TStreamType::WRITER) {
                    auto buffers= handle->context->getBuffers(handle->uri, IContextActions::TStreamType::WRITER);
                    auto buffer = assertExists(buffers.front(), "buffer to wrtie is empty");
                    auto n      = ::write(handle->fd, buffer->data(), buffer->size());

                    LOGT << "wrote, n: " << n;

                    if (n > 0) {
                        handle->context->setPosition(handle->uri, IContextActions::TStreamType::WRITER, n);
                        event->operation = TEvent::TOperation::WRITE;
                    }

                    else

                    if (n == 0) {
                        event->operation = TEvent::TOperation::CLOSE;
                    }

                    else

                    {
                        handle->context->addBuffer(
                            handle->uri,
                            handle->stream_type,
                            Buffer::create(std::strerror(errno)));
                        event->operation = TEvent::TOperation::ERROR_;
                    }
                }

                bool result = false;
                try {
                    result = handle->protocol->control(event);
                } catch (std::exception const &e) {
                    LOGE << e.what();
                } catch (...) {
                    LOGE << "unknown error";
                }

                if (result) {
                    auto &target_list = map_protocol_actions[handle->protocol];
                    target_list.splice(target_list.end(), handle->context->getActions());
                } else {
                    m_manager->releaseHandle(handle);
                }
            }

            // update maps
            m_manager->updateHandles(peers);
            // update kqueue
            m_manager->updateHandles(map_protocol_actions);
        }
    }
}


void CConnectionManager::CKEventRunnable::initialize() {}


void CConnectionManager::CKEventRunnable::finalize() {}


} // iridium::io::implementation::platform


#endif // MACOS_PLATFORM
