#include <iridium/testing/tester.h>

#include <iridium/io/implementation/connection_manager.h>
#include <iridium/items.h>


using iridium::io::implementation::CConnectionManager;
using iridium::io::IAcceptor;
using iridium::io::IProtocol;
using iridium::io::IContext;
using iridium::io::TStream;
using iridium::io::URI;
using iridium::io::TEvent;
using iridium::threading::sleep;
using iridium::checkOneOf;


class CTestPeerEchoProtocol: public IProtocol {
public:
    DEFINE_IMPLEMENTATION(CTestPeerEchoProtocol);

    bool control(TEvent::TSharedPtr const &event) override {
        static std::string const DEFAULT_PIPE_NAME = "default";
        LOGT << "event: " << event;

        if (event->operation == TEvent::TOperation::OPEN) {
            event->context->setPipe(DEFAULT_PIPE_NAME, event->uri, nullptr);
            return true;
        }

        if (event->operation == TEvent::TOperation::READ) {
            LOGT <<
                event->operation << ": " <<
                event->context->getBuffers(event->uri, IContext::TStreamType::READER) << " -> " <<
                event->context->getBuffers(event->uri, IContext::TStreamType::WRITER);

            event->context->swapPipe(event->uri);
            return true;
        }

        if (event->operation == TEvent::TOperation::WRITE &&
            event->context->getBuffers(event->uri, IContext::TStreamType::WRITER).empty())
        {
            return false;
        }

        if (event->operation == TEvent::TOperation::CLOSE)
            return false;

        return true;
    }
};


class CTestAcceptor: public IAcceptor {
public:
    DEFINE_IMPLEMENTATION(CTestAcceptor);

    IProtocol::TSharedPtr accept(URI::TSharedPtr const &uri) override {
        LOGT << "accept: " << uri;
        auto protocol = CTestPeerEchoProtocol::create();
        return protocol;
    }
};


TEST(echo) {
    auto connection_manager = CConnectionManager::create();
    connection_manager->initialize();
    connection_manager->manage(URI::create("tcp://127.0.0.1:55550"), CTestAcceptor::create());
    sleep(5000);
    connection_manager->finalize();
}


class CTestProcessProtocol: public IProtocol {
public:
    DEFINE_IMPLEMENTATION(CTestProcessProtocol);

    bool control(TEvent::TSharedPtr const &event) override {
        static std::string const DEFAULT_PIPE_NAME = "default";
        LOGT << "event: " << event;

        if (event->operation == TEvent::TOperation::OPEN) {
            event->context->setPipe(DEFAULT_PIPE_NAME, event->uri, nullptr);
            return true;
        }

        if (event->operation == TEvent::TOperation::READ) {
            LOGT <<
                event->operation << ": " <<
                event->context->getBuffers(event->uri, IContext::TStreamType::READER) << " -> " <<
                event->context->getBuffers(event->uri, IContext::TStreamType::WRITER);
        }

        return false;
    }
};


TEST(process) {
    auto connection_manager = CConnectionManager::create();
    connection_manager->initialize();
    connection_manager->manage(URI::create("process:///bin/ls"), CTestProcessProtocol::create());
    sleep(500);
    connection_manager->finalize();
}
