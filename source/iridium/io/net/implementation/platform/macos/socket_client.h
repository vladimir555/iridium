// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_CLIENT_7D1C4066_85ED_442D_A3CF_2D32852D4E26
#define HEADER_SOCKET_CLIENT_7D1C4066_85ED_442D_A3CF_2D32852D4E26


#include "iridium/platform.h"


#ifdef MACOS_PLATFORM


/// \~english @file socket_client.h
/// \~english @brief Defines the client socket implementation for the macOS platform.
/// \~english This client socket is used to establish and manage outgoing network connections.
/// \~russian @file socket_client.h
/// \~russian @brief Определяет реализацию клиентского сокета для платформы macOS.
/// \~russian Этот клиентский сокет используется для установления и управления исходящими сетевыми подключениями.

//#include "../unix/socket_client.h" // Original include, now likely superseded by CSocketBase via "socket.h"

#include "iridium/io/net/socket.h"

#include "socket.h" // This should provide the CSocket type, which is a typedef to unix_::CSocketBase



namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


//typedef unix::CSocketClient CSocketClient; // Original typedef, now a direct class definition


/// \~english @brief macOS-specific client socket.
///     Derives from `unix_::CSocketBase` (via `CSocket` typedef in "socket.h") and implements `ISocketStream`.
///     It is responsible for establishing a connection to a server specified by a URI and handling read/write operations.
///     SSL/TLS features appear to be commented out in the current version.
/// \~russian @brief Клиентский сокет для macOS.
///     Наследуется от `unix_::CSocketBase` (через typedef `CSocket` в "socket.h") и реализует `ISocketStream`.
///     Отвечает за установление соединения с сервером, указанным в URI, и обработку операций чтения/записи.
///     Функции SSL/TLS, по-видимому, закомментированы в текущей версии.
class CSocketClient:
    virtual public unix_::CSocketBase, // Should be CSocket, which is unix_::CSocketBase
    public ISocketStream,
    public std::enable_shared_from_this<CSocketClient>
{
public:
    DEFINE_IMPLEMENTATION(CSocketClient)
    /// \~english @brief Constructs a `CSocketClient`.
    /// \~russian @brief Конструирует `CSocketClient`.
    /// \~english @param uri The URI of the server to connect to.
    /// \~russian @param uri URI сервера для подключения.
    CSocketClient(URI const &uri);

    /// \~english @brief Initializes the client socket.
    ///     Opens the socket and attempts to connect to the server specified in the URI.
    /// \~russian @brief Инициализирует клиентский сокет.
    ///     Открывает сокет и пытается подключиться к серверу, указанному в URI.
    /// \~english @throws std::runtime_error on errors during socket open or connect.
    /// \~russian @throws std::runtime_error при ошибках во время открытия или подключения сокета.
    void initialize() override;

    /// \~english @brief Finalizes the client socket.
    ///     Closes the connection and the socket.
    /// \~russian @brief Завершает работу клиентского сокета.
    ///     Закрывает соединение и сокет.
    void finalize() override;

    /// \~english @brief Gets the URI this client is connected or connecting to.
    /// \~russian @brief Получает URI, к которому этот клиент подключен или подключается.
    /// \~english @return A shared pointer to the URI.
    /// \~russian @return Умный указатель на URI.
    URI::TSharedPtr getURI() const override;

    // \~english @brief Gets the underlying file descriptor of the socket (Commented out in source).
    // \~russian @brief Получает базовый файловый дескриптор сокета (Закомментировано в исходном коде).
//    int getID() const override;

    /// \~english @brief Reads data from the socket stream.
    /// \~russian @brief Читает данные из сокетного потока.
    /// \~english @param size The maximum number of bytes to read.
    /// \~russian @param size Максимальное количество байт для чтения.
    /// \~english @return A buffer containing the data read. Returns `nullptr` on EOF or error.
    /// \~russian @return Буфер, содержащий прочитанные данные. Возвращает `nullptr` при EOF или ошибке.
    Buffer::TSharedPtr read(size_t const &size) override;

    /// \~english @brief Writes data to the socket stream.
    /// \~russian @brief Записывает данные в сокетный поток.
    /// \~english @param buffer The buffer containing data to write.
    /// \~russian @param buffer Буфер, содержащий данные для записи.
    /// \~english @return The number of bytes actually written.
    /// \~russian @return Количество фактически записанных байт.
    size_t write(Buffer::TSharedPtr const &buffer) override;

private:
    // \~english @brief Asserts SSL is initialized (Commented out in source).
    // \~english Used for SSL/TLS connections to ensure handshake is complete before I/O.
    // \~russian @brief Проверяет, инициализирован ли SSL (Закомментировано в исходном коде).
    // \~russian Используется для SSL/TLS соединений, чтобы убедиться, что рукопожатие завершено перед вводом-выводом.
//    bool assertSSLInitialized();

    // \~english @brief OpenSSL context (Commented out in source).
    // \~russian @brief Контекст OpenSSL (Закомментировано в исходном коде).
//    encryption::OpenSSL::TContext  *m_context;

    // \~english @brief OpenSSL SSL structure (Commented out in source).
    // \~russian @brief Структура SSL OpenSSL (Закомментировано в исходном коде).
//    encryption::OpenSSL::TSSL      *m_ssl;

    // \~english @brief Flag indicating if SSL handshake is done (Commented out in source).
    // \~russian @brief Флаг, указывающий, выполнено ли SSL-рукопожатие (Закомментировано в исходном коде).
//    bool                            m_is_ssl_handshake_done;
};



} // platform
} // implementation
} // net
} // io
} // iridium


#endif // MACOS_PLATFORM


#endif // HEADER_SOCKET_CLIENT_7D1C4066_85ED_442D_A3CF_2D32852D4E26
