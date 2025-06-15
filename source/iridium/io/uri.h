// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines a class for parsing and representing Uniform Resource Identifiers (URIs).
/// \~russian @brief Определяет класс для разбора и представления универсальных идентификаторов ресурсов (URI).
#ifndef HEADER_URI_F2F08356_C16A_4981_92C6_32E1B85D4384
#define HEADER_URI_F2F08356_C16A_4981_92C6_32E1B85D4384


#include "iridium/enum.h"               // For DEFINE_ENUM
#include "iridium/smart_ptr.h"          // For DEFINE_CREATE, TSharedPtr, TConstSharedPtr
#include "iridium/convertion/convert.h" // For DEFINE_CONVERT

#include <memory>
#include <vector>
#include <array>
#include <unordered_map>
#include <set>


// Examples of URI formats understood by this parser:
//     [IPC]://[/usr/bin]/[ls]                                  - protocol + path
//    [FILE]://[/usr/bin]/[ls]                                  - protocol + path + host
// [PROCESS]://[/usr/bin]/[ls] [dir1] [dir2]                    - protocol + path + host + arguments
//   [HTTPS]://[hostname.com][/dir1/dir2][?arg1=1&arg2=2]       - protocol + host + path + arguments
//   [HTTPS]://[hostname.com][:80][/dir1/dir2][?arg1=1&arg2=2]  - protocol + host + port + path + arguments
//   [MYSQL]://[user:password]@[hostname.com][:80][/db_name]    - protocol + user + password + host + port + path

// Note on address components:
// host + port + path   = address
// host + path          = address
// path + host          = address (less common, depends on scheme)

namespace iridium {
namespace io {


/// \~english @brief Represents and facilitates parsing of Uniform Resource Identifiers (URIs).
///     This class can parse a URI string into its constituent components such as scheme (protocol),
///     user information, host, port, path, and query arguments. It provides methods to access these components.
/// \~russian @brief Представляет и облегчает разбор универсальных идентификаторов ресурсов (URI).
///     Этот класс может разбирать строку URI на ее составные компоненты, такие как схема (протокол),
///     информация о пользователе, хост, порт, путь и аргументы запроса. Он предоставляет методы для доступа к этим компонентам.
class URI {
public:
    /// \~english @brief Enumeration of known URI schemes/protocols and their typical default ports or identifiers.
    ///     Negative values are used for schemes without standard port numbers or for abstract schemes.
    /// \~russian @brief Перечисление известных схем/протоколов URI и их типичных портов по умолчанию или идентификаторов.
    ///     Отрицательные значения используются для схем без стандартных номеров портов или для абстрактных схем.
    DEFINE_ENUM(
        TProtocol,
        /// \~english @brief Transmission Control Protocol. Generic identifier, port usually specified.
        /// \~russian @brief Протокол управления передачей. Общий идентификатор, порт обычно указывается.
        TCP         = -2,
        /// \~english @brief User Datagram Protocol. Generic identifier, port usually specified.
        /// \~russian @brief Протокол пользовательских датаграмм. Общий идентификатор, порт обычно указывается.
        UDP         = -3,
        /// \~english @brief WebSocket protocol. Generic identifier, port usually 80 or 443.
        /// \~russian @brief Протокол WebSocket. Общий идентификатор, порт обычно 80 или 443.
        WS          = -4,
        /// \~english @brief Secure WebSocket protocol. Generic identifier, port usually 443.
        /// \~russian @brief Безопасный протокол WebSocket. Общий идентификатор, порт обычно 443.
        WSS         = -5,
        /// \~english @brief File system path.
        /// \~russian @brief Путь в файловой системе.
        FILE        = -6,
        /// \~english @brief Local process execution.
        /// \~russian @brief Локальное выполнение процесса.
        PROCESS     = -7,
        /// \~english @brief Shared memory stream.
        /// \~russian @brief Межпроцессный обмен.
        IPC         = -8,
        /// \~english @brief Hypertext Transfer Protocol, default port 80.
        /// \~russian @brief Протокол передачи гипертекста, порт по умолчанию 80.
        HTTP        = 80,
        /// \~english @brief Hypertext Transfer Protocol Secure, default port 443.
        /// \~russian @brief Безопасный протокол передачи гипертекста, порт по умолчанию 443.
        HTTPS       = 443,
        /// \~english @brief Secure Shell protocol, default port 22.
        /// \~russian @brief Протокол безопасной оболочки, порт по умолчанию 22.
        SSH         = 22,
        /// \~english @brief Telnet protocol, default port 23.
        /// \~russian @brief Протокол Telnet, порт по умолчанию 23.
        Telnet      = 23,
        /// \~english @brief Real Time Streaming Protocol, default port 554.
        /// \~russian @brief Протокол потоковой передачи в реальном времени, порт по умолчанию 554.
        RTSP        = 554,
        /// \~english @brief MySQL database protocol, default port 3306.
        /// \~russian @brief Протокол базы данных MySQL, порт по умолчанию 3306.
        MYSQL       = 3306,
        /// \~english @brief PostgreSQL database protocol, default port 5432.
        /// \~russian @brief Протокол базы данных PostgreSQL, порт по умолчанию 5432.
        POSTGRES    = 5432
    );

