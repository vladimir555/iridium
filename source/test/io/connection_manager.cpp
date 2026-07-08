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


class CTestProtocol: public IProtocol {
public:
    DEFINE_IMPLEMENTATION(CTestProtocol);
    CTestProtocol() = default;

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
    CTestAcceptor() = default;

    IProtocol::TSharedPtr accept(URI::TSharedPtr const &uri) override {
        LOGT << "accept: " << uri;
        auto protocol = CTestProtocol::create();
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
