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
        for (auto const &ident_handle: m_map_ident_handle)
            ::close(ident_handle.first);

        m_map_ident_handle.clear();
        m_map_uri_idents.clear();
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
        std::vector<int> idents;

        THandle::TSharedPtr handle;

        {
            LOCK_SCOPE();
            auto uri_idents  = m_map_uri_idents.find(uri);
            // peer
            if ( uri_idents != m_map_uri_idents.end()) {
                idents = assertComplete(uri_idents->second, "idents are empty");
                idents = uri_idents->second;

                auto ident_handle  = m_map_ident_handle.find(uri_idents->second.front());
                if ( ident_handle != m_map_ident_handle.end())
                    handle = ident_handle->second;
            }
        }

        LOGT << "idents: " << idents;

        if (idents.empty())
            idents = connect(uri, IContext::TStreamType::WRITER);

        LOGT << "idents: " << idents;

        bool is_new = false;
        if(!handle) {
            handle              = THandle::create();
            handle->context     = CContext::create();
            handle->protocol    = protocol;
            is_new = true;
        }

        handle->uri     = uri;
        handle->idents  = assertComplete(idents, "empty idents");

        {
            LOCK_SCOPE();

            for (int const &ident: idents) {
                if (ident < 0)
                    continue; // <---
                m_map_ident_handle[ident] = handle;
            }
            m_map_uri_idents[uri] = idents;
        }

        if (is_new) {
            auto event = TEvent::create();
            event->context      = handle->context;
            event->operation    = TEvent::TOperation::OPEN;
            event->uri          = uri;
            protocol->control(event);
            std::map<IProtocol::TSharedPtr, std::list<IContextActions::TAction> > m;
            m[protocol] = handle->context->getActions();
            updateHandles(m);
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

        int ident = assertOK(::socket(domain, type, protocol), "socket");

        static int const YES = 1;
        assertOK(
            ::setsockopt(ident, SOL_SOCKET, SO_REUSEADDR, &YES, sizeof(YES)),
             "setsockopt REUSEADDR");
        assertOK(
            ::setsockopt(ident, SOL_SOCKET, SO_REUSEPORT, &YES, sizeof(YES)),
             "setsockopt REUSEPORT");

        int flags = assertOK(
            ::fcntl(ident, F_GETFL, 0),
             "fcntl GETFL");
        assertOK(
            ::fcntl(ident, F_SETFL, flags | O_NONBLOCK),
             "fcntl SETFL O_NONBLOCK");

        assertOK(
            ::bind(ident, reinterpret_cast<sockaddr*>(&address), address_length),
             "bind");

        if (type == SOCK_STREAM)
            assertOK(
                ::listen(ident, SOMAXCONN),
                 "listen");

        struct kevent event;
        EV_SET(&event, ident, EVFILT_READ, EV_ADD |
            (type == SOCK_DGRAM ? EV_CLEAR : 0), 0, 0, reinterpret_cast<void *>(1));

        assertOK(
            ::kevent(m_kqueue, &event, 1, nullptr, 0, nullptr),
             "kevent");

        auto handle = THandle::create();
        handle->acceptor    = acceptor;
        handle->uri         = uri;
        handle->idents      = { ident };

        {
            LOCK_SCOPE();
            LOGT << "add acceptor, ident: " << ident << ", uri: " << uri;
            m_map_ident_handle[ident]       = handle;
            m_map_uri_idents[handle->uri]   = { ident };
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
    LOGT << "connect: " << uri;
    std::vector<int> idents;
    if (uri->getProtocol() == URI::TProtocol::PROCESS) {
        int stdin_pipe[2]  = { -1, -1 };
        int stdout_pipe[2] = { -1, -1 };

#ifdef LINUX_PLATFORM
        assertOK(pipe2 (stdin_pipe, O_CLOEXEC | O_NONBLOCK), "pipe stdin");
        assertOK(pipe2(stdout_pipe, O_CLOEXEC | O_NONBLOCK), "pipe stdout");
#endif

#ifdef MACOS_PLATFORM
        assertOK(pipe(stdin_pipe),  "pipe stdin");
        assertOK(pipe(stdout_pipe), "pipe stdout");
        // non-blocking
        static auto setNonblock = [] (int ident) {
            int flags_fd = assertOK(
                ::fcntl(ident, F_GETFD, 0),
                 "fcntl GETFD");
            assertOK(
                ::fcntl(ident, F_SETFD, flags_fd | FD_CLOEXEC),
                 "fcntl SETFD FD_CLOEXEC");
            int flags_fl = assertOK(
                ::fcntl(ident, F_GETFL, 0),
                 "fcntl GETFL");
            assertOK(
                ::fcntl(ident, F_SETFL, flags_fl | O_NONBLOCK),
                 "fcntl SETFL O_NONBLOCK");
        };

        setNonblock(stdin_pipe[1]);   // Parent -> Child STDIN
        setNonblock(stdout_pipe[0]);  // Parent <- Child STDOUT
#endif

        LOGT << "::execlp(\"" << uri->getPath()
        << "\", \"" <<  uri->getHost()
        << "\", \"" << uri->getArguments()
        << "\", nullptr);";

        std::vector<char const *> argv;
        // argv[0]
        argv.push_back(uri->getPath().c_str());
        for (auto const &arg: split(uri->getArguments(), " "))
            argv.push_back(arg.c_str());
        argv.push_back(nullptr);

        // fork
        pid_t pid = assertOK(::fork(), "fork");

        if (pid == 0) {
            // ----- CHILD
            assertOK(::dup2(stdin_pipe[0],  STDIN_FILENO),  "dup2 stdin");
            assertOK(::dup2(stdout_pipe[1], STDOUT_FILENO), "dup2 stdout");
            assertOK(::dup2(STDOUT_FILENO, STDERR_FILENO),  "dup2 stderr");

            ::close(stdin_pipe[0]);
            ::close(stdin_pipe[1]);
            ::close(stdout_pipe[0]);
            ::close(stdout_pipe[1]);

            ::signal(SIGPIPE, SIG_DFL);
            ::signal(SIGSEGV, SIG_DFL);
            ::signal(SIGABRT, SIG_DFL);
            ::signal(SIGFPE,  SIG_DFL);
            ::signal(SIGILL,  SIG_DFL);

            ::execvp(uri->getPath().c_str(), const_cast<char * const *>(argv.data()));

            auto error = string(std::strerror(errno)) + "\n";
            ::write(STDOUT_FILENO, error.c_str(), error.size());
            _exit(127);
            // -----
        }

        // ----- PARENT
        ::close(stdin_pipe[0]);
        ::close(stdout_pipe[1]);

        idents = { static_cast<int>(pid), stdout_pipe[0], stdin_pipe[1] };

        return idents; // ----->
    }

    if (uri->getProtocol() == URI::TProtocol::FILE) {
        // struct kevent event;
        if (stream_type == IContextActions::TStreamType::WRITER) {
            auto ident = assertOK(
                ::open(uri->getPath().c_str(), O_WRONLY | O_CREAT),
                 "open file WRONLY");
            assertOK(
                ::flock(ident, LOCK_EX | LOCK_NB),
                 "flock");
            idents = { ident };
            // EV_SET(&event, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, nullptr);
            // flock(fd, LOCK_UN);
        } else {
            auto ident = assertOK(
                ::open(uri->getPath().c_str(), O_RDONLY),
                 "open file RDONLY");
            idents = { ident };
            // EV_SET(&event, fd, EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, nullptr);
        }
        return idents; // ----->
    }

    int domain      = -1;
    int type        = -1;
    int protocol    =  0;

    struct sockaddr_storage
        address {};
    socklen_t
        address_length = 0;

    if (uri->getProtocol() == URI::TProtocol::UDP) {
        domain      = AF_INET;
        type        = SOCK_DGRAM;
        protocol    = IPPROTO_UDP;

        auto *address_in = reinterpret_cast<sockaddr_in*>(&address);

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

        auto *address_unix          = reinterpret_cast<sockaddr_un *>(&address);
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

    int ident = assertOK(
        ::socket(domain, type, protocol),
         "socket");

    static int const YES = 1;
    assertOK(
        ::setsockopt(ident, SOL_SOCKET, SO_REUSEADDR, &YES, sizeof(YES)),
         "setsockopt REUSEADDR");
    assertOK(
        ::setsockopt(ident, SOL_SOCKET, SO_REUSEPORT, &YES, sizeof(YES)),
         "setsockopt REUSEPORT");

    int flags = assertOK(
        ::fcntl(ident, F_GETFL, 0),
         "fcntl GETFL");
    assertOK(
        ::fcntl(ident, F_SETFL, flags | O_NONBLOCK),
         "fcntl SETFL O_NONBLOCK");

    int result = ::connect(ident, reinterpret_cast<sockaddr*>(&address), address_length);
    if (result < 0 && errno != EINPROGRESS)
        assertOK(result, "connect");

    // struct kevent event;
    // EV_SET(&event, fd, EVFILT_READ, EV_ADD | (type == SOCK_DGRAM ? EV_CLEAR : 0), 0, 0, nullptr);
    // assertOK(::kevent(m_kqueue, &event, 1, nullptr, 0, nullptr), "kevent READ");
    // if (result < 0 && errno == EINPROGRESS) {
    //     EV_SET(&event, fd, EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, nullptr);
    //     assertOK(::kevent(m_kqueue, &event, 1, nullptr, 0, nullptr), "kevent WRITE (pending)");
    // }

    return idents;
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
        auto const *in = reinterpret_cast<const sockaddr_in *>(&address);
        inet_ntop(AF_INET, &in->sin_addr, ip, sizeof(ip));
        port = ntohs(in->sin_port);
        return URI::create("tcp://" + string(ip) + ":" + convert<string>(port));
    }

    else

    if (address.ss_family == AF_INET6) {
        auto const *in6 = reinterpret_cast<const sockaddr_in6 *>(&address);
        inet_ntop(AF_INET6, &in6->sin6_addr, ip, sizeof(ip));
        port = ntohs(in6->sin6_port);
        return URI::create("tcp://[" + string(ip) + "]:" + convert<string>(port));
    }

    throw std::runtime_error("get peer uri error: unknown sockaddr type");
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

        auto ident_handle  = m_map_ident_handle.find(static_cast<int>(event.ident));
        if ( ident_handle == m_map_ident_handle.end()) {
            LOGW << "connection manager: get handle error: not registered ident: " << event.ident;
            ::close(static_cast<int>(event.ident));
            continue; // <---
        } else {
            if (ident_handle->second->protocol) {
                if (event.filter == EVFILT_READ)
                    ident_handle->second->stream_type = IContextActions::TStreamType::READER;
                if (event.filter == EVFILT_WRITE)
                    ident_handle->second->stream_type = IContextActions::TStreamType::WRITER;
            }
            result.push_back(ident_handle->second);
        }
    }

    return result; // ----->

    // if (event.filter == EVFILT_PROC && (event.fflags & NOTE_EXIT)) {
    //     // m_manager->handleEvent(event.ident, TEvent::TOperation::CLOSE, nullptr, 0);
    //     // clean zombie
    //     ::waitpid(static_cast<pid_t>(event.ident), nullptr, 0);
    // }
}


