// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_STREAM_PORT_3353B98B_8ECE_48F8_8627_EE37A8AB711C
#define HEADER_STREAM_PORT_3353B98B_8ECE_48F8_8627_EE37A8AB711C


#include "iridium/platform.h"


#ifdef UNIX_PLATFORM


#include "iridium/io/uri.h"
#include "iridium/io/stream.h"
#include "iridium/convertion/convert.h"
#include "iridium/pattern/non_copyable.h"

#include <cstring>
#include <string>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {
namespace unix_ {


/// \~english @brief Base class for Unix-like platform stream ports (e.g., sockets, pipes).
///     Provides common functionality for stream ports based on file descriptors,
///     including reading, writing, and managing blocking mode.
///     It is non-copyable. `initialize()` and `finalize()` are pure virtual and must be implemented by derived classes.
/// \~russian @brief Базовый класс для потоковых портов Unix-подобных платформ (например, сокетов, каналов).
///     Предоставляет общую функциональность для потоковых портов на основе файловых дескрипторов,
///     включая чтение, запись и управление режимом блокировки.
///     Класс является некопируемым. `initialize()` и `finalize()` являются чисто виртуальными и должны быть реализованы производными классами.
class CStreamPort: virtual public IStreamPort, public pattern::NonCopyable {
protected:
    /// \~english @brief Protected constructor. Initializes the stream port with a URI.
    /// \~russian @brief Защищенный конструктор. Инициализирует потоковый порт с помощью URI.
    /// \~english @param uri The URI associated with this stream port (e.g., "tcp://host:port", "file:///path").
    /// \~russian @param uri URI, связанный с этим потоковым портом (например, "tcp://host:port", "file:///path").
    CStreamPort(URI const &uri);
    /// \~english @brief Virtual destructor. Ensures proper cleanup for derived classes.
    /// \~russian @brief Виртуальный деструктор. Обеспечивает правильную очистку для производных классов.
    virtual ~CStreamPort() = default;

public:
    /// \~english @brief Pure virtual method to initialize the stream port (e.g., open socket, connect).
    /// \~russian @brief Чисто виртуальный метод для инициализации потокового порта (например, открытие сокета, подключение).
    virtual void            initialize() override = 0;
    /// \~english @brief Pure virtual method to finalize the stream port (e.g., close socket).
    /// \~russian @brief Чисто виртуальный метод для завершения работы потокового порта (например, закрытие сокета).
    virtual void            finalize() override = 0;

    /// \~english @brief Writes data from the provided buffer to the stream's writer file descriptor.
    /// \~russian @brief Записывает данные из предоставленного буфера в файловый дескриптор записи потока.
    /// \~english @param buffer The buffer containing data to write.
    /// \~russian @param buffer Буфер, содержащий данные для записи.
    /// \~english @return The number of bytes actually written.
    /// \~russian @return Количество фактически записанных байт.
    /// \~english @throws std::runtime_error if the stream is not open or if a write error occurs.
    /// \~russian @throws std::runtime_error, если поток не открыт или возникает ошибка записи.
    size_t                  write(Buffer::TSharedPtr const &buffer) override;

    /// \~english @brief Reads data from the stream's reader file descriptor into a new buffer.
    /// \~russian @brief Читает данные из файлового дескриптора чтения потока в новый буфер.
    /// \~english @param size The maximum number of bytes to read. Defaults to `DEFAULT_BUFFER_SIZE`.
    /// \~russian @param size Максимальное количество байт для чтения. По умолчанию `DEFAULT_BUFFER_SIZE`.
    /// \~english @return A shared pointer to a buffer containing the data read. Returns `nullptr` if EOF is reached or an error occurs that isn't an interrupt.
    /// \~russian @return Умный указатель на буфер, содержащий прочитанные данные. Возвращает `nullptr`, если достигнут EOF или возникает ошибка, не являющаяся прерыванием.
    /// \~english @throws std::runtime_error if the stream is not open or if a read error occurs (and is not EINTR/EAGAIN).
    /// \~russian @throws std::runtime_error, если поток не открыт или возникает ошибка чтения (и это не EINTR/EAGAIN).
    Buffer::TSharedPtr      read(size_t const &size = DEFAULT_BUFFER_SIZE) override;

    /// \~english @brief Gets the list of underlying OS file descriptor handles for this stream.
    ///     Typically contains the reader and writer file descriptors if they are distinct and valid.
    /// \~russian @brief Получает список базовых файловых дескрипторов ОС для этого потока.
    ///     Обычно содержит файловые дескрипторы чтения и записи, если они различны и действительны.
    /// \~english @return A list of `uintptr_t` representing the file descriptors.
    /// \~russian @return Список `uintptr_t`, представляющий файловые дескрипторы.
    std::list<uintptr_t>    getHandles() const override;

    /// \~english @brief Gets the URI associated with this stream port.
    /// \~russian @brief Получает URI, связанный с этим потоковым портом.
    /// \~english @return A shared pointer to the `URI` object.
    /// \~russian @return Умный указатель на объект `URI`.
    URI::TSharedPtr         getURI() const override;

protected:
    /// \~english @brief Sets the blocking or non-blocking mode for both reader and writer file descriptors.
    /// \~russian @brief Устанавливает блокирующий или неблокирующий режим для файловых дескрипторов чтения и записи.
    /// \~english @param is_blocking `true` to set to blocking mode, `false` for non-blocking.
    /// \~russian @param is_blocking `true` для установки блокирующего режима, `false` для неблокирующего.
    /// \~english @throws std::runtime_error if `fcntl` fails to set the mode.
    /// \~russian @throws std::runtime_error, если `fcntl` не удалось установить режим.
    void            setBlockingMode(bool const &is_blocking);

    /// \~english @brief Atomic integer storing the file descriptor for reading. Initialized to -1.
    /// \~russian @brief Атомарное целое число, хранящее файловый дескриптор для чтения. Инициализируется значением -1.
    std::atomic<int>    m_fd_reader;
    /// \~english @brief Atomic integer storing the file descriptor for writing. Initialized to -1.
    /// \~russian @brief Атомарное целое число, хранящее файловый дескриптор для записи. Инициализируется значением -1.
    std::atomic<int>    m_fd_writer;
    /// \~english @brief The URI associated with this stream port, provided at construction.
    /// \~russian @brief URI, связанный с этим потоковым портом, предоставленный при конструировании.
    URI::TSharedPtr     m_uri;
    /// \~english @brief Flag indicating whether the stream port is currently open (initialized and not finalized).
    /// \~russian @brief Флаг, указывающий, открыт ли в данный момент потоковый порт (инициализирован и не финализирован).
    bool                m_is_opened;
    /// \~english @brief Flag indicating the current blocking mode of the file descriptors.
    /// \~russian @brief Флаг, указывающий текущий режим блокировки файловых дескрипторов.
    bool                m_is_blocking_mode;

protected:
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
    /// \~english @return An integer, typically 0, though its specific meaning here is not defined beyond being a static initializer guard.
    /// \~russian @return Целое число, обычно 0, хотя его конкретное значение здесь не определено, кроме как статический инициализатор-страж.
    static int initSignal();
};


template<typename T>
T CStreamPort::assertOK(T const &result, std::string const &message) {
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


} // _unix
} // platform
} // implementation
} // io
} // iridium


#endif // UNIX_PLATFORM


#endif // HEADER_STREAM_PORT_3353B98B_8ECE_48F8_8627_EE37A8AB711C
