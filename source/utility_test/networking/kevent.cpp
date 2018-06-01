//
//  kevent.cpp
//  utility_test
//
//  Created by volodja on 14.05.2018.
//


#include <gtest/gtest.h>
#include <utility/logging/logger.h>
#include <utility/networking/url.h>

#include <vector>
#include <list>
#include <map>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <unistd.h>
#include <signal.h>

#include <sys/event.h>

#include <utility/pattern/initializable.h>


using utility::logging::update;
using utility::logging::config::createDefaultConsoleLoggerConfig;
using utility::networking::URL;
using utility::convertion::convert;


DEFINE_ENUM(
    TEventFlag,
    ADD             = EV_ADD,
    DELETE          = EV_DELETE,
    ENABLE          = EV_ENABLE,
    DISABLE         = EV_DISABLE,

    ONESHOT         = EV_ONESHOT,
    CLEAR           = EV_CLEAR,
    RECEIPT         = EV_RECEIPT,
    DISPATCH        = EV_DISPATCH,
    UDATA_SPECIFIC  = EV_UDATA_SPECIFIC,
    DISPATCH2       = EV_DISPATCH | EV_UDATA_SPECIFIC,
    VANISHED        = EV_VANISHED,
    SYSFLAGS        = EV_SYSFLAGS,
    FLAG0           = EV_FLAG0,
    FLAG1           = EV_FLAG1,

    EOF_            = EV_EOF,
    ERROR           = EV_ERROR
)


IMPLEMENT_ENUM(TEventFlag)


DEFINE_ENUM(TEvenFilterFlag,
    READ            = EVFILT_READ,
    WRITE           = EVFILT_WRITE,
    AIO             = EVFILT_AIO,
    VNODE           = EVFILT_VNODE,
    PROC            = EVFILT_PROC,
    SIGNAL          = EVFILT_SIGNAL,
    TIMER           = EVFILT_TIMER,
    MACHPORT        = EVFILT_MACHPORT,
    FS              = EVFILT_FS,
    USER            = EVFILT_USER,
    VM              = EVFILT_VM,
    EXCEPT          = EVFILT_EXCEPT,
    SYSCOUNT_THREADMARKER = EVFILT_THREADMARKER
)


IMPLEMENT_ENUM(TEvenFilterFlag)


void handleSignal(int signal) {
    LOGT << "broken pipe signal " << signal;
}


template<typename T>
T assertOK(T const &result, std::string const &message) {
    if (result < 0)
        throw std::runtime_error(message + ": " +
              std::strerror(errno) + ", code " + convert<std::string>(errno)); // ----->
    else
        return result; // ----->
}


void setBlockingMode(int const &m_socket_fd, bool const &is_blocking) {
    auto flags = assertOK(fcntl(m_socket_fd, F_GETFL, 0), "socket get flag error");
    if (is_blocking)
        flags &= !O_NONBLOCK;
    else
        flags |=  O_NONBLOCK;
    assertOK(fcntl(m_socket_fd, F_SETFL, flags), "socket set flag error");
}


int const DEFAULT_SOCKET_BUFFER_SIZE = 4;


size_t write(int const &m_socket_fd, std::vector<char> const &packet, size_t const &size) {
    auto buffer = static_cast<void const *>(packet.data());
    auto result = ::send(m_socket_fd, buffer, DEFAULT_SOCKET_BUFFER_SIZE, 0);
    LOGT << "result " << result;
    if (result == EAGAIN)
        return DEFAULT_SOCKET_BUFFER_SIZE; // ----->
    else
        return result; // ----->
}

struct TPeer {
    DEFINE_ENUM(TAction, OPEN, READ, WRITE, CLOSE);

    // todo: send read and write buffer to protocol without action;
    
    std::vector<char>   read_buffer;
    std::vector<char>   write_buffer;
    size_t              write_buffer_pos;
    int                 peer_fd;
    TAction             action;
    int                 process_count;
};


