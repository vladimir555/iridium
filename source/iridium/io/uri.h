// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_URI_F2F08356_C16A_4981_92C6_32E1B85D4384
#define HEADER_URI_F2F08356_C16A_4981_92C6_32E1B85D4384


#include "iridium/enum.h"
#include "iridium/smart_ptr.h"

#include <memory>
#include <vector>
#include <array>
#include <unordered_map>
#include <set>

#include "iridium/convertion/convert.h"


namespace iridium::io {


//     [IPC]://[/usr/bin]/[ls]                                  - protocol + path
//    [FILE]://[/usr/bin]/[ls]                                  - protocol + path + host
// [PROCESS]://[/usr/bin]/[ls] [dir1] [dir2]                    - protocol + path + host + arguments
//   [HTTPS]://[hostname.com][/dir1/dir2][?arg1=1&arg2=2]       - protocol + host + path + arguments
//   [HTTPS]://[hostname.com][:80][/dir1/dir2][?arg1=1&arg2=2]  - protocol + host + port + path + arguments
//   [MYSQL]://[user:password]@[hostname.com][:80][/db_name]    - protocol + user + password + host + port + path

// host + port + path   = address
// host + path          = address
// path + host          = address

/// \~english @brief Represents a Uniform Resource Identifier (URI).
/// \~english @details Parses a URI string into its components (protocol, host, port, etc.) and provides access to them.
/// \~russian @brief Представляет универсальный идентификатор ресурса (URI).
/// \~russian @details Разбирает строку URI на компоненты (протокол, хост, порт и т.д.) и предоставляет доступ к ним.
class URI {
public:
    /// \~english @brief Enumeration of supported URI protocols.
    /// \~russian @brief Перечисление поддерживаемых протоколов URI.
    DEFINE_ENUM(
        TProtocol,
        /// \~english @brief Transmission Control Protocol.
        /// \~russian @brief Протокол управления передачей.
        TCP         = -2,
        /// \~english @brief User Datagram Protocol.
        /// \~russian @brief Протокол пользовательских датаграмм.
        UDP         = -3,
        /// \~english @brief WebSocket Protocol.
        /// \~russian @brief Протокол WebSocket.
        WS          = -4,
        /// \~english @brief Secure WebSocket Protocol.
        /// \~russian @brief Безопасный протокол WebSocket.
        WSS         = -5,
        /// \~english @brief File system path.
        /// \~russian @brief Путь в файловой системе.
        FILE        = -6,
        /// \~english @brief Local process.
        /// \~russian @brief Локальный процесс.
        PROCESS     = -7,
        /// \~english @brief Inter-Process Communication (e.g., UNIX domain socket or Windows named pipe).
        /// \~russian @brief Межпроцессное взаимодействие (например, сокет домена UNIX или именованный канал Windows).
        IPC         = -8, // unix:// or windows named pipe
        /// \~english @brief Hypertext Transfer Protocol.
        /// \~russian @brief Протокол передачи гипертекста.
        HTTP        = 80,
        /// \~english @brief Hypertext Transfer Protocol Secure.
        /// \~russian @brief Безопасный протокол передачи гипертекста.
        HTTPS       = 443,
        /// \~english @brief Secure Shell protocol.
        /// \~russian @brief Протокол безопасной оболочки.
        SSH         = 22,
        /// \~english @brief Telnet protocol.
        /// \~russian @brief Протокол Telnet.
        Telnet      = 23,
        /// \~english @brief Real Time Streaming Protocol.
        /// \~russian @brief Протокол потоковой передачи в реальном времени.
        RTSP        = 554,
        /// \~english @brief MySQL database protocol.
        /// \~russian @brief Протокол базы данных MySQL.
        MYSQL       = 3306,
        /// \~english @brief PostgreSQL database protocol.
        /// \~russian @brief Протокол базы данных PostgreSQL.
        POSTGRES    = 5432
    )

    DEFINE_CREATE(URI)

    /// \~english @brief Constructs a URI from a string.
    /// \~russian @brief Создает URI из строки.
    /// \~english @param source The string representation of the URI.
    /// \~russian @param source Строковое представление URI.
    URI(std::string const &source);
   ~URI() = default;

