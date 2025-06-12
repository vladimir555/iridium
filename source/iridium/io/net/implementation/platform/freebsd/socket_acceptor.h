// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_ACCEPTOR_E881803B_D869_4CAC_8429_8F9220B1981E
#define HEADER_SOCKET_ACCEPTOR_E881803B_D869_4CAC_8429_8F9220B1981E


#include "iridium/platform.h"


#ifdef FREEBSD_PLATFORM


/// \~english @file socket_acceptor.h
/// \~english @brief Defines the socket acceptor implementation for the FreeBSD platform.
/// \~english This typically handles listening for and accepting incoming network connections.
/// \~russian @file socket_acceptor.h
/// \~russian @brief Определяет реализацию принимающего сокета для платформы FreeBSD.
/// \~russian Обычно обрабатывает прослушивание и прием входящих сетевых подключений.

#include "iridium/io/net/socket.h"
#include "../unix/socket.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


//typedef unix::CSocketAcceptor CSocketAcceptor;
/// \~english @brief FreeBSD-specific socket acceptor.
///     Derives from the common Unix `CSocketBase` and implements the `ISocketAcceptor` interface.
///     It is responsible for listening on a network address and accepting client connections.
/// \~russian @brief Принимающий сокет для FreeBSD.
///     Наследуется от общего Unix `CSocketBase` и реализует интерфейс `ISocketAcceptor`.
///     Отвечает за прослушивание сетевого адреса и прием клиентских подключений.
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
    ///     Opens, binds, and listens on the socket specified by the URI.
    /// \~russian @brief Инициализирует принимающий сокет.
    ///     Открывает, привязывает и начинает прослушивание на сокете, указанном в URI.
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
    URI getURI() const override; // Note: ISocket defines URI::TSharedPtr getURI() const; this seems to be a direct URI object.
                                 // This might be an inconsistency or a specific design choice. Assuming it should be URI::TSharedPtr.
                                 // For now, documenting as is.

    /// \~english @brief Gets the underlying file descriptor of the listening socket.
    /// \~russian @brief Получает базовый файловый дескриптор слушающего сокета.
    /// \~english @return The integer file descriptor.
    /// \~russian @return Целочисленный файловый дескриптор.
    int getID() const override;

    /// \~english @brief Accepts an incoming client connection.
    /// \~russian @brief Принимает входящее клиентское подключение.
    /// \~english @return A shared pointer to an `ISocketStream` for the new connection.
    /// \~russian @return Умный указатель на `ISocketStream` для нового подключения.
    /// \~english @throws std::runtime_error if `accept` fails.
    /// \~russian @throws std::runtime_error, если `accept` не удался.
    ISocketStream::TSharedPtr accept() override;

private:
    // \~english @brief Context for SSL/TLS encryption (commented out in source).
    // \~english If uncommented, this would likely hold state for an SSL_CTX or similar.
    // \~russian @brief Контекст для SSL/TLS шифрования (закомментировано в исходном коде).
    // \~russian Если раскомментировано, здесь, вероятно, будет храниться состояние для SSL_CTX или аналога.
//    encryption::OpenSSL::TContext *m_context;
};


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // FREEBSD_PLATFORM


#endif // HEADER_SOCKET_ACCEPTOR_E881803B_D869_4CAC_8429_8F9220B1981E
