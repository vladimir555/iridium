#ifndef HEADER_STREAM_PORT_070ECA4F_6B07_4564_A97F_165282BB6685
#define HEADER_STREAM_PORT_070ECA4F_6B07_4564_A97F_165282BB6685


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/stream.h"
#include "iridium/io/uri.h"
#include "iridium/pattern/non_copyable.h"

//#include <wsa>


namespace iridium {
namespace io {
namespace implementation {
namespace platform {


/// \~english @brief Windows-specific stream port implementation.
///     This class is intended to provide a concrete implementation of `IStreamPort` for Windows,
///     likely using Windows-specific APIs like `HANDLE` for I/O operations.
///     It is non-copyable. The specifics of its behavior (e.g., for sockets, files, or pipes)
///     would be determined by its concrete implementation details in a corresponding .cpp file or derived classes.
/// \~russian @brief Реализация потокового порта для Windows.
///     Этот класс предназначен для предоставления конкретной реализации `IStreamPort` для Windows,
///     вероятно, используя специфичные для Windows API, такие как `HANDLE`, для операций ввода-вывода.
///     Класс является некопируемым. Особенности его поведения (например, для сокетов, файлов или каналов)
///     будут определяться деталями его конкретной реализации в соответствующем .cpp файле или производных классах.
class CStreamPort: virtual public IStreamPort, public pattern::NonCopyable {
 public:
	DEFINE_IMPLEMENTATION(CStreamPort)
	/// \~english @brief Constructs a `CStreamPort` with a given URI.
	///     The URI is stored for later use, e.g., to establish a connection in `initialize()`.
	/// \~russian @brief Конструирует `CStreamPort` с заданным URI.
	///     URI сохраняется для последующего использования, например, для установления соединения в `initialize()`.
	/// \~english @param uri The URI associated with this stream port (e.g., "tcp://host:port", "file:///path").
	/// \~russian @param uri URI, связанный с этим потоковым портом (например, "tcp://host:port", "file:///path").
	CStreamPort(URI const &uri);

	/// \~english @brief Initializes the stream port.
	///     This method should perform any setup required to make the stream usable,
	///     such as opening a file, creating a socket, or connecting to a remote host.
	///     The actual operations depend on the concrete implementation.
	/// \~russian @brief Инициализирует потоковый порт.
	///     Этот метод должен выполнять любую настройку, необходимую для того, чтобы сделать поток пригодным для использования,
	///     например, открытие файла, создание сокета или подключение к удаленному хосту.
	///     Фактические операции зависят от конкретной реализации.
	void initialize() override;

    /// \~english @brief Finalizes the stream port.
    ///     This method should perform any cleanup required, such as closing handles or releasing resources.
    /// \~russian @brief Завершает работу потокового порта.
    ///     Этот метод должен выполнять любую необходимую очистку, такую как закрытие дескрипторов или освобождение ресурсов.
    void finalize() override;

	/// \~english @brief Gets the list of underlying OS handles (e.g., `HANDLE` on Windows) for this stream.
	/// \~russian @brief Получает список базовых дескрипторов ОС (например, `HANDLE` в Windows) для этого потока.
	/// \~english @return A list of `uintptr_t` representing the OS handles. The content depends on the specific stream type and state.
	/// \~russian @return Список `uintptr_t`, представляющий дескрипторы ОС. Содержимое зависит от конкретного типа и состояния потока.
	std::list<uintptr_t> getHandles() const override;

	/// \~english @brief Reads data from the stream into a new buffer.
	/// \~russian @brief Читает данные из потока в новый буфер.
	/// \~english @param size The maximum number of bytes to read. Defaults to `DEFAULT_BUFFER_SIZE`.
	/// \~russian @param size Максимальное количество байт для чтения. По умолчанию `DEFAULT_BUFFER_SIZE`.
	/// \~english @return A shared pointer to a buffer containing the data read. Behavior on EOF or error depends on implementation.
	/// \~russian @return Умный указатель на буфер, содержащий прочитанные данные. Поведение при EOF или ошибке зависит от реализации.
	Buffer::TSharedPtr read(size_t const &size = DEFAULT_BUFFER_SIZE) override;

    /// \~english @brief Writes data from the provided buffer to the stream.
    /// \~russian @brief Записывает данные из предоставленного буфера в поток.
    /// \~english @param buffer The buffer containing data to write.
    /// \~russian @param buffer Буфер, содержащий данные для записи.
    /// \~english @return The number of bytes actually written. Behavior on error depends on implementation.
    /// \~russian @return Количество фактически записанных байт. Поведение при ошибке зависит от реализации.
    size_t write(Buffer::TSharedPtr const &buffer) override;
private:
    // TODO: Add Doxygen comments for members when they are defined and used.
    // URI::TSharedPtr m_uri; // Expected member based on constructor
    // HANDLE m_handle_reader; // Example of a possible member for reading
    // HANDLE m_handle_writer; // Example of a possible member for writing
	//HANDLE m_handle; // This was the original commented-out member
};


} // platform
} // implementation
} // io
} // iridium


#endif  // WINDOWS_PLATFORM


#endif // HEADER_STREAM_PORT_070ECA4F_6B07_4564_A97F_165282BB6685
