// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SESSION_MANAGER_D7D8E1E1_DE14_43A0_AAFC_8E95DFFE4453
#define HEADER_SESSION_MANAGER_D7D8E1E1_DE14_43A0_AAFC_8E95DFFE4453


#include "iridium/io/session.h"
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


class CSessionManager: public ISessionManager {
public:
    DEFINE_IMPLEMENTATION(CSessionManager);
    CSessionManager();

    void initialize() override;
    void finalize() override;
    void manage(IStreamPort::TSharedPtr const &stream, IProtocol::TSharedPtr const &protocol) override;

private:
    class ContextManager: public threading::Synchronized<std::mutex> {
    public:
        DEFINE_CREATE(ContextManager)

        class Context: public IPipeManager {
        public:
            DEFINE_CREATE(Context)

            Context(
                IMultiplexer::TSharedPtr    const &multiplexer,
                ContextManager            * const context_manager,
                IProtocol::TSharedPtr       const &protocol);
            virtual ~Context();

            IPipe::TSharedPtr   createPipe      (std::string const &name) override;
            void                removePipe      (std::string const &name) override;
            void                updateStream    (std::string const &name, IStream::TSharedPtr const &stream, IEvent::TType const &type);
            void                updateReader    (std::string const &name, IStreamReader::TSharedPtr const &reader) override;
            void                updateWriter    (std::string const &name, IStreamWriter::TSharedPtr const &writer) override;

            void                            pushEvent(IEvent::TSharedPtr const &event);
            std::list<IEvent::TSharedPtr>   popEvents();
            IProtocol::TSharedPtr           getProtocol() const;
            IPipe::TSharedPtr               findPipe(IStream::TSharedPtr const &stream) const;

        private:
            IPipe::TSharedPtr findPipe(std::string const &name) const;

            IMultiplexer::TSharedPtr
                m_multiplexer;
            IProtocol::TSharedPtr
                m_protocol;
            std::unordered_map<std::string, IPipe::TSharedPtr>
                m_map_name_pipe;
            std::unordered_map<IStream::TConstSharedPtr, IPipe::TSharedPtr>
                m_map_stream_pipe;
            threading::IAsyncQueue<IEvent::TSharedPtr>::TSharedPtr
                m_events;
        }; // Context

        ContextManager(IMultiplexer::TSharedPtr const &multiplexer);
        virtual ~ContextManager();

        Context::TSharedPtr acquireContext  (IEvent::TSharedPtr  const &event);
        void                releaseContext  (Context::TSharedPtr const &context);
        void                updateContext   (Context::TSharedPtr const &context, IStream::TSharedPtr const &stream);
        void                removeContext   (Context::TSharedPtr const &context);
        void                clear();

        std::list<IStream::TConstSharedPtr> getOutdatedStreams();
        void updateStreamTimestamp(IStream::TConstSharedPtr const &stream);

    private:
        IMultiplexer::TSharedPtr
            m_multiplexer;
        std::unordered_map<IStream::TSharedPtr, Context::TSharedPtr>
            m_map_stream_context;
        std::unordered_map<Context::TSharedPtr, std::unordered_set<IStream::TSharedPtr> >
            m_map_context_streams;
        std::unordered_map<IStream::TConstSharedPtr, std::chrono::system_clock::time_point>
            m_map_stream_timestamp;
        std::unordered_set<Context::TSharedPtr>
            m_acquired_contexts;
    };

    typedef threading::IWorker<IEvent::TSharedPtr> IContextWorker;

    class CMultiplexerThreadHandler: public iridium::threading::IRunnable {
    public:
        DEFINE_IMPLEMENTATION(CMultiplexerThreadHandler)
        CMultiplexerThreadHandler(
            IContextWorker::TSharedPtr  const &context_worker,
            IMultiplexer::TSharedPtr    const &multiplexer,
            ContextManager::TSharedPtr  const &context_manager
        );

        void initialize() override;
        void finalize() override;

        void run(std::atomic<bool> &is_running) override;
    private:
        IMultiplexer::TSharedPtr
            m_multiplexer;
        IContextWorker::TSharedPtr
            m_context_worker;
        ContextManager::TSharedPtr
            m_context_manager;
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
        CContextWorkerHandler(ContextManager::TSharedPtr const &context_manager);
        void initialize() override;
        void finalize() override;
        IContextWorker::TOutputItems handle(IContextWorker::TInputItems const &events) override;
    private:
        ContextManager::TSharedPtr
            m_context_manager;
    };

    IMultiplexer::TSharedPtr
        m_multiplexer;
    ContextManager::TSharedPtr
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