void CConnectionManager::updateHandles(
    std::list<THandle::TSharedPtr>
        const &handles)
{
    LOCK_SCOPE();

    for (auto const &handle: handles) {
        if (handle->protocol) {
            for (auto const &ident: handle->idents)
                m_map_ident_handle[ident] = handle;
            m_map_uri_idents[handle->uri] = handle->idents;
        } else {
            for (auto const &ident: handle->idents)
                m_map_ident_handle.erase(ident);
            m_map_uri_idents.erase(handle->uri);
        }
    }
}


void CConnectionManager::updateHandles(
    std::map<IProtocol::TSharedPtr, std::list<IContextActions::TAction> >
        const &map_protocol_actions)
{
    std::vector<struct kevent>
        batch;
    std::list< std::pair<int, URI::TSharedPtr> >
        ident_uri_list_to_close;
    std::unordered_set<URI::TSharedPtr>
        just_opened_uris;

    for(auto const &protocol_actions: map_protocol_actions) {
        auto const &protocol = protocol_actions.first;
        for (auto const &action: protocol_actions.second) {
            if (action.action_type == IContextActions::TActionType::OPEN) {
                manage(action.uri, protocol);
                just_opened_uris.insert(action.uri);
                continue; // <---
            }

            // todo: optimize
            std::vector<int> idents;

            {
                LOCK_SCOPE();

                auto uri_idents  = m_map_uri_idents.find(action.uri);
                if ( uri_idents == m_map_uri_idents.end()) {
                    throw std::runtime_error(
                        "update handle error: ident not found by " +
                        convert<string>(action.uri)); // ----->
                } else {
                    idents = uri_idents->second;
                }

                if (action.action_type == IContextActions::TActionType::CLOSE) {
                    for (auto const &ident: idents)
                        ident_uri_list_to_close.push_back( { ident, action.uri } );
                    continue; // <---
                }
            }

            short filter = 0;
            if (action.stream_type == IContextActions::TStreamType::READER)
                filter = EVFILT_READ;
            if (action.stream_type == IContextActions::TStreamType::WRITER)
                filter = EVFILT_WRITE;

            short flags = 0;
            if (action.action_type == IContextActions::TActionType::SUBSCRIBE) {
                flags = EV_ADD | EV_CLEAR;
            }

            if (action.action_type == IContextActions::TActionType::UNSUBSCRIBE) {
                // skip processes with stdin / stdout idents
                flags = EV_DELETE;
            }

            if (idents.size() == 1) {
                struct kevent event;
                EV_SET(&event, idents[0], filter, flags, 0, 0, nullptr);
                batch.push_back(event);
                LOGT << "EV_SET: " << idents[0] << " " << filter << " " << flags;
            }

            else

            if (idents.size() == 3) {
                struct kevent event;

                // 1. Отслеживание завершения процесса (pid = idents[0])
                // Добавляем только при открытии или закрытии, чтобы не дублировать подписку
                // if (just_opened_uris.count(action.uri) || action.action_type == IContextActions::TActionType::CLOSE) {
                //     EV_SET(&event, idents[0], EVFILT_PROC, flags, NOTE_EXIT, 0, nullptr);
                //     batch.push_back(event);
                //     LOGT << "EV_SET: " << idents[0] << " EVFILT_PROC " << flags;
                // }

                // 2. Подписка строго на нужный конец канала в зависимости от stream_type
                if (action.stream_type == IContextActions::TStreamType::READER) {
                    // idents[1] = stdout процесса (мы из него читаем)
                    EV_SET(&event, idents[1], EVFILT_READ, flags, 0, 0, nullptr);
                    batch.push_back(event);
                    LOGT << "EV_SET: " << idents[1] << " EVFILT_READ " << flags;
                }
                else if (action.stream_type == IContextActions::TStreamType::WRITER) {
                    // idents[2] = stdin процесса (мы в него пишем)
                    EV_SET(&event, idents[2], EVFILT_WRITE, flags, 0, 0, nullptr);
                    batch.push_back(event);
                    LOGT << "EV_SET: " << idents[2] << " EVFILT_WRITE " << flags;
                }
            }
        }
    }

    if (batch.empty())
        return; // ----->

    assertOK(
        ::kevent(m_kqueue, batch.data(), static_cast<int>(batch.size()), nullptr, 0, nullptr),
         "kevent actions batch");

    LOCK_SCOPE();
    for (auto const &ident_uri: ident_uri_list_to_close) {
        ::close(ident_uri.first);
        m_map_ident_handle.erase(ident_uri.first);
        m_map_uri_idents.erase(ident_uri.second);
    }
}


