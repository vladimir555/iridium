// Copyright © 2019-2026 Vladimir Bulaev.
// Contacts: <bulaev_vladimir@mail.ru>
// License: LGPL-3.0 (https://www.gnu.org/licenses/lgpl-3.0)

#include <iridium/testing/tester.h>

#include <iridium/io/connection_manager.h>
#include <iridium/io/implementation/connection_manager.h>
#include <iridium/items.h>


using iridium::io::implementation::CConnectionManager;
using iridium::io::IAcceptor;
using iridium::io::IProtocol;
using iridium::io::IContext;
using iridium::io::TStream;
using iridium::io::URI;
using iridium::io::TEvent;
using iridium::io::checkSuffix;
using iridium::threading::sleep;
using iridium::checkOneOf;


class CTestPeerEchoProtocol: public IProtocol {
public:
    DEFINE_IMPLEMENTATION(CTestPeerEchoProtocol);

    bool control(TEvent::TSharedPtr const &event) override {
        static std::string const DEFAULT_PIPE_NAME = "default";

        if (event->operation == TEvent::TOperation::OPEN) {
            event->context->setPipe(DEFAULT_PIPE_NAME, event->uri, nullptr);
            return true;
        }

        if (event->operation == TEvent::TOperation::READ) {
            LOGT <<
                event->operation << ": " <<
                event->context->getBuffers(event->uri, IContext::TStreamType::READER);

            if (checkSuffix(event->context->getBuffers(event->uri, IContext::TStreamType::READER), "\n\n")) {
                event->context->swapPipe(event->uri);
                LOGT << "SWAP, " <<
                    event->operation << ": " <<
                    event->context->getBuffers(event->uri, IContext::TStreamType::READER) << " -> " <<
                    event->context->getBuffers(event->uri, IContext::TStreamType::WRITER);
            }
        }

        if (event->operation == TEvent::TOperation::WRITE) {
            LOGT <<
                event->operation << ": " <<
                event->context->getBuffers(event->uri, IContext::TStreamType::WRITER);
        }

        if (event->operation == TEvent::TOperation::READ_END) {
            LOGT <<
                event->operation << ": " <<
                event->context->getBuffers(event->uri, IContext::TStreamType::READER) << " -> " <<
                event->context->getBuffers(event->uri, IContext::TStreamType::WRITER);

            event->context->swapPipe(event->uri);
            LOGT << "SWAP";
            return true;
        }

        if (event->operation == TEvent::TOperation::WRITE_END) {
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

        if (event->operation == TEvent::TOperation::CLOSE)
            return false;

        return true;
    }
};


TEST(process) {
    auto connection_manager = CConnectionManager::create();
    connection_manager->initialize();
    connection_manager->manage(URI::create("process:///bin/ls"), CTestProcessProtocol::create());
    sleep(500);
    connection_manager->finalize();
}
