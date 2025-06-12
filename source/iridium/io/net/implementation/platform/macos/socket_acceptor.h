// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_ACCEPTOR_4CB6AFEA_83AE_479D_BC6E_251D62C80C29
#define HEADER_SOCKET_ACCEPTOR_4CB6AFEA_83AE_479D_BC6E_251D62C80C29


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


/// \~english @file socket_acceptor.h
/// \~english @brief Defines the socket acceptor implementation for the macOS platform.
/// \~english This acceptor listens for incoming network connections and creates new socket streams for them.
/// \~russian @file socket_acceptor.h
/// \~russian @brief Определяет реализацию принимающего сокета для платформы macOS.
/// \~russian Этот принимающий сокет прослушивает входящие сетевые подключения и создает для них новые сокетные потоки.

#include "iridium/io/net/socket.h"
#include "../unix/socket.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


//typedef unix::CSocketAcceptor CSocketAcceptor;
/// \~english @brief macOS-specific socket acceptor.
///     Derives from the common Unix `CSocketBase` and implements the `ISocketAcceptor` interface.
///     It is responsible for listening on a network address specified by a URI and accepting incoming client connections.
/// \~russian @brief Принимающий сокет для macOS.
///     Наследуется от общего Unix `CSocketBase` и реализует интерфейс `ISocketAcceptor`.
///     Отвечает за прослушивание сетевого адреса, указанного в URI, и прием входящих клиентских подключений.
class CSocketAcceptor:
    public unix_::CSocketBase,
    public ISocketAcceptor,
    public std::enable_shared_from_this<CSocketAcceptor>
{
public:
    DEFINE_IMPLEMENTATION(CSocketAcceptor)
    /// \~english @brief Constructs a `CSocketAcceptor`.
    /// \~russian @brief Конструирует `CSocketAcceptor`.
    /// \~english @param uri The URI specifying the local address and port to listen on.
    /// \~russian @param uri URI, указывающий локальный адрес и порт для прослушивания.
    CSocketAcceptor(URI const &uri);

    /// \~english @brief Initializes the acceptor socket.
    ///     This involves opening the socket, binding it to the address in the URI, and putting it into listening mode.
    /// \~russian @brief Инициализирует принимающий сокет.
    ///     Это включает открытие сокета, его привязку к адресу из URI и перевод в режим прослушивания.
    /// \~english @throws std::runtime_error on errors during socket open, bind, or listen.
    /// \~russian @throws std::runtime_error при ошибках во время открытия, привязки или прослушивания сокета.
    void initialize() override;

    /// \~english @brief Finalizes the acceptor socket.
    ///     Closes the listening socket.
    /// \~russian @brief Завершает работу принимающего сокета.
    ///     Закрывает слушающий сокет.
    void finalize() override;

    /// \~english @brief Gets the URI this acceptor is listening on.
    /// \~russian @brief Получает URI, на котором этот принимающий сокет прослушивает.
    /// \~english @return A shared pointer to the URI.
    /// \~russian @return Умный указатель на URI.
    URI::TSharedPtr getURI() const override;

    // \~english @brief Gets the underlying file descriptor of the listening socket (Commented out in source).
    // \~russian @brief Получает базовый файловый дескриптор слушающего сокета (Закомментировано в исходном коде).
//    int getID() const override;

    /// \~english @brief Accepts an incoming client connection.
    ///     This is a blocking call that waits for a new connection.
    /// \~russian @brief Принимает входящее клиентское подключение.
    ///     Это блокирующий вызов, который ожидает нового подключения.
    /// \~english @return A shared pointer to an `ISocketStream` for the new connection.
    /// \~russian @return Умный указатель на `ISocketStream` для нового подключения.
    /// \~english @throws std::runtime_error if `CSocketBase::accept()` fails or if the creation of the peer socket object fails.
    /// \~russian @throws std::runtime_error, если `CSocketBase::accept()` не удался или если не удалось создать объект сокета для равноправного узла.
    ISocketStream::TSharedPtr accept() override;

private:
    // \~english @brief Context for SSL/TLS encryption (commented out in source).
    // \~english If uncommented, this would likely hold state for an SSL_CTX or similar for SSL-enabled server sockets.
    // \~russian @brief Контекст для SSL/TLS шифрования (закомментировано в исходном коде).
    // \~russian Если раскомментировано, здесь, вероятно, будет храниться состояние для SSL_CTX или аналога для серверных сокетов с поддержкой SSL.
//    encryption::OpenSSL::TContext *m_context;
};


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // MACOS_PLATFORM


#endif // HEADER_SOCKET_ACCEPTOR_4CB6AFEA_83AE_479D_BC6E_251D62C80C29
