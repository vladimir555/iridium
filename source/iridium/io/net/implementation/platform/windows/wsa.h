// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#ifndef HEADER_SOCKET_51FA6227_172E_42DD_BF9E_98E03940D246
#define HEADER_SOCKET_51FA6227_172E_42DD_BF9E_98E03940D246


#include "iridium/pattern/singleton.h"
#include "iridium/io/uri.h"
#include "iridium/io/buffer.h"
#include "iridium/smart_ptr.h"

#include <string>
#include <list>

#include <ws2tcpip.h>


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


/// \~english @brief Singleton class for managing Windows Sockets API (WSA) initialization and utility functions.
///     Ensures `WSAStartup` is called on first use and `WSACleanup` on program termination (via destructor).
///     Provides wrappers for common socket operations and error handling.
/// \~russian @brief Класс-одиночка для управления инициализацией Windows Sockets API (WSA) и служебными функциями.
///     Гарантирует вызов `WSAStartup` при первом использовании и `WSACleanup` при завершении программы (через деструктор).
///     Предоставляет обертки для общих операций с сокетами и обработки ошибок.
class WSA: public iridium::pattern::Singleton<WSA> {
public:
    /// \~english @brief Destructor. Calls `WSACleanup()` to release Winsock resources.
    /// \~russian @brief Деструктор. Вызывает `WSACleanup()` для освобождения ресурсов Winsock.
    virtual ~WSA();

    /// \~english @brief Resolves a hostname to an IPv4 address using Winsock.
    /// \~russian @brief Разрешает имя хоста в IPv4-адрес с использованием Winsock.
    /// \~english @param name The hostname to resolve.
    /// \~russian @param name Имя хоста для разрешения.
    /// \~english @return A `URI::TIPv4` structure representing the resolved IPv4 address.
    /// \~russian @return Структура `URI::TIPv4`, представляющая разрешенный IPv4-адрес.
    /// \~english @throws std::runtime_error if hostname resolution fails.
    /// \~russian @throws std::runtime_error, если разрешение имени хоста не удалось.
    URI::TIPv4   getIPv4ByName(std::string const &name);

    /// \~english @brief Reads data from a socket.
    /// \~russian @brief Читает данные из сокета.
    /// \~english @param socket The socket to read from.
    /// \~russian @param socket Сокет для чтения.
    /// \~english @param size The maximum number of bytes to read.
    /// \~russian @param size Максимальное количество байт для чтения.
    /// \~english @return A `Buffer` containing the data read. An empty buffer may indicate EOF or an error.
    /// \~russian @return `Buffer`, содержащий прочитанные данные. Пустой буфер может указывать на EOF или ошибку.
    /// \~english @throws std::runtime_error on socket errors.
    /// \~russian @throws std::runtime_error при ошибках сокета.
    Buffer  read(SOCKET const &socket, size_t const &size);

    /// \~english @brief Writes data from a buffer to a socket.
    /// \~russian @brief Записывает данные из буфера в сокет.
    /// \~english @param socket The socket to write to.
    /// \~russian @param socket Сокет для записи.
    /// \~english @param packet The `Buffer` containing data to write.
    /// \~russian @param packet `Buffer`, содержащий данные для записи.
    /// \~english @return The number of bytes actually written.
    /// \~russian @return Количество фактически записанных байт.
    /// \~english @throws std::runtime_error on socket errors.
    /// \~russian @throws std::runtime_error при ошибках сокета.
    size_t  write(SOCKET const &socket, Buffer const &packet);

    /// \~english @brief Closes a socket.
    /// \~russian @brief Закрывает сокет.
    /// \~english @param socket The socket to close.
    /// \~russian @param socket Сокет для закрытия.
    /// \~english @throws std::runtime_error on socket errors.
    /// \~russian @throws std::runtime_error при ошибках сокета.
    void    close(SOCKET const &socket);

    /// \~english @brief Connects a socket to a remote address specified by a URI.
    /// \~russian @brief Подключает сокет к удаленному адресу, указанному в URI.
    /// \~english @param uri The URI containing the address and port to connect to.
    /// \~russian @param uri URI, содержащий адрес и порт для подключения.
    /// \~english @return The connected `SOCKET`.
    /// \~russian @return Подключенный `SOCKET`.
    /// \~english @throws std::runtime_error if socket creation or connection fails.
    /// \~russian @throws std::runtime_error, если создание сокета или подключение не удалось.
    SOCKET  connect(URI const &uri);

    /// \~english @brief Puts a socket into listening mode for incoming connections.
    /// \~russian @brief Переводит сокет в режим прослушивания для входящих подключений.
    /// \~english @param uri The URI containing the local address and port to listen on.
    /// \~russian @param uri URI, содержащий локальный адрес и порт для прослушивания.
    /// \~english @return The listening `SOCKET`.
    /// \~russian @return Слушающий `SOCKET`.
    /// \~english @throws std::runtime_error if socket creation, bind, or listen fails.
    /// \~russian @throws std::runtime_error, если создание сокета, привязка или прослушивание не удалось.
    SOCKET  listen(URI const &uri);

