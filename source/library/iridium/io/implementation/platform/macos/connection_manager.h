// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONNECTION_MANAGER_DC986A73_D371_47C7_9DA9_E36F0DF5CC75
#define HEADER_CONNECTION_MANAGER_DC986A73_D371_47C7_9DA9_E36F0DF5CC75


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


#include "iridium/enum.h"
#include "iridium/convertion/convert.h"
#include "iridium/io/context.h"
#include "iridium/io/connection_manager.h"
#include "iridium/threading/synchronized.h"
#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"

#include "../unix/connection_manager.h"


#include <unordered_map>
#include <list>
#include <mutex>
#include <atomic>

#include <sys/event.h>
#include <cstring>
#include <netinet/in.h>


namespace iridium::io::implementation::platform {


class CConnectionManager:
    public IConnectionManager,
    public unix_::CConnectionManager,
    public threading::Synchronized<std::mutex>
{
public:
    DEFINE_IMPLEMENTATION(CConnectionManager)

    CConnectionManager();

    void initialize() override;
    void finalize() override;

    // client
    void manage(
        URI::TSharedPtr
            const &uri,
        IProtocol::TSharedPtr
            const &protocol) override;
    // server
    void manage(
        URI::TSharedPtr
            const &uri,
        IAcceptor::TSharedPtr
            const &acceptor) override;
private:
    class CKEventRunnable: public threading::IRunnable {
    public:
        DEFINE_IMPLEMENTATION(CKEventRunnable);
        CKEventRunnable(CConnectionManager * const manager);
        void run(std::atomic<bool> &is_running) override;
        void initialize() override;
        void finalize() override;
    private:
        CConnectionManager *
            const m_manager;
    };

    friend class CKEventRunnable;

    template<typename T>
    static T assertOK(T const &result, std::string const &message);

    static URI::TSharedPtr
        getPeerURI(sockaddr_storage const &address);
    URI::TSharedPtr
        getPeerURI(int const &fd);

    struct THandle {
        DEFINE_CREATE(THandle)
        URI::TSharedPtr
            uri;
        IProtocol::TSharedPtr
            protocol;
        IAcceptor::TSharedPtr
            acceptor;
        IContextActions::TSharedPtr
            context;
    };

    struct TTCPPeer {
        DEFINE_CREATE(TTCPPeer);
        URI::TSharedPtr
            uri;
        int
            fd;
    };

    std::list<THandle::TSharedPtr>
        getHandles(std::vector<struct kevent> const &events);

    std::vector<int> connect(
        URI::TSharedPtr
            const &uri,
        bool
            const &is_writer = false);

    void wakeKEvent(int const &code);

    // IContextActions::TSharedPtr
    //     handleEvent(
    //         std::vector<int>
    //             const &fds,
    //         TEvent::TOperation
    //             const &operation,
    //         Buffer::TSharedPtr
    //             const &read_buffer,
    //         size_t
    //             const &written_bytes_count);

    // IContextActions::TSharedPtr
    //     handleEvent(
    //         int
    //             const &acceptor_fd,
    //         std::vector<TTCPPeer::TSharedPtr>
    //             const &peers);

    threading::IThread::TSharedPtr
        m_thread;
    std::unordered_map<int, URI::TSharedPtr>
        m_map_fd_uri;
    std::unordered_map<URI::TSharedPtr, std::vector<int> >
        m_map_uri_fd;
    std::unordered_map<URI::TSharedPtr, IProtocol::TSharedPtr>
        m_map_uri_protocol;
    std::unordered_map<int, IAcceptor::TSharedPtr>
        m_map_fd_acceptor;
    std::unordered_map<URI::TSharedPtr, IContextActions::TSharedPtr>
        m_map_uri_context;
    std::atomic<int>
        m_kqueue;
};


template<typename T>
T CConnectionManager::assertOK(T const &result, std::string const &message) {
    if (result < 0)
        throw std::runtime_error(message + ". " + std::strerror(errno) +
          ", code " + iridium::convertion::convert<std::string>(errno)); // ----->

    return result; // ----->
}


} // iridium::io::implementation::platform


#endif // MACOS_PLATFORM


#endif // HEADER_CONNECTION_MANAGER_DC986A73_D371_47C7_9DA9_E36F0DF5CC75