IMPLEMENT_ENUM(TPeer::TAction)


typedef std::vector<unsigned char> TBuffer;


// namespace protocol
class IStream {
public:
    DEFINE_INTERFACE(IStream)
    virtual void close() = 0;
};


class IStreamReader: public IStream {
public:
    DEFINE_INTERFACE(IStreamReader)
    // empty result means eof
    virtual TBuffer read(size_t const &size) = 0;
};


class IStreamWriter: public IStream {
public:
    DEFINE_INTERFACE(IStreamWriter)
    virtual size_t write(TBuffer const &packet) = 0;
};


class IProtocol {
public:
    DEFINE_INTERFACE(IProtocol)

    DEFINE_ENUM(TState, READ, WRITE, CLOSE);
    
//    struct TAction {
//        TState  state;
//        TBuffer buffer;
//    };
//
//    virtual TAction read(TBuffer const &buffer) = 0;
    
    IStreamReader::TSharedPtr handle(TBuffer const &buffer);
    
};


class IProtocolFactory {
public:
    DEFINE_INTERFACE(IProtocolFactory)
    virtual IProtocol::TSharedPtr create() = 0;
};


class Peer {
public:
    DEFINE_SMART_PTR(Peer)
    DEFINE_CREATE(Peer);
    
    Peer(IStream::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol);
   ~Peer() = default;
    
    void handle();
    
private:
    IStream::TSharedPtr     m_stream;
    IProtocol::TSharedPtr   m_protocol;
    TBuffer                 m_buffer;
    IProtocol::TState       m_state;
    size_t                  m_index;
};


Peer::Peer(IStream::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol)
:
    m_stream    (stream),
    m_protocol  (protocol),
    m_state     (IProtocol::TState::READ),
    m_index     (0)
{}


IMPLEMENT_ENUM(IProtocol::TState)


void Peer::handle() {
    switch (m_state) {
        case IProtocol::TState::READ:
//            m_buffer.emplace_back(m_stream->read(DEFAULT_SOCKET_BUFFER_SIZE));
            break;
        case IProtocol::TState::WRITE:
//            m_stream->write(m_buffer);
            break;
        case IProtocol::TState::CLOSE:
            m_stream->close();
            break;
        default:
            throw std::runtime_error("peer wrong state: " + convert<std::string>(m_state)); // ----->
            break;
    }
}


struct TEvent {
    
};


class CSocket: public utility::pattern::IInitializable {
public:
    void initialize() override;
    void finalize() override;
    std::list<Peer::TSharedPtr> accept();
};