    /// \~english @brief Accepts an incoming connection on a listening socket.
    /// \~russian @brief Принимает входящее соединение на слушающем сокете.
    /// \~english @param socket The listening socket.
    /// \~russian @param socket Слушающий сокет.
    /// \~english @return The `SOCKET` for the newly accepted connection.
    /// \~russian @return `SOCKET` для нового принятого соединения.
    /// \~english @throws std::runtime_error if `accept` fails.
    /// \~russian @throws std::runtime_error, если `accept` не удался.
    SOCKET  accept(SOCKET const &socket);

    /// \~english @brief Shuts down sending and/or receiving on a socket.
    /// \~russian @brief Завершает отправку и/или получение данных на сокете.
    /// \~english @param socket The socket to shut down.
    /// \~russian @param socket Сокет для завершения работы.
    /// \~english @throws std::runtime_error on socket errors.
    /// \~russian @throws std::runtime_error при ошибках сокета.
    void    shutdown(SOCKET const &socket);

    /// \~english @brief Sets the blocking or non-blocking mode for a socket.
    /// \~russian @brief Устанавливает блокирующий или неблокирующий режим для сокета.
    /// \~english @param socket The socket to modify.
    /// \~russian @param socket Сокет для изменения.
    /// \~english @param is_blocking `true` to set to blocking mode, `false` for non-blocking.
    /// \~russian @param is_blocking `true` для установки блокирующего режима, `false` для неблокирующего.
    /// \~english @throws std::runtime_error if `ioctlsocket` fails.
    /// \~russian @throws std::runtime_error, если `ioctlsocket` не удался.
    void    setBlockingMode(SOCKET const &socket, bool const &is_blocking);

private:
    friend class iridium::pattern::Singleton<WSA>;

    /// \~english @brief Private constructor to enforce singleton pattern.
    ///     Calls `WSAStartup` to initialize Winsock.
    /// \~russian @brief Приватный конструктор для реализации шаблона "одиночка".
    ///     Вызывает `WSAStartup` для инициализации Winsock.
    /// \~english @throws std::runtime_error if `WSAStartup` fails.
    /// \~russian @throws std::runtime_error, если `WSAStartup` не удалось.
    WSA();

    /// \~english @brief Gets a string representation of the last Winsock error.
    /// \~russian @brief Получает строковое представление последней ошибки Winsock.
    /// \~english @return A string describing the last error.
    /// \~russian @return Строка, описывающая последнюю ошибку.
    std::string getLastWSAErrorString() const;

    /// \~english @brief Asserts that a result is equal to an expected value.
    ///     Throws a runtime_error with a Winsock error message if the assertion fails.
    /// \~russian @brief Проверяет, что результат равен ожидаемому значению.
    ///     Выбрасывает runtime_error с сообщением об ошибке Winsock, если проверка не удалась.
    /// \~english @tparam T Type of the values to compare.
    /// \~russian @tparam T Тип сравниваемых значений.
    /// \~english @param result The actual result.
    /// \~russian @param result Фактический результат.
    /// \~english @param value The expected value.
    /// \~russian @param value Ожидаемое значение.
    /// \~english @param message Message to include in the exception.
    /// \~russian @param message Сообщение для включения в исключение.
    template<typename T>
    void assertEQ(T const &result, T const &value, std::string const &message) const;

    /// \~english @brief Asserts that a result is not equal to a specific value (often an error indicator).
    ///     Throws a runtime_error with a Winsock error message if the assertion fails.
    /// \~russian @brief Проверяет, что результат не равен определенному значению (часто индикатору ошибки).
    ///     Выбрасывает runtime_error с сообщением об ошибке Winsock, если проверка не удалась.
    /// \~english @tparam T Type of the values to compare.
    /// \~russian @tparam T Тип сравниваемых значений.
    /// \~english @param result The actual result (rvalue reference to allow modification or pass-through).
    /// \~russian @param result Фактический результат (rvalue ссылка, чтобы разрешить изменение или сквозную передачу).
    /// \~english @param value The value `result` should not be equal to.
    /// \~russian @param value Значение, которому `result` не должен быть равен.
    /// \~english @param message Message to include in the exception.
    /// \~russian @param message Сообщение для включения в исключение.
    /// \~english @return The `result` if the assertion passes.
    /// \~russian @return `result`, если проверка прошла успешно.
    template<typename T>
    T &assertNE(T &&result, T const &value, std::string const &message) const;

    /// \~english @brief Stores data about the Windows Sockets implementation. Initialized by `WSAStartup`.
    /// \~russian @brief Хранит данные о реализации Windows Sockets. Инициализируется `WSAStartup`.
    WSADATA m_wsa_data;
};


// implementation


template<typename T>
void WSA::assertEQ(T const &result, T const &value, std::string const &message) const {
    if (result != value)
        throw std::runtime_error(message + ": " + getLastWSAErrorString()); // ----->
}


template<typename T>
T &WSA::assertNE(T &&result, T const &value, std::string const &message) const {
    if (result == value)
        throw std::runtime_error(message + ": " + getLastWSAErrorString()); // ----->
    else
        return result; // ----->
}


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // HEADER_SOCKET_51FA6227_172E_42DD_BF9E_98E03940D246


#endif // WINDOWS_PLATFORM
