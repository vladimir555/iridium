// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SESSION_MANAGER_D7D8E1E1_DE14_43A0_AAFC_8E95DFFE4453
#define HEADER_SESSION_MANAGER_D7D8E1E1_DE14_43A0_AAFC_8E95DFFE4453


#include "iridium/io/context.h"
#include "iridium/io/context_manager.h"
#include "iridium/io/session_manager.h"
#include "iridium/io/multiplexer.h"
#include "iridium/threading/thread.h"
#include "iridium/threading/runnable.h"
#include "iridium/threading/worker.h"
#include "iridium/threading/synchronized.h"

#include <unordered_map>
#include <unordered_set>
#include <mutex>


namespace iridium::io::implementation {


class CSessionManager: public ISessionManager {
public:
    DEFINE_IMPLEMENTATION(CSessionManager);

    CSessionManager();

    void initialize() override;
    void finalize() override;

    void manage(IStreamPort::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol) override;

private:
    typedef threading::IWorker<Event::TSharedPtr> IContextWorker;

    class CMultiplexerThreadHandler: public iridium::threading::IRunnable {
    public:
        DEFINE_IMPLEMENTATION(CMultiplexerThreadHandler)

        CMultiplexerThreadHandler(
            IContextWorker::TSharedPtr  const &context_worker,
            IContextManager::TSharedPtr const &context_manager,
            IMultiplexer::TSharedPtr    const &multiplexer
        );

        void initialize() override;
        void finalize() override;

        void run(std::atomic<bool> &is_running) override;
    private:
        IContextWorker::TSharedPtr  m_context_worker;
        IContextManager::TSharedPtr m_context_manager;
        IMultiplexer::TSharedPtr    m_multiplexer;
    };

    class CContextWorkerHandler: public IContextWorker::IHandler {
    public:
        DEFINE_IMPLEMENTATION(CContextWorkerHandler)
        CContextWorkerHandler(IContextManager::TSharedPtr const &context_manager, IMultiplexer::TSharedPtr const &multiplexer);
        void initialize() override;
        void finalize() override;
        TOutputItems handle(TInputItems const &events) override;
    private:
        IContextManager::TSharedPtr
            m_context_manager;
        IMultiplexer::TSharedPtr
            m_multiplexer;
    };

    IMultiplexer::TSharedPtr
        m_multiplexer;
    IContextManager::TSharedPtr
        m_context_manager;
    IPipeManager::TSharedPtr
        m_pipe_manager;
    IContextWorker::TSharedPtr
        m_context_worker;
    threading::IThread::TSharedPtr
        m_multiplexer_thread;
};



} // iridium::io::implementation


#endif // HEADER_SESSION_MANAGER_D7D8E1E1_DE14_43A0_AAFC_8E95DFFE4453
