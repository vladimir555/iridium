// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_PORT_260FEF87_5E53_4DDA_87CF_4744EFC54586
#define HEADER_STREAM_PORT_260FEF87_5E53_4DDA_87CF_4744EFC54586


#include "iridium/platform.h"


/// \~english @file stream_port.h
/// \~english @brief Defines stream port implementations for the macOS platform.
/// \~english The base `CStreamPort` for macOS typedefs to the Unix `CStreamPort` implementation.
/// \~english It also defines a `CStreamPortSSL` for SSL/TLS streams, intended to use SecureTransport.
/// \~russian @file stream_port.h
/// \~russian @brief Определяет реализации потоковых портов для платформы macOS.
/// \~russian Базовый `CStreamPort` для macOS использует typedef для Unix-реализации `CStreamPort`.
/// \~russian Также определяет `CStreamPortSSL` для SSL/TLS потоков, предназначенный для использования SecureTransport.

#ifdef MACOS_PLATFORM


#include "../unix/stream_port.h"


//#include <Security/SecureTransport.h>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


/// \~english @brief Typedef for the base macOS stream port, using the Unix implementation.
/// \~russian @brief Typedef для базового потокового порта macOS, использующий реализацию Unix.
typedef platform::unix_::CStreamPort CStreamPort;


/// \~english @brief SSL/TLS stream port implementation for macOS using SecureTransport.
///     This class extends the Unix stream port to add SSL/TLS capabilities.
///     Note: The implementation details (e.g., `initialize`, `finalize`) are not in this header.
/// \~russian @brief Реализация SSL/TLS потокового порта для macOS с использованием SecureTransport.
///     Этот класс расширяет потоковый порт Unix для добавления возможностей SSL/TLS.
///     Примечание: Детали реализации (например, `initialize`, `finalize`) отсутствуют в этом заголовочном файле.
class CStreamPortSSL: public platform::unix_::CStreamPort {
public:
    DEFINE_IMPLEMENTATION(CStreamPortSSL)
    /// \~english @brief Constructs a `CStreamPortSSL` with a given URI.
    /// \~russian @brief Конструирует `CStreamPortSSL` с заданным URI.
    /// \~english @param uri The URI, which might contain information like hostname for SSL validation.
    /// \~russian @param uri URI, который может содержать информацию, такую как имя хоста для проверки SSL.
    CStreamPortSSL(URI const &uri);
    
    /// \~english @brief Initializes the SSL stream port.
    ///     Establishes the underlying connection via the parent Unix implementation and then performs SSL handshake.
    ///     (Implementation details are expected in a .cpp file).
    /// \~russian @brief Инициализирует SSL потоковый порт.
    ///     Устанавливает базовое соединение через родительскую Unix-реализацию, а затем выполняет SSL-рукопожатие.
    ///     (Детали реализации ожидаются в .cpp файле).
    void initialize() override;

    /// \~english @brief Finalizes the SSL stream port.
    ///     Closes the SSL session and the underlying connection.
    ///     (Implementation details are expected in a .cpp file).
    /// \~russian @brief Завершает работу SSL потокового порта.
    ///     Закрывает SSL-сессию и базовое соединение.
    ///     (Детали реализации ожидаются в .cpp файле).
    void finalize() override;
    
private:
    // \~english @brief Reference to the SSL connection/session (e.g., SecureTransport SSLContextRef).
    // \~english This member is currently commented out in the source.
    // \~russian @brief Ссылка на SSL-соединение/сессию (например, SecureTransport SSLContextRef).
    // \~russian Этот член в настоящее время закомментирован в исходном коде.
//    SSLConnectionRef m_ssl_connection;
};


} // platform
} // implementation
} // io
} // iridium


#endif // MACOS_PLATFORM


#endif // HEADER_STREAM_PORT_260FEF87_5E53_4DDA_87CF_4744EFC54586
