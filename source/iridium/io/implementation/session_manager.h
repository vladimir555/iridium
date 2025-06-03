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


namespace iridium {
namespace io {
namespace implementation {


// todo:
// fsm: kevent, epoll, iocp
// session: fsm state + pipes
// stream statuses:
// open,    begin   -> fsm
// open,    end     -> fsm -> protocol -> add, rm, modify pipes
// read,    begin   -> fsm.
// read,    end     -> fsm -> protocol
// write,   begin   -> fsm
// write,   end     -> fsm -> protocol
// close,   begin   -> fsm
// close,   end     -> fsm -> protocol

// example(kevent or epoll):
// stream -> manage -> event(open, begin) -> multiplexer(subscribe) +
// worker(initialize) -> event(open, end) ->
// worker(protocol) -> pipe(event(read, begin)) ->
// worker(read) -> event(read, end) -> if read size > 0 then repeat event(read, begin) + worker(protocol) -> context still valid, no event
// ...
// kevent -> event(read, begin) -> repeat worker(read) or event(timeout, unknown) -> worker(protocol) ->
// ...
// worker(protocol) -> not valid context -> event(close, begin) ->
// unsubscribe ->
// kevent -> event(close, end) ->
// worker(finalize + remove context)

// example(iocp):
// stream -> manage -> event(open, begin) ->
// worker(initialize) -> event(open, end) ->
// worker(protocol) -> pipe(event(read, begin)) ->
// worker(read) -> none
// iocp -> event(read, end) -> if size > 0 then repeat + worker(protocol) ->
// worker(protocol) -> not valid context -> event(close, begin) ->


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

    class CEventRepeaterHandler: public threading::IRunnable {
    public:
        DEFINE_IMPLEMENTATION(CEventRepeaterHandler)
        
        CEventRepeaterHandler(IContextWorker::TSharedPtr const &context_worker);

        void initialize() override;
        void finalize() override;

        void run(std::atomic<bool> &is_running) override;
    private:
        IContextWorker::TSharedPtr
            m_context_worker;
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
    threading::IThread::TSharedPtr
        m_event_repeater_thread;
};



} // implementation
} // io
} // iridium


#endif // HEADER_SESSION_MANAGER_D7D8E1E1_DE14_43A0_AAFC_8E95DFFE4453