void CConnectionManager::releaseHandle(THandle::TSharedPtr const &handle) {
    LOCK_SCOPE();
    for (auto ident_handle = m_map_ident_handle.begin(); ident_handle != m_map_ident_handle.end(); ) {
        if (ident_handle->second == handle) {
            ident_handle = m_map_ident_handle.erase(ident_handle);
            ::close(ident_handle->first);
        } else {
            ident_handle++;
        }
    }

    m_map_uri_idents.erase(handle->uri);
    LOGT << "RELEASE PROTOCOL";
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
            handles_to_update;

        std::map<IProtocol::TSharedPtr, std::list<IContextActions::TAction> >
            map_protocol_actions;

        LOGT << "NEXT";
        for (auto const &handle: m_manager->getHandles(triggered_events, count)) {
            LOGT << "event handle: " << handle->uri << " " << handle->stream_type << ", idents: " << handle->idents;
            if (handle->acceptor) {
                while (true) {
                    sockaddr_storage
                        address {};
                    socklen_t
                        address_length = sizeof(address);

                    int peer_ident = ::accept(
                        assertOne(handle->idents, "few idents for acceptor not allowed"),
                        reinterpret_cast<struct sockaddr *>(&address),
                        &address_length);

                    if (peer_ident == -1) {
                        if (errno == EAGAIN || errno == EWOULDBLOCK)
                            break; // --->
                        if (errno == EINTR)
                            continue; // <---
                        LOGW << "peer accept error: " << string(std::strerror(errno));
                        break; // --->
                    }

                    try {
                        auto peer_handle = CConnectionManager::THandle::create();
                        peer_handle->idents     = { peer_ident };
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
                            // add peer ident to map
                            auto &target_list = map_protocol_actions[peer_handle->protocol];
                            target_list.splice(target_list.end(), peer_handle->context->getActions());
                        } else {
                            // close peer after accepting
                            ::close(peer_ident);
                            continue; // <---
                        }

                        handles_to_update.push_back(peer_handle);
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
                    int ident = -1;
                    if (handle->idents.size() == 3)
                        ident = handle->idents[1];
                    else
                    if (handle->idents.size() == 1)
                        ident = handle->idents[0];
                    else
                        throw std::runtime_error("wrong idents size " + convert<string>(handle->idents));

                    auto buffer = Buffer::create(DEFAULT_BUFFER_SIZE);
                    auto n      = ::read(ident, buffer->data(), buffer->capacity());

                    LOGT << "read, n: " << n;

                    if (n > 0) {
                        buffer->resize(n);

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
                    int ident = -1;
                    if (handle->idents.size() == 3)
                        ident = handle->idents[2];
                    else
                    if (handle->idents.size() == 1)
                        ident = handle->idents[0];
                    else
                        throw std::runtime_error("wrong idents size " + convert<string>(handle->idents));

                    auto buffers= handle->context->getBuffers(handle->uri, IContextActions::TStreamType::WRITER);
                    auto buffer = assertExists(buffers.front(), "buffer to write is empty");
                    auto n      = ::write(ident, buffer->data(), buffer->size());

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

                if (!result) {
                    // delete all pipes
                    handle->context->delPipe("");
                    handle->protocol.reset();
                }

                auto &target_list = map_protocol_actions[handle->protocol];
                target_list.splice(target_list.end(), handle->context->getActions());
            }
            // update maps
            m_manager->updateHandles(handles_to_update);
            // update kqueue
            m_manager->updateHandles(map_protocol_actions);
        }
    }
}


void CConnectionManager::CKEventRunnable::initialize() {}


void CConnectionManager::CKEventRunnable::finalize() {}


} // iridium::io::implementation::platform


#endif // MACOS_PLATFORM
