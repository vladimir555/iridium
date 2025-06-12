// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CLIENT_B9ECC183_EEA1_45FA_80E2_FA88C07AE6BC
#define HEADER_CLIENT_B9ECC183_EEA1_45FA_80E2_FA88C07AE6BC


#include "iridium/io/client.h"
#include "iridium/io/uri.h"
#include "iridium/io/session_manager.h"


namespace iridium {
namespace io {
namespace implementation {


/// \~english @brief Concrete implementation of the `IClient` interface.
///     This class manages a client connection to a server specified by a URI,
///     using a given protocol for communication and a session manager to handle the connection lifecycle.
/// \~russian @brief Конкретная реализация интерфейса `IClient`.
///     Этот класс управляет клиентским подключением к серверу, указанному URI,
///     используя заданный протокол для обмена данными и менеджер сессий для управления жизненным циклом подключения.
class CClient: public IClient {
public:
    DEFINE_IMPLEMENTATION(CClient)
    /// \~english @brief Constructs a `CClient` instance.
    ///     Sets up the client with the target URI and the communication protocol.
    ///     The actual connection and session management are typically initiated in the `initialize()` method.
    /// \~russian @brief Конструирует экземпляр `CClient`.
    ///     Настраивает клиент с целевым URI и протоколом обмена данными.
    ///     Фактическое подключение и управление сессией обычно инициируются в методе `initialize()`.
    /// \~english @param uri The URI of the server to connect to.
    /// \~russian @param uri URI сервера для подключения.
    /// \~english @param protocol The protocol instance to be used for communication over the established stream.
    /// \~russian @param protocol Экземпляр протокола, который будет использоваться для обмена данными через установленный поток.
    CClient(URI const &uri, IProtocol::TSharedPtr const &protocol);
    
    /// \~english @brief Initializes the client connection.
    ///     This typically involves creating a stream (e.g., TCP socket) based on the URI,
    ///     then creating and initializing a session manager to handle the stream and protocol.
    /// \~russian @brief Инициализирует клиентское подключение.
    ///     Обычно это включает создание потока (например, TCP-сокета) на основе URI,
    ///     а затем создание и инициализацию менеджера сессий для обработки потока и протокола.
    /// \~english @throws iridium::Exception or derived classes on errors (e.g., connection failure, session manager initialization failure).
    /// \~russian @throws iridium::Exception или производные классы при ошибках (например, сбой подключения, сбой инициализации менеджера сессий).
    void initialize()   override;

    /// \~english @brief Finalizes the client connection.
    ///     This typically involves finalizing and destroying the session manager, which in turn
    ///     should close the stream and clean up resources.
    /// \~russian @brief Завершает клиентское подключение.
    ///     Обычно это включает завершение работы и уничтожение менеджера сессий, который, в свою очередь,
    ///     должен закрыть поток и очистить ресурсы.
    /// \~english @throws iridium::Exception or derived classes on errors during finalization.
    /// \~russian @throws iridium::Exception или производные классы при ошибках во время завершения работы.
    void finalize()     override;

private:
    /// \~english @brief The stream port representing the communication channel (e.g., socket connection).
    ///     This is established during `initialize()`.
    /// \~russian @brief Порт потока, представляющий канал связи (например, сокетное соединение).
    ///     Устанавливается во время `initialize()`.
    IStreamPort::TSharedPtr     m_stream;

    /// \~english @brief The protocol used for communication over the `m_stream`.
    /// \~russian @brief Протокол, используемый для обмена данными через `m_stream`.
    IProtocol::TSharedPtr       m_protocol;

    /// \~english @brief The session manager responsible for handling the lifecycle of the client's communication session.
    /// \~russian @brief Менеджер сессий, отвечающий за управление жизненным циклом сеанса связи клиента.
    ISessionManager::TSharedPtr m_session_manager;
};


} // implementation
} // io
} // iridium


#endif // HEADER_CLIENT_B9ECC183_EEA1_45FA_80E2_FA88C07AE6BC