TEST(example, kevent) {
    update(createDefaultConsoleLoggerConfig());
    
    LOGT << "begin";
    
    URL m_url("http://127.0.0.1:55555");
    
    size_t const MAX_EVENT_COUNT            = 3;
    int    const KEVENT_TIMIOUT_MS          = 1000;

    std::vector<struct kevent>  m_events        (MAX_EVENT_COUNT - 1, { 0 });
    std::vector<struct kevent>  m_monitor_events(m_events.size() + 1, { 0 });
    int                         m_monitor_events_used_count = 0;
    int                         m_kqueue;
    int                         m_socket_fd = 0;

    LOGT << "open";
    auto protocol   = IPPROTO_TCP;
    if (m_url.getProtocol() == URL::TProtocol::UDP)
        protocol    = IPPROTO_UDP;
    
    m_socket_fd     = assertOK(::socket(AF_INET, SOCK_STREAM, protocol), "socket open error");
    LOGT << "open OK, fd " << m_socket_fd;

    LOGT << "listen";
    setBlockingMode(m_socket_fd, false);
    struct sockaddr_in server_address = { 0 };
    
    server_address.sin_family       = AF_INET;
    auto ipv4                       = *m_url.getIPv4();
    server_address.sin_addr.s_addr  = htonl(
        ( ipv4[0] << 24 ) | ( ipv4[1] << 16 ) |
        ( ipv4[2] << 8  ) |   ipv4[3]);
    server_address.sin_port         = htons(*m_url.getPort());
    
    int yes = 1;
    setsockopt(m_socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    
    assertOK(::bind  (m_socket_fd, (struct sockaddr *) (&server_address), sizeof(server_address)), "socket bind error");
    assertOK(::listen(m_socket_fd, SOMAXCONN), "socket listen error");
    
    LOGT << "set empty signal handler for broken pipe";
    struct sigaction sh;
    struct sigaction osh;
    
    // can set to SIG_IGN
    sh.sa_handler   = &handleSignal;
    // restart interrupted system calls
    sh.sa_flags     = SA_RESTART;
    
    // block every signal during the handler
    sigemptyset(&sh.sa_mask);
    
    if (sigaction(SIGPIPE, &sh, &osh) < 0)
        throw std::runtime_error("sigaction error");

    // -----
    m_kqueue                    = assertOK(kqueue(), "socket kqueue create error");
    m_monitor_events[0]         = { static_cast<uintptr_t>(m_socket_fd), EVFILT_READ | EVFILT_WRITE, EV_ADD, 0, 0, nullptr };
    m_monitor_events_used_count = 1;
    LOGT << "listen OK";
    
    static struct timespec const timeout = { KEVENT_TIMIOUT_MS / 1000, KEVENT_TIMIOUT_MS % 1000 };
    
    std::map<int, TPeer> map_fd_peer;
    
    while(true) {
        LOGT << "event";

        sleep(1);
        
        auto events_count = assertOK(
             kevent(m_kqueue,
                    m_monitor_events.data(), m_monitor_events_used_count,
                    m_events.data(), m_events.size() & std::numeric_limits<int>::max(),
                    &timeout),
             "socket resolving kevent error");
        m_monitor_events_used_count = 1;
        
        for (int i = 0; i < events_count; i++) {
            if (m_events[i].ident == 0)
                continue; // <---
            
            LOGT << "fd " << m_events[i].ident << " flags " << TEventFlag(m_events[i].flags).convertToFlagsString();
            LOGT << "accept fd " << m_socket_fd;
            
            if (m_events[i].flags & EV_EOF) {
                //            LOGT << getPeerURL(m_events[i].ident) << "EOF";
                if (m_monitor_events_used_count >= m_monitor_events.size())
                    continue; // <---
                m_monitor_events[m_monitor_events_used_count] =
                    { m_events[i].ident, EVFILT_READ | EVFILT_WRITE, EV_DELETE, 0, 0, nullptr };
                m_monitor_events_used_count++;
                continue; // <---
            }
            
            if (m_events[i].flags & EV_ERROR) {
                LOGE <<  "kevent error: " <<
                std::string(strerror(static_cast<int>(m_events[i].data))) << " " <<
                TEventFlag(m_events[i].flags).convertToFlagsString() <<
                " queue size " << m_monitor_events_used_count;
                continue; // <---
            }

            // accepted socket
            if (m_events[i].ident == m_socket_fd) {
                if (m_monitor_events_used_count >= m_monitor_events.size())
                    continue; // <---
                
                struct sockaddr_in  client_address      = { 0 };
                socklen_t           client_address_size = sizeof(client_address);
                int                 peer_fd             = 0;
                
                do {
                    peer_fd = ::accept(m_socket_fd, reinterpret_cast<struct sockaddr *>(&client_address), &client_address_size);
                    if  (peer_fd > 0) {

                        if (m_monitor_events_used_count >= m_monitor_events.size()) {
                            LOGT << "close " << peer_fd;
                            close(peer_fd); // <---
                            continue;
                        }
                        m_monitor_events[m_monitor_events_used_count] =
                            { static_cast<uintptr_t>(peer_fd), EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, nullptr };
                        m_monitor_events_used_count++;

                        TPeer peer;
                        
                        peer.action             = TPeer::TAction::OPEN;
                        peer.peer_fd            = peer_fd;
                        map_fd_peer[peer_fd]    = peer;
                        
                        setBlockingMode(peer_fd, false);
                        
                        LOGT << "event peer fd " << peer_fd << " action " << peer.action;
                    }
                } while (peer_fd > 0);
            } else {
                try {
                    auto peer_fd = static_cast<int>(m_events[i].ident);
                    
                    LOGT << "peer fd " << peer_fd << " event filter flags " << m_events[i].fflags <<
                    " " << TEvenFilterFlag(m_events[i].fflags).convertToFlagsString() <<
                    " filter " << TEvenFilterFlag(m_events[i].filter);
                    
                    if (m_events[i].filter == EVFILT_READ)
                        map_fd_peer[peer_fd].action = TPeer::TAction::READ;
                    if (m_events[i].filter == EVFILT_WRITE)
                        map_fd_peer[peer_fd].action = TPeer::TAction::WRITE;
                    
                    LOGT << "event peer fd " << peer_fd << " action " << map_fd_peer[peer_fd].action;
                } catch (std::exception const &e) {
                    LOGT << e.what();
                }
            }
            
            LOGT << "processing peers";
            for (auto &i: map_fd_peer) {
                auto &peer = i.second;
                LOGT << "processing peer_fd " << peer.peer_fd << " " << peer.action;
                
                if (peer.action == TPeer::TAction::READ) {
                    char buffer[DEFAULT_SOCKET_BUFFER_SIZE];
                    auto received_size  = assertOK(::recv(peer.peer_fd, buffer, DEFAULT_SOCKET_BUFFER_SIZE - 1, 0), "socket read error");
                    auto packet         = std::vector<char>(buffer, buffer + received_size);
                    peer.read_buffer.insert(peer.read_buffer.end(), packet.begin(), packet.end());
                    LOGT << "peer fd " << peer.peer_fd << " read buffer " << std::string(peer.read_buffer.begin(), peer.read_buffer.end());
                    
                    if (m_monitor_events_used_count >= m_monitor_events.size()) {
                        LOGT << "close " << peer.peer_fd;
                        close(peer.peer_fd); // <---
                        continue;
                    }
                    m_monitor_events[m_monitor_events_used_count] =
                        { static_cast<uintptr_t>(peer.peer_fd), EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, nullptr };
                    m_monitor_events_used_count++;
                }
                
                // protocol
                if (peer.read_buffer == std::vector<char>( { '1', '2', '3', '4', '5' } )) {
                    LOGT << "write by protocol";
                    peer.write_buffer   = { 'A', 'B', 'C', 'D', 'E' };
                    peer.action         = TPeer::TAction::WRITE;
                }
                
                if (peer.action == TPeer::TAction::WRITE) {
//                    LOGT << "peer fd " << peer.peer_fd << " write buffer " << std::string(peer.write_buffer.begin(), peer.write_buffer.end());

                    auto size       = write(peer.peer_fd, peer.write_buffer, peer.write_buffer_pos);
                    peer.write_buffer_pos += size;
                    
                    LOGT << "peer fd " << peer.peer_fd << " write buffer " << std::string(peer.write_buffer.begin() + size, peer.write_buffer.end());

                    if (m_monitor_events_used_count >= m_monitor_events.size()) {
                        LOGT << "close " << peer.peer_fd;
                        close(peer.peer_fd); // <---
                        continue;
                    }
                    m_monitor_events[m_monitor_events_used_count] =
                        { static_cast<uintptr_t>(peer.peer_fd), EVFILT_READ | EVFILT_WRITE, EV_ADD | EV_ONESHOT, 0, 0, nullptr };
                    m_monitor_events_used_count++;
                }
            }
            LOGT << "processing peers OK";
        }
        LOGT << "event OK";
    };
    
//    LOGT << "end";
}