    /// \~english @brief Macro likely defining a static `create` method or similar factory functionality
    ///     for creating shared pointers (`URI::TSharedPtr`) to `URI` instances.
    /// \~russian @brief Макрос, вероятно, определяющий статический метод `create` или аналогичную фабричную функциональность
    ///     для создания умных указателей (`URI::TSharedPtr`) на экземпляры `URI`.
    DEFINE_CREATE(URI)

    /// \~english @brief Constructs a URI object by parsing the provided URI string.
    ///     The constructor analyzes the source string and populates the internal members
    ///     representing different parts of the URI (scheme, host, port, path, etc.).
    /// \~russian @brief Конструирует объект URI путем разбора предоставленной строки URI.
    ///     Конструктор анализирует исходную строку и заполняет внутренние члены,
    ///     представляющие различные части URI (схема, хост, порт, путь и т.д.).
    /// \~english @param source The URI string to parse.
    /// \~russian @param source Строка URI для разбора.
    /// \~english @throws iridium::Exception or other exceptions if the URI string is malformed or cannot be parsed.
    /// \~russian @throws iridium::Exception или другие исключения, если строка URI некорректна или не может быть разобрана.
    URI(std::string const &source);

    /// \~english @brief Default destructor.
    /// \~russian @brief Деструктор по умолчанию.
   ~URI() = default;

    /// \~english @brief Represents an IPv4 address. This structure stores an IPv4 address as an array of 4 bytes.
    /// \~russian @brief Представляет IPv4-адрес. Эта структура хранит IPv4-адрес в виде массива из 4 байт.
    struct TIPv4: public std::array<uint8_t , 4> {
        /// \~english @brief Default constructor for TIPv4. Initializes the IPv4 address to 0.0.0.0.
        /// \~russian @brief Конструктор по умолчанию для TIPv4. Инициализирует IPv4-адрес значением 0.0.0.0.
        TIPv4();
        /// \~english @brief Macro for creating shared instances of TIPv4.
        /// \~russian @brief Макрос для создания разделяемых экземпляров TIPv4.
        DEFINE_CREATE(TIPv4)
    };

    /// \~english @brief Represents an IPv6 address. This structure stores an IPv6 address as an array of 8 16-bit integers.
    /// \~russian @brief Представляет IPv6-адрес. Эта структура хранит IPv6-адрес в виде массива из 8 16-битных целых чисел.
    struct TIPv6: public std::array<uint16_t, 8> {
        /// \~english @brief Macro for creating shared instances of TIPv6.
        /// \~russian @brief Макрос для создания разделяемых экземпляров TIPv6.
        DEFINE_CREATE(TIPv6)
    };

    /// \~english @brief Typedef for the port number, represented as a 16-bit unsigned integer.
    /// \~russian @brief Typedef для номера порта, представленного как 16-битное беззнаковое целое число.
    typedef uint16_t TPort;

    /// \~english @brief Typedef for a map of name-value pairs, typically used for query arguments.
    ///     Keys and values are both strings.
    /// \~russian @brief Typedef для карты пар имя-значение, обычно используемой для аргументов запроса.
    ///     Ключи и значения являются строками.
    typedef std::unordered_map<std::string, std::string> TMapNameValue;

    /// \~english @brief Gets the IPv4 address component of the URI, if present and valid.
    ///     This method may parse the host part of the URI on first call if it appears to be an IPv4 address.
    /// \~russian @brief Получает компонент IPv4-адреса URI, если он присутствует и действителен.
    ///     Этот метод может разбирать хост-часть URI при первом вызове, если она похожа на IPv4-адрес.
    /// \~english @return A const shared pointer to a `TIPv4` object if the host is a valid IPv4 address; otherwise, `nullptr`.
    /// \~russian @return Константный умный указатель на объект `TIPv4`, если хост является действительным IPv4-адресом; в противном случае `nullptr`.
    TIPv4::TConstSharedPtr getIPv4()       const;