    /// \~english @brief Represents an IPv4 address.
    /// \~russian @brief Представляет IPv4-адрес.
    struct TIPv4: public std::array<uint8_t , 4> {
        TIPv4();
        DEFINE_CREATE(TIPv4)
    };
    /// \~english @brief Represents an IPv6 address.
    /// \~russian @brief Представляет IPv6-адрес.
    struct TIPv6: public std::array<uint16_t, 8> {
        DEFINE_CREATE(TIPv6)
    };

    /// \~english @brief Type definition for a network port.
    /// \~russian @brief Определение типа для сетевого порта.
    typedef uint16_t
                TPort;
    /// \~english @brief Type definition for a map of name-value pairs, typically for URI arguments.
    /// \~russian @brief Определение типа для карты пар "имя-значение", обычно для аргументов URI.
    typedef std::unordered_map<std::string, std::string>
                TMapNameValue;

    /// \~english @brief Gets the IPv4 address of the host.
    /// \~russian @brief Возвращает IPv4-адрес хоста.
    /// \~english @return A shared pointer to the IPv4 address, or nullptr if not available.
    /// \~russian @return Умный указатель на IPv4-адрес, или nullptr, если он недоступен.
    TIPv4::TConstSharedPtr
                getIPv4()       const;
    /// \~english @brief Gets the IPv6 address of the host.
    /// \~russian @brief Возвращает IPv6-адрес хоста.
    /// \~english @return A shared pointer to the IPv6 address, or nullptr if not available.
    /// \~russian @return Умный указатель на IPv6-адрес, или nullptr, если он недоступен.
    TIPv6::TConstSharedPtr
                getIPv6()       const;

    /// \~english @brief Gets the protocol.
    /// \~russian @brief Возвращает протокол.
    TProtocol   getProtocol()   const;
    /// \~english @brief Gets the user name.
    /// \~russian @brief Возвращает имя пользователя.
    std::string getUser()       const;
    /// \~english @brief Gets the password.
    /// \~russian @brief Возвращает пароль.
    std::string getPassword()   const;
    /// \~english @brief Gets the host name.
    /// \~russian @brief Возвращает имя хоста.
    std::string getHost()       const;
    /// \~english @brief Gets the port number.
    /// \~russian @brief Возвращает номер порта.
    TPort       getPort()       const;
    /// \~english @brief Gets the path component.
    /// \~russian @brief Возвращает компонент пути.
    std::string getPath()       const;
    // todo: map
    /// \~english @brief Gets the arguments as a single string.
    /// \~russian @brief Возвращает аргументы в виде одной строки.
    std::string getArguments()  const;
    /// \~english @brief Gets the address portion (e.g., "host:port/path").
    /// \~russian @brief Возвращает адресную часть (например, "host:port/path").
    std::string getAddress()    const;
    /// \~english @brief Gets the original source string.
    /// \~russian @brief Возвращает исходную строку.
    std::string getSource()     const;
    // todo: url arguments

    /// \~english @brief Compares two URI objects for equality.
    /// \~russian @brief Сравнивает два объекта URI на равенство.
    bool operator == (URI const &uri) const;
    /// \~english @brief Compares two URI objects for ordering.
    /// \~russian @brief Сравнивает два объекта URI для упорядочивания.
    bool operator <  (URI const &uri) const;

private:
    std::string     m_source;
    TProtocol       m_protocol;
    std::string     m_user;
    std::string     m_password;
    std::string     m_host;
    std::string     m_path;
    std::string     m_address;
    std::string     m_arguments;
    std::shared_ptr<TIPv4> mutable m_ipv4;
    std::shared_ptr<TIPv6> mutable m_ipv6;
    TPort           m_port;
};


} // namespace iridium::io


//DEFINE_ENUM_CONVERT(iridium::io::URI::TProtocol)

DEFINE_CONVERT(std::string, iridium::io::URI::TIPv4)
DEFINE_CONVERT(iridium::io::URI::TIPv4, std::string)

DEFINE_CONVERT(std::string, iridium::io::URI)
DEFINE_CONVERT(iridium::io::URI, std::string)


#endif // HEADER_URI_F2F08356_C16A_4981_92C6_32E1B85D4384
