// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F
#define HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/uri.h"
#include "iridium/io/net/socket.h"
#include "iridium/convertion/convert.h"
#include "iridium/pattern/non_copyable.h"

#include <cstring>
#include <string>
#include <netinet/in.h>


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {
namespace unix_ {


/// \~english @brief Base class for Unix platform socket implementations.
///     Provides common functionalities for socket operations like open, close, listen, connect, accept, read, write,
///     and manages socket options like blocking mode. It is non-copyable.
/// \~russian @brief Базовый класс для реализаций сокетов на платформе Unix.
///     Предоставляет общие функции для операций с сокетами, таких как открытие, закрытие, прослушивание, подключение, прием, чтение, запись,
///     и управляет опциями сокета, такими как режим блокировки. Класс является некопируемым.
class CSocketBase: public ISocket, public pattern::NonCopyable {
protected:
    /// \~english @brief Constructs a socket base associated with a URI.
    ///     The URI is used to determine connection parameters (e.g., address, port).
    /// \~russian @brief Конструирует базовый сокет, связанный с URI.
    ///     URI используется для определения параметров подключения (например, адрес, порт).
    /// \~english @param uri The URI representing the socket endpoint.
    /// \~russian @param uri URI, представляющий конечную точку сокета.
    CSocketBase(URI const &uri);

    /// \~english @brief Constructs a socket base from an existing socket file descriptor.
    ///     Used for sockets created externally (e.g., by `accept()`).
    /// \~russian @brief Конструирует базовый сокет из существующего файлового дескриптора сокета.
    ///     Используется для сокетов, созданных извне (например, функцией `accept()`).
    /// \~english @param socket The existing socket file descriptor.
    /// \~russian @param socket Существующий файловый дескриптор сокета.
    CSocketBase(int const &socket);

    /// \~english @brief Virtual destructor. Ensures proper cleanup for derived classes.
    ///     Closes the socket if it's still open.
    /// \~russian @brief Виртуальный деструктор. Обеспечивает правильную очистку для производных классов.
    ///     Закрывает сокет, если он все еще открыт.
    virtual ~CSocketBase() = default; // Should call close() in actual implementation if m_is_opened

    /// \~english @brief Opens a new socket (e.g., using `socket()` syscall).
    ///     The socket type (e.g., TCP, UDP) is typically determined by the URI's protocol.
    /// \~russian @brief Открывает новый сокет (например, используя системный вызов `socket()`).
    ///     Тип сокета (например, TCP, UDP) обычно определяется протоколом URI.
    /// \~english @throws std::runtime_error if socket creation fails.
    /// \~russian @throws std::runtime_error, если создание сокета не удалось.
    void    open();

    /// \~english @brief Closes the socket file descriptor.
    /// \~russian @brief Закрывает файловый дескриптор сокета.
    /// \~english @throws std::runtime_error if `close()` syscall fails.
    /// \~russian @throws std::runtime_error, если системный вызов `close()` не удался.
    void    close();

    /// \~english @brief Puts the socket into a listening state for incoming connections.
    ///     Typically used for server sockets. Calls `bind()` and `listen()` syscalls.
    /// \~russian @brief Переводит сокет в состояние прослушивания для входящих подключений.
    ///     Обычно используется для серверных сокетов. Вызывает системные вызовы `bind()` и `listen()`.
    /// \~english @throws std::runtime_error if `bind()` or `listen()` syscalls fail.
    /// \~russian @throws std::runtime_error, если системные вызовы `bind()` или `listen()` не удались.
    void    listen();

    /// \~english @brief Connects the socket to a remote address specified by the URI.
    ///     Typically used for client sockets. Calls `connect()` syscall.
    /// \~russian @brief Подключает сокет к удаленному адресу, указанному в URI.
    ///     Обычно используется для клиентских сокетов. Вызывает системный вызов `connect()`.
    /// \~english @throws std::runtime_error if `connect()` syscall fails (unless non-blocking and error is EINPROGRESS).
    /// \~russian @throws std::runtime_error, если системный вызов `connect()` не удался (кроме неблокирующего режима и ошибки EINPROGRESS).
    void    connect();

    /// \~english @brief Accepts an incoming connection on a listening socket.
    /// \~russian @brief Принимает входящее соединение на слушающем сокете.
    /// \~english @return The file descriptor of the newly accepted client socket.
    /// \~russian @return Файловый дескриптор нового принятого клиентского сокета.
    /// \~english @throws std::runtime_error if `accept()` syscall fails.
    /// \~russian @throws std::runtime_error, если системный вызов `accept()` не удался.
    int     accept();

    /// \~english @brief Gets the local URI of the socket.
    ///     Uses `getsockname()` to retrieve the local address and port.
    /// \~russian @brief Получает локальный URI сокета.
    ///     Использует `getsockname()` для получения локального адреса и порта.
    /// \~english @param protocol The protocol type to set in the resulting URI.
    /// \~russian @param protocol Тип протокола для установки в результирующем URI.
    /// \~english @return A `URI` object representing the local endpoint of the socket.
    /// \~russian @return Объект `URI`, представляющий локальную конечную точку сокета.
    /// \~english @throws std::runtime_error if `getsockname()` fails.
    /// \~russian @throws std::runtime_error, если `getsockname()` не удался.
    URI     getSocketURI(URI::TProtocol const &protocol);

    /// \~english @brief Writes data from the provided buffer to the socket.
    ///     Uses the `send()` syscall.
    /// \~russian @brief Записывает данные из предоставленного буфера в сокет.
    ///     Использует системный вызов `send()`.
    /// \~english @param buffer The buffer containing data to write.
    /// \~russian @param buffer Буфер, содержащий данные для записи.
    /// \~english @return The number of bytes actually written.
    /// \~russian @return Количество фактически записанных байт.
    /// \~english @throws std::runtime_error if `send()` syscall fails.
    /// \~russian @throws std::runtime_error, если системный вызов `send()` не удался.
    size_t  write(Buffer::TSharedPtr const &buffer);

    /// \~english @brief Reads data from the socket into a new buffer.
    ///     Uses the `recv()` syscall.
    /// \~russian @brief Читает данные из сокета в новый буфер.
    ///     Использует системный вызов `recv()`.
    /// \~english @param size The maximum number of bytes to read.
    /// \~russian @param size Максимальное количество байт для чтения.
    /// \~english @return A shared pointer to a buffer containing the data read. Returns `nullptr` on EOF or error.
    /// \~russian @return Умный указатель на буфер, содержащий прочитанные данные. Возвращает `nullptr` при EOF или ошибке.
    /// \~english @throws std::runtime_error if `recv()` syscall fails for reasons other than non-blocking would-block.
    /// \~russian @throws std::runtime_error, если системный вызов `recv()` завершается с ошибкой по причинам, отличным от блокировки в неблокирующем режиме.
    Buffer::TSharedPtr read(size_t const &size);

    // todo: rename non-blocking -> overlapped
    /// \~english @brief Sets the blocking or non-blocking mode for the given socket file descriptor.
    /// \~russian @brief Устанавливает блокирующий или неблокирующий режим для указанного файлового дескриптора сокета.
    /// \~english @param socket The file descriptor of the socket.
    /// \~russian @param socket Файловый дескриптор сокета.
    /// \~english @param is_blocking `true` to set to blocking mode, `false` for non-blocking.
    /// \~russian @param is_blocking `true` для установки блокирующего режима, `false` для неблокирующего.
    /// \~english @throws std::runtime_error if `fcntl()` fails to set the mode.
    /// \~russian @throws std::runtime_error, если `fcntl()` не удалось установить режим.
    void    setBlockingMode(int const &socket, bool const &is_blocking);

    /// \~english @brief Gets the URI associated with this socket.
    /// \~russian @brief Получает URI, связанный с этим сокетом.
    /// \~english @return A shared pointer to the `URI` object provided at construction or derived from an accepted socket.
    /// \~russian @return Умный указатель на объект `URI`, предоставленный при конструировании или полученный из принятого сокета.
    URI::TSharedPtr getURI() const override;
//    int     getID()  const override; // This method is commented out in the source.

    /// \~english @brief The socket file descriptor. Value < 0 indicates an invalid or closed socket.
    /// \~russian @brief Файловый дескриптор сокета. Значение < 0 указывает на недействительный или закрытый сокет.
    int             m_socket;
    /// \~english @brief The URI associated with this socket, used for connection or identification.
    /// \~russian @brief URI, связанный с этим сокетом, используемый для подключения или идентификации.
    URI::TSharedPtr m_uri;
    /// \~english @brief Flag indicating whether the socket is currently open and valid.
    /// \~russian @brief Флаг, указывающий, открыт ли сокет в данный момент и действителен ли он.
    bool            m_is_opened;

private:
    /// \~english @brief Template utility function to check the result of a system call.
    ///     Throws a `std::runtime_error` with a formatted message (including URI and `strerror(errno)`) if the result is less than 0.
    /// \~russian @brief Шаблонная служебная функция для проверки результата системного вызова.
    ///     Выбрасывает `std::runtime_error` с отформатированным сообщением (включая URI и `strerror(errno)`), если результат меньше 0.
    /// \~english @tparam T The type of the result.
    /// \~russian @tparam T Тип результата.
    /// \~english @param result The result value of the system call.
    /// \~russian @param result Значение результата системного вызова.
    /// \~english @param message A descriptive message for the operation being checked.
    /// \~russian @param message Описательное сообщение для проверяемой операции.
    /// \~english @return The `result` if it's not less than 0.
    /// \~russian @return `result`, если он не меньше 0.
    template<typename T>
    T assertOK(T const &result, std::string const &message);

    /// \~english @brief Static method to initialize signal handling, specifically to ignore `SIGPIPE`.
    ///     This prevents the program from terminating if a write is attempted on a closed pipe or socket.
    /// \~russian @brief Статический метод для инициализации обработки сигналов, в частности, для игнорирования `SIGPIPE`.
    ///     Это предотвращает завершение программы при попытке записи в закрытый канал или сокет.
    /// \~english @return An integer, typically 0, used as a static initializer guard. Its specific value isn't critical.
    /// \~russian @return Целое число, обычно 0, используемое как статический инициализатор-страж. Его конкретное значение не критично.
    static int initSignal();

    /// \~english @brief Static constant used for `setsockopt` (e.g., for `SO_REUSEADDR`). Value is 1.
    /// \~russian @brief Статическая константа, используемая для `setsockopt` (например, для `SO_REUSEADDR`). Значение равно 1.
    static int const    YES;

    /// \~english @brief The socket address structure (IPv4) used for bind, connect, etc.
    /// \~russian @brief Структура адреса сокета (IPv4), используемая для bind, connect и т.д.
    struct sockaddr_in  m_address;
};


template<typename T>
T CSocketBase::assertOK(T const &result, std::string const &message) {
    using convertion::convert;
    using std::string;
    if (result < 0) {
        if (m_uri)
            throw std::runtime_error(message + ": uri " + convert<string>(*m_uri) +
                ", " + std::strerror(errno) + ", code " + convert<string>(errno)); // ----->
        else
            throw std::runtime_error(message +
                ", " + std::strerror(errno) + ", code " + convert<string>(errno)); // ----->
    } else
        return result; // ----->
}


} // unix_
} // platform
} // implementation
} // net
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_SOCKET_7C85EFA4_9F88_4A5A_8F23_881B465DDB1F