    /// \~english @brief Gets the IPv6 address component of the URI, if present and valid.
    ///     This method may parse the host part of the URI on first call if it appears to be an IPv6 address.
    /// \~russian @brief Получает компонент IPv6-адреса URI, если он присутствует и действителен.
    ///     Этот метод может разбирать хост-часть URI при первом вызове, если она похожа на IPv6-адрес.
    /// \~english @return A const shared pointer to a `TIPv6` object if the host is a valid IPv6 address; otherwise, `nullptr`.
    /// \~russian @return Константный умный указатель на объект `TIPv6`, если хост является действительным IPv6-адресом; в противном случае `nullptr`.
    TIPv6::TConstSharedPtr getIPv6()       const;

    /// \~english @brief Gets the protocol (scheme) of the URI.
    /// \~russian @brief Получает протокол (схему) URI.
    /// \~english @return The `TProtocol` enum value representing the URI's scheme.
    /// \~russian @return Значение перечисления `TProtocol`, представляющее схему URI.
    TProtocol   getProtocol()   const;

    /// \~english @brief Gets the user information part of the URI (e.g., username in `user:password@host`).
    /// \~russian @brief Получает часть URI с информацией о пользователе (например, имя пользователя в `user:password@host`).
    /// \~english @return A string containing the user information, or an empty string if not present.
    /// \~russian @return Строка, содержащая информацию о пользователе, или пустая строка, если отсутствует.
    std::string getUser()       const;

    /// \~english @brief Gets the password part of the URI (e.g., password in `user:password@host`).
    /// \~russian @brief Получает часть URI с паролем (например, пароль в `user:password@host`).
    /// \~english @return A string containing the password, or an empty string if not present.
    /// \~russian @return Строка, содержащая пароль, или пустая строка, если отсутствует.
    std::string getPassword()   const;

    /// \~english @brief Gets the host component of the URI (e.g., domain name or IP address).
    /// \~russian @brief Получает хост-компонент URI (например, доменное имя или IP-адрес).
    /// \~english @return A string containing the host, or an empty string if not present.
    /// \~russian @return Строка, содержащая хост, или пустая строка, если отсутствует.
    std::string getHost()       const;

    /// \~english @brief Gets the port number component of the URI.
    ///     If the port is not explicitly specified in the URI string, this may return a default port
    ///     associated with the protocol, or 0 if no default is applicable or specified.
    /// \~russian @brief Получает компонент номера порта URI.
    ///     Если порт не указан явно в строке URI, может вернуть порт по умолчанию,
    ///     связанный с протоколом, или 0, если порт по умолчанию не применим или не указан.
    /// \~english @return The port number as `TPort` (uint16_t).
    /// \~russian @return Номер порта как `TPort` (uint16_t).
    TPort       getPort()       const;

    /// \~english @brief Gets the path component of the URI (e.g., `/path/to/resource`).
    /// \~russian @brief Получает компонент пути URI (например, `/path/to/resource`).
    /// \~english @return A string containing the path, or an empty string if not present.
    /// \~russian @return Строка, содержащая путь, или пустая строка, если отсутствует.
    std::string getPath()       const;

    /// \~english @brief Gets the query arguments part of the URI (e.g., `arg1=1&arg2=2` from `?arg1=1&arg2=2`).
    ///     The `todo: map` comment in the source suggests this might be intended to return a parsed map in the future.
    ///     Currently, it returns the raw query string.
    /// \~russian @brief Получает часть URI с аргументами запроса (например, `arg1=1&arg2=2` из `?arg1=1&arg2=2`).
    ///     Комментарий `todo: map` в исходном коде предполагает, что в будущем это может возвращать разобранную карту.
    ///     В настоящее время возвращает необработанную строку запроса.
    /// \~english @return A string containing the query arguments, or an empty string if not present.
    /// \~russian @return Строка, содержащая аргументы запроса, или пустая строка, если отсутствует.
    std::string getArguments()  const;

    /// \~english @brief Gets a constructed "address" string, typically combining host, port, and path.
    ///     The exact format depends on the URI scheme and components present. Refer to the comments at the top of the header for examples.
    /// \~russian @brief Получает сконструированную строку "адреса", обычно объединяющую хост, порт и путь.
    ///     Точный формат зависит от схемы URI и присутствующих компонентов. Примеры см. в комментариях в начале заголовочного файла.
    /// \~english @return A string representing the address part of the URI.
    /// \~russian @return Строка, представляющая адресную часть URI.
    std::string getAddress()    const;

    /// \~english @brief Gets the original, unparsed URI string that was used to construct this object.
    /// \~russian @brief Получает исходную, неразобранную строку URI, которая использовалась для конструирования этого объекта.
    /// \~english @return The original source URI string.
    /// \~russian @return Исходная строка URI.
    std::string getSource()     const;
    // todo: url arguments (This comment suggests planned enhancements for argument parsing)

