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


namespace iridium {
namespace io {


//     [IPC]://[/usr/bin]/[ls]                                  - protocol + path
//    [FILE]://[/usr/bin]/[ls]                                  - protocol + path + host
// [PROCESS]://[/usr/bin]/[ls] [dir1] [dir2]                    - protocol + path + host + arguments
//   [HTTPS]://[hostname.com][/dir1/dir2][?arg1=1&arg2=2]       - protocol + host + path + arguments
//   [HTTPS]://[hostname.com][:80][/dir1/dir2][?arg1=1&arg2=2]  - protocol + host + port + path + arguments
//   [MYSQL]://[user:password]@[hostname.com][:80][/db_name]    - protocol + user + password + host + port + path

// host + port + path   = address
// host + path          = address
// path + host          = address

/// \~english @brief A class for parsing and manipulating Uniform Resource Identifiers (URIs).
/// \~english @details This class provides functionality to parse a URI string into its components
/// \~english (protocol, user, password, host, port, path, and arguments) and access them.
/// \~english It supports various protocols, including network protocols like TCP, UDP, HTTP(S), WS(S),
/// \~english and database protocols like MYSQL, POSTGRES, as well as local resource identifiers
/// \~english like FILE, PROCESS, and IPC.
/// \~english Example URI formats:
/// \~english - `[IPC]://[/usr/bin]/[ls]`
/// \~english - `[FILE]://[/usr/bin]/[ls]`
/// \~english - `[PROCESS]://[/usr/bin]/[ls] [dir1] [dir2]`
/// \~english - `[HTTPS]://[hostname.com][/dir1/dir2][?arg1=1&arg2=2]`
/// \~english - `[MYSQL]://[user:password]@[hostname.com][:3306][/db_name]`
/// \~russian @brief Класс для парсинга и манипулирования универсальными идентификаторами ресурсов (URI).
/// \~russian @details Этот класс предоставляет функциональность для разбора строки URI на ее компоненты
/// \~russian (протокол, пользователь, пароль, хост, порт, путь и аргументы) и доступа к ним.
/// \~russian Он поддерживает различные протоколы, включая сетевые протоколы, такие как TCP, UDP, HTTP(S), WS(S),
/// \~russian и протоколы баз данных, такие как MYSQL, POSTGRES, а также идентификаторы локальных ресурсов,
/// \~russian такие как FILE, PROCESS и IPC.
/// \~russian Примеры форматов URI:
/// \~russian - `[IPC]://[/usr/bin]/[ls]`
/// \~russian - `[FILE]://[/usr/bin]/[ls]`
/// \~russian - `[PROCESS]://[/usr/bin]/[ls] [dir1] [dir2]`
/// \~russian - `[HTTPS]://[hostname.com][/dir1/dir2][?arg1=1&arg2=2]`
/// \~russian - `[MYSQL]://[user:password]@[hostname.com][:3306][/db_name]`
class URI {
public:
    /// \~english @brief Enumeration of supported URI protocols.
    /// \~english The values are either conventional port numbers or negative values for protocols without a standard port.
    /// \~russian @brief Перечисление поддерживаемых протоколов URI.
    /// \~russian Значения представляют собой либо общепринятые номера портов, либо отрицательные значения для протоколов без стандартного порта.
    DEFINE_ENUM(
        TProtocol, 
        TCP         = -2,   ///< \~english Transmission Control Protocol \~russian TCP
        UDP         = -3,   ///< \~english User Datagram Protocol \~russian UDP
        WS          = -4,   ///< \~english WebSocket \~russian WebSocket
        WSS         = -5,   ///< \~english WebSocket Secure \~russian WebSocket Secure
        FILE        = -6,   ///< \~english Local file path \~russian Локальный файловый путь
        PROCESS     = -7,   ///< \~english Local process \~russian Локальный процесс
        IPC         = -8,   ///< \~english Inter-process communication (e.g., unix:// or Windows named pipe) \~russian Межпроцессное взаимодействие (например, unix:// или именованный канал Windows)
        HTTP        = 80,   ///< \~english Hypertext Transfer Protocol \~russian HTTP
        HTTPS       = 443,  ///< \~english Hypertext Transfer Protocol Secure \~russian HTTPS
        SSH         = 22,   ///< \~english Secure Shell \~russian SSH
        Telnet      = 23,   ///< \~english Telnet Protocol \~russian Telnet
        RTSP        = 554,  ///< \~english Real Time Streaming Protocol \~russian RTSP
        MYSQL       = 3306, ///< \~english MySQL Protocol \~russian MySQL
        POSTGRES    = 5432  ///< \~english PostgreSQL Protocol \~russian PostgreSQL
    )

