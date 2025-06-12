// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines the interface for a factory that creates I/O stream ports, particularly for network sockets.
/// \~russian @brief Определяет интерфейс для фабрики, создающей порты потоков ввода-вывода, в частности для сетевых сокетов.
#ifndef HEADER_STREAM_FACTORY_E90EE5D7_FE8C_4512_B392_C50C72BF1662
#define HEADER_STREAM_FACTORY_E90EE5D7_FE8C_4512_B392_C50C72BF1662


#include "iridium/io/stream.h" // For IStreamPort::TSharedPtr
#include "iridium/io/uri.h"    // For URI
#include "iridium/smart_ptr.h" // For DEFINE_INTERFACE


namespace iridium {
namespace io {


/// \~english @brief Interface for a stream factory, responsible for creating various types of I/O stream ports.
///     This factory specifically focuses on creating client and server socket stream ports based on URI configurations.
///     Concrete implementations of this interface would handle the specifics of socket creation for different
///     network protocols or transport layers as indicated by the URI.
/// \~russian @brief Интерфейс для фабрики потоков, отвечающей за создание различных типов портов потоков ввода-вывода.
///     Эта фабрика специализируется на создании клиентских и серверных сокетных портов потоков на основе конфигураций URI.
///     Конкретные реализации этого интерфейса будут обрабатывать специфику создания сокетов для различных
///     сетевых протоколов или транспортных уровней, указанных в URI.
class IStreamFactory {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IStreamFactory)

    /// \~english @brief Creates a client socket stream port configured to connect to the specified URI.
    ///     The URI should contain the necessary information (e.g., hostname, port, protocol like tcp or udp)
    ///     to establish a client-side network connection. The returned stream port can then be used
    ///     for bidirectional communication.
    /// \~russian @brief Создает клиентский сокетный порт потока, настроенный для подключения к указанному URI.
    ///     URI должен содержать необходимую информацию (например, имя хоста, порт, протокол, такой как tcp или udp)
    ///     для установления клиентского сетевого соединения. Возвращенный порт потока затем может использоваться
    ///     для двунаправленной связи.
    /// \~english @param uri The URI specifying the address and parameters for the client socket connection.
    /// \~russian @param uri URI, указывающий адрес и параметры для подключения клиентского сокета.
    /// \~english @return A shared pointer to an `IStreamPort` representing the client socket.
    ///     Returns `nullptr` or throws an exception if the client socket cannot be created or configured.
    /// \~russian @return Умный указатель на `IStreamPort`, представляющий клиентский сокет.
    ///     Возвращает `nullptr` или выбрасывает исключение, если клиентский сокет не может быть создан или настроен.
    /// \~english @throws iridium::Exception or derived classes on failure (e.g., invalid URI, network errors).
    /// \~russian @throws iridium::Exception или производные классы в случае сбоя (например, неверный URI, сетевые ошибки).
    virtual IStreamPort::TSharedPtr createClientSocket(URI const &uri) = 0;

    /// \~english @brief Creates a server socket stream port configured to listen on the address specified by the URI.
    ///     The URI should contain the necessary information (e.g., local IP address, port, protocol)
    ///     to bind a listening socket. This stream port would typically be used by a server
    ///     to accept incoming client connections.
    /// \~russian @brief Создает серверный сокетный порт потока, настроенный для прослушивания на адресе, указанном в URI.
    ///     URI должен содержать необходимую информацию (например, локальный IP-адрес, порт, протокол)
    ///     для привязки прослушивающего сокета. Этот порт потока обычно используется сервером
    ///     для приема входящих клиентских соединений.
    /// \~english @param uri The URI specifying the local address and parameters for the server socket.
    /// \~russian @param uri URI, указывающий локальный адрес и параметры для серверного сокета.
    /// \~english @return A shared pointer to an `IStreamPort` representing the server listening socket.
    ///     Returns `nullptr` or throws an exception if the server socket cannot be created or configured.
    /// \~russian @return Умный указатель на `IStreamPort`, представляющий серверный прослушивающий сокет.
    ///     Возвращает `nullptr` или выбрасывает исключение, если серверный сокет не может быть создан или настроен.
    /// \~english @throws iridium::Exception or derived classes on failure (e.g., invalid URI, address in use, network errors).
    /// \~russian @throws iridium::Exception или производные классы в случае сбоя (например, неверный URI, адрес уже используется, сетевые ошибки).
    virtual IStreamPort::TSharedPtr createServerSocket(URI const &uri) = 0;
};


} // io
} // iridium


#endif // HEADER_STREAM_FACTORY_E90EE5D7_FE8C_4512_B392_C50C72BF1662