    /// \~english @brief Equality comparison operator. Compares this URI with another URI.
    ///     Comparison is likely based on the original source string or all parsed components.
    /// \~russian @brief Оператор сравнения на равенство. Сравнивает этот URI с другим URI.
    ///     Сравнение, вероятно, основано на исходной строке или всех разобранных компонентах.
    /// \~english @param uri The URI object to compare against.
    /// \~russian @param uri Объект URI для сравнения.
    /// \~english @return `true` if the URIs are considered equal, `false` otherwise.
    /// \~russian @return `true`, если URI считаются равными, иначе `false`.
    bool operator == (URI const &uri) const;

    /// \~english @brief Less-than comparison operator. Compares this URI with another URI.
    ///     Provides a strict weak ordering, likely based on the original source string or a lexicographical comparison of components.
    /// \~russian @brief Оператор сравнения "меньше чем". Сравнивает этот URI с другим URI.
    ///     Обеспечивает строгий слабый порядок, вероятно, на основе исходной строки или лексикографического сравнения компонентов.
    /// \~english @param uri The URI object to compare against.
    /// \~russian @param uri Объект URI для сравнения.
    /// \~english @return `true` if this URI is considered less than the other, `false` otherwise.
    /// \~russian @return `true`, если этот URI считается меньше другого, иначе `false`.
    bool operator <  (URI const &uri) const;

private:
    /// \~english @brief Stores the original URI string passed to the constructor.
    /// \~russian @brief Хранит исходную строку URI, переданную в конструктор.
    std::string     m_source;
    /// \~english @brief The parsed protocol (scheme) of the URI.
    /// \~russian @brief Разобранный протокол (схема) URI.
    TProtocol       m_protocol;
    /// \~english @brief The parsed user information (username) from the URI.
    /// \~russian @brief Разобранная информация о пользователе (имя пользователя) из URI.
    std::string     m_user;
    /// \~english @brief The parsed password from the URI.
    /// \~russian @brief Разобранный пароль из URI.
    std::string     m_password;
    /// \~english @brief The parsed host (domain or IP address) from the URI.
    /// \~russian @brief Разобранный хост (домен или IP-адрес) из URI.
    std::string     m_host;
    /// \~english @brief The parsed path component from the URI.
    /// \~russian @brief Разобранный компонент пути из URI.
    std::string     m_path;
    /// \~english @brief A constructed string representing the main address part (e.g., host:port/path).
    /// \~russian @brief Сконструированная строка, представляющая основную адресную часть (например, хост:порт/путь).
    std::string     m_address;
    /// \~english @brief The raw query arguments string (part after '?').
    /// \~russian @brief Необработанная строка аргументов запроса (часть после '?').
    std::string     m_arguments;
    /// \~english @brief Lazily parsed IPv4 address.
    /// \~russian @brief IPv4-адрес, разбираемый "лениво".
    std::shared_ptr<TIPv4> mutable m_ipv4;
    /// \~english @brief Lazily parsed IPv6 address.
    /// \~russian @brief IPv6-адрес, разбираемый "лениво".
    std::shared_ptr<TIPv6> mutable m_ipv6;
    /// \~english @brief The parsed port number.
    /// \~russian @brief Разобранный номер порта.
    TPort           m_port;
};


} // io
} // iridium


/// \~english @brief Declares a conversion specialization from `std::string` to `iridium::io::URI::TIPv4`.
/// \~russian @brief Объявляет специализацию преобразования из `std::string` в `iridium::io::URI::TIPv4`.
DEFINE_CONVERT(iridium::io::URI::TIPv4, std::string)
/// \~english @brief Declares a conversion specialization from `iridium::io::URI::TIPv4` to `std::string`.
/// \~russian @brief Объявляет специализацию преобразования из `iridium::io::URI::TIPv4` в `std::string`.
DEFINE_CONVERT(std::string, iridium::io::URI::TIPv4)

/// \~english @brief Declares a conversion specialization from `std::string` to `iridium::io::URI`.
///     Likely constructs a URI by parsing the string.
/// \~russian @brief Объявляет специализацию преобразования из `std::string` в `iridium::io::URI`.
///     Вероятно, конструирует URI путем разбора строки.
DEFINE_CONVERT(iridium::io::URI, std::string)
/// \~english @brief Declares a conversion specialization from `iridium::io::URI` to `std::string`.
///     Likely returns the original source string of the URI.
/// \~russian @brief Объявляет специализацию преобразования из `iridium::io::URI` в `std::string`.
///     Вероятно, возвращает исходную строку URI.
DEFINE_CONVERT(std::string, iridium::io::URI)


#endif // HEADER_URI_F2F08356_C16A_4981_92C6_32E1B85D4384