    DEFINE_CREATE(URI)

    /// \~english @brief Constructs a URI object from a source string.
    /// \~russian @brief Создает объект URI из исходной строки.
    /// \~english @param source The URI string to parse.
    /// \~russian @param source Строка URI для разбора.
    URI(std::string const &source);
    ~URI() = default;

    /// \~english @brief Represents an IPv4 address as an array of 4 bytes.
    /// \~russian @brief Представляет IPv4-адрес в виде массива из 4 байт.
    struct TIPv4: public std::array<uint8_t , 4> {
        TIPv4();
        DEFINE_CREATE(TIPv4)
    };

    /// \~english @brief Represents an IPv6 address as an array of 8 16-bit words.
    /// \~russian @brief Представляет IPv6-адрес в виде массива из 8 16-битных слов.
    struct TIPv6: public std::array<uint16_t, 8> {
        DEFINE_CREATE(TIPv6)
    };
    
    /// \~english @brief Type definition for a port number.
    /// \~russian @brief Определение типа для номера порта.
    typedef uint16_t TPort;

    /// \~english @brief Type definition for a map of name-value pairs, typically for URI arguments.
    /// \~russian @brief Определение типа для карты пар "имя-значение", обычно для аргументов URI.
    typedef std::unordered_map<std::string, std::string> TMapNameValue;

    /// \~english @brief Gets the IPv4 address if the host is a valid IPv4.
    /// \~english @return A shared pointer to the TIPv4 object, or nullptr if not a valid IPv4.
    /// \~russian @brief Возвращает IPv4-адрес, если хост является действительным IPv4.
    /// \~russian @return Умный указатель на объект TIPv4 или nullptr, если это не действительный IPv4.
    TIPv4::TConstSharedPtr getIPv4() const;

    /// \~english @brief Gets the IPv6 address if the host is a valid IPv6.
    /// \~english @return A shared pointer to the TIPv6 object, or nullptr if not a valid IPv6.
    /// \~russian @brief Возвращает IPv6-адрес, если хост является действительным IPv6.
    /// \~russian @return Умный указатель на объект TIPv6 или nullptr, если это не действительный IPv6.
    TIPv6::TConstSharedPtr getIPv6() const;

    /// \~english @brief Gets the protocol of the URI.
    /// \~russian @brief Возвращает протокол URI.
    TProtocol   getProtocol()   const;
    /// \~english @brief Gets the user component of the URI.
    /// \~russian @brief Возвращает компонент пользователя URI.
    std::string getUser()       const;
    /// \~english @brief Gets the password component of the URI.
    /// \~russian @brief Возвращает компонент пароля URI.
    std::string getPassword()   const;
    /// \~english @brief Gets the host component of the URI.
    /// \~russian @brief Возвращает компонент хоста URI.
    std::string getHost()       const;
    /// \~english @brief Gets the port number of the URI.
    /// \~russian @brief Возвращает номер порта URI.
    TPort       getPort()       const;
    /// \~english @brief Gets the path component of the URI.
    /// \~russian @brief Возвращает компонент пути URI.
    std::string getPath()       const;
    /// \~english @brief Gets the arguments/query string component of the URI.
    /// \~russian @brief Возвращает компонент аргументов/строки запроса URI.
    // todo: map
    std::string getArguments()  const;
    /// \~english @brief Gets the address part of the URI (e.g., host:port/path).
    /// \~russian @brief Возвращает адресную часть URI (например, хост:порт/путь).
    std::string getAddress()    const;
    /// \~english @brief Gets the original source string of the URI.
    /// \~russian @brief Возвращает исходную строку URI.
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


} // io
} // iridium


//DEFINE_ENUM_CONVERT(iridium::io::URI::TProtocol)

DEFINE_CONVERT(std::string, iridium::io::URI::TIPv4)
DEFINE_CONVERT(iridium::io::URI::TIPv4, std::string)

DEFINE_CONVERT(std::string, iridium::io::URI)
DEFINE_CONVERT(iridium::io::URI, std::string)


#endif // HEADER_URI_F2F08356_C16A_4981_92C6_32E1B85D4384
