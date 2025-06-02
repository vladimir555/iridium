// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines the `CFileStream` class, a concrete implementation for file-based I/O streams.
/// \~russian @brief Определяет класс `CFileStream`, конкретную реализацию для файловых потоков ввода-вывода.
#ifndef HEADER_FILE_STREAM_6A05934B_514A_4E85_97E7_69A721D4ED29
#define HEADER_FILE_STREAM_6A05934B_514A_4E85_97E7_69A721D4ED29


#include "iridium/enum.h"                 // For DEFINE_ENUM
#include "iridium/io/fs/file_stream.h"    // For IFileStreamReader, IFileStreamWriter, TFileStatus
#include "iridium/pattern/non_copyable.h" // For NonCopyable
#include "iridium/io/uri.h"               // For URI::TSharedPtr
#include "iridium/io/buffer.h"            // For Buffer::TSharedPtr
#include "iridium/smart_ptr.h"          // For TSharedPtr (though often included by other iridium headers)


#include <string>
#include <stdio.h> // For ::FILE, fopen, fclose, fread, fwrite, fflush, fileno, fseek, ftell etc.
#include <list>    // For std::list (used in IStream::getHandles)
#include <vector>  // For IStream (though not directly, Buffer inherits from it)


namespace iridium {
namespace io {
namespace fs {
/// \~english @brief Namespace for internal implementation details of file system functionalities.
/// \~russian @brief Пространство имен для внутренних деталей реализации функциональностей файловой системы.
namespace implementation {


/// \~english @brief Concrete implementation of a file stream providing read, write, and seeking capabilities.
///     This class implements `IFileStreamReader` and `IFileStreamWriter` (and by extension `IFileStream` and `IStream`).
///     It uses C standard I/O functions (`FILE*`) for underlying file operations.
///     The class is non-copyable.
/// \~russian @brief Конкретная реализация файлового потока, предоставляющая возможности чтения, записи и позиционирования.
///     Этот класс реализует `IFileStreamReader` и `IFileStreamWriter` (и, соответственно, `IFileStream` и `IStream`).
///     Он использует стандартные функции ввода-вывода C (`FILE*`) для базовых файловых операций.
///     Класс является некопируемым.
class CFileStream:
    public virtual pattern::NonCopyable,    // Ensures instances of CFileStream are not copyable
    public virtual IFileStreamReader,       // Implements readable file stream interface
    public virtual IFileStreamWriter      // Implements writable file stream interface
{
public:
    /// \~english @brief Enumerates the modes in which a file can be opened.
    /// \~russian @brief Перечисляет режимы, в которых файл может быть открыт.
    DEFINE_ENUM(
        TOpenMode,
        /// \~english @brief Open for reading. The file must exist.
        /// \~russian @brief Открыть для чтения. Файл должен существовать.
        READ,
        /// \~english @brief Open for writing. If the file exists, its contents are discarded (truncated). If it does not exist, it is created.
        /// \~russian @brief Открыть для записи. Если файл существует, его содержимое удаляется (обрезается). Если не существует, создается.
        WRITE,
        /// \~english @brief Open for writing. Creates the file if it does not exist. If the file exists, its contents are discarded (truncated).
        ///     This mode is typically equivalent to `fopen` mode `w` or `w+b`.
        /// \~russian @brief Открыть для записи. Создает файл, если он не существует. Если файл существует, его содержимое удаляется (обрезается).
        ///     Этот режим обычно эквивалентен режиму `fopen` `w` или `w+b`.
        REWRITE
    )

    /// \~english @brief Virtual destructor. Ensures the file is closed when the `CFileStream` object is destroyed.
    ///     Calls `finalize()` to perform cleanup.
    /// \~russian @brief Виртуальный деструктор. Гарантирует закрытие файла при уничтожении объекта `CFileStream`.
    ///     Вызывает `finalize()` для выполнения очистки.
    virtual ~CFileStream();

    /// \~english @brief Initializes the file stream by opening the file.
    ///     This method is part of the `IInitializable` interface. It uses the filename and open mode
    ///     provided during construction to open the file using `fopen`.
    /// \~russian @brief Инициализирует файловый поток, открывая файл.
    ///     Этот метод является частью интерфейса `IInitializable`. Он использует имя файла и режим открытия,
    ///     предоставленные при конструировании, для открытия файла с помощью `fopen`.
    /// \~english @throws iridium::io::fs::Exception (or a derived class) if the file cannot be opened based on the specified mode.
    /// \~russian @throws iridium::io::fs::Exception (или производный класс), если файл не может быть открыт в указанном режиме.
    void initialize()   override;

    /// \~english @brief Finalizes the file stream by closing the file if it is open.
    ///     This method is part of the `IInitializable` interface. It calls `fclose` on the internal file handle.
    /// \~russian @brief Финализирует файловый поток, закрывая файл, если он открыт.
    ///     Этот метод является частью интерфейса `IInitializable`. Он вызывает `fclose` для внутреннего дескриптора файла.
    void finalize()     override;

    /// \~english @brief Gets the underlying OS file handle(s) associated with this stream.
    ///     For `CFileStream`, this typically returns the file descriptor number obtained via `fileno(m_file)`.
    /// \~russian @brief Получает базовый(е) дескриптор(ы) ОС, связанный(е) с этим потоком.
    ///     Для `CFileStream` это обычно возвращает номер файлового дескриптора, полученный через `fileno(m_file)`.
    /// \~english @return A list containing the file descriptor number if the file is open; otherwise, an empty list.
    /// \~russian @return Список, содержащий номер файлового дескриптора, если файл открыт; в противном случае — пустой список.
    std::list<uintptr_t>    getHandles()    const override;

    /// \~english @brief Gets the URI associated with this file stream.
    ///     The URI will typically be a "file://" scheme URI.
    /// \~russian @brief Получает URI, связанный с этим файловым потоком.
    ///     URI обычно будет иметь схему "file://".
    /// \~english @return A shared pointer to a `URI` object representing the file path.
    /// \~russian @return Умный указатель на объект `URI`, представляющий путь к файлу.
    URI::TSharedPtr         getURI()        const override;

    /// \~english @brief Retrieves the status information (size and modification time) of the opened file.
    /// \~russian @brief Получает информацию о состоянии (размер и время изменения) открытого файла.
    /// \~english @return A `TFileStatus` structure containing the file's status.
    /// \~russian @return Структура `TFileStatus`, содержащая состояние файла.
    /// \~english @throws iridium::io::fs::Exception (or derived) if the file is not open or status cannot be retrieved (e.g., `fstat` fails).
    /// \~russian @throws iridium::io::fs::Exception (или производный), если файл не открыт или статус не может быть получен (например, сбой `fstat`).
    TFileStatus             getStatus()     const override;

    /// \~english @brief Reads up to `size` bytes of data from the file stream into a buffer.
    ///     Implements the `IStreamReader::read` method.
    /// \~russian @brief Читает до `size` байт данных из файлового потока в буфер.
    ///     Реализует метод `IStreamReader::read`.
    /// \~english @param size The maximum number of bytes to read.
    /// \~russian @param size Максимальное количество байт для чтения.
    /// \~english @return A shared pointer to a `Buffer` containing the data read.
    ///     Returns `nullptr` on EOF if no bytes were read, or if an error occurs.
    ///     Returns an empty buffer if `size` is 0 and no prior accumulated data (not typical for file streams).
    /// \~russian @return Умный указатель на `Buffer`, содержащий прочитанные данные.
    ///     Возвращает `nullptr` при EOF, если не было прочитано ни одного байта, или в случае ошибки.
    ///     Возвращает пустой буфер, если `size` равен 0 и нет предварительно накопленных данных (нетипично для файловых потоков).
    /// \~english @throws iridium::io::fs::Exception (or derived) on read errors if not returning nullptr.
    /// \~russian @throws iridium::io::fs::Exception (или производный) при ошибках чтения, если не возвращается nullptr.
    Buffer::TSharedPtr      read(size_t const &size) override;

    /// \~english @brief Writes data from a buffer to the file stream.
    ///     Implements the `IStreamWriter::write` method.
    /// \~russian @brief Записывает данные из буфера в файловый поток.
    ///     Реализует метод `IStreamWriter::write`.
    /// \~english @param buffer A shared pointer to a `Buffer` containing the data to write.
    /// \~russian @param buffer Умный указатель на `Buffer`, содержащий данные для записи.
    /// \~english @return The number of bytes actually written to the file.
    /// \~russian @return Количество байт, фактически записанных в файл.
    /// \~english @throws iridium::io::fs::Exception (or derived) on write errors.
    /// \~russian @throws iridium::io::fs::Exception (или производный) при ошибках записи.
    size_t                  write(Buffer::TSharedPtr const &buffer) override;

    /// \~english @brief Flushes any buffered data associated with the file stream to the underlying file.
    ///     Calls `fflush` on the internal `FILE*` handle.
    /// \~russian @brief Сбрасывает все буферизованные данные, связанные с файловым потоком, в базовый файл.
    ///     Вызывает `fflush` для внутреннего дескриптора `FILE*`.
    /// \~english @throws iridium::io::fs::Exception (or derived) if the flush operation fails.
    /// \~russian @throws iridium::io::fs::Exception (или производный), если операция сброса не удалась.
    void                    flush() override;

protected:
    /// \~english @brief Protected constructor to set up the file stream parameters.
    ///     This constructor stores the file name and open mode. The actual file opening
    ///     is deferred until the `initialize()` method is called.
    /// \~russian @brief Защищенный конструктор для настройки параметров файлового потока.
    ///     Этот конструктор сохраняет имя файла и режим открытия. Фактическое открытие файла
    ///     откладывается до вызова метода `initialize()`.
    /// \~english @param file_name The path to the file that will be associated with this stream.
    /// \~russian @param file_name Путь к файлу, который будет связан с этим потоком.
    /// \~english @param open_mode The mode (Read, Write, Rewrite) to be used when `initialize()` is called to open the file.
    /// \~russian @param open_mode Режим (Read, Write, Rewrite), который будет использоваться при вызове `initialize()` для открытия файла.
    explicit CFileStream(std::string const &file_name, TOpenMode const &open_mode);

private:
    /// \~english @brief The path to the file being managed by this stream.
    /// \~russian @brief Путь к файлу, управляемому этим потоком.
    std::string     m_file_name;

    /// \~english @brief The C standard library file handle (`FILE*`).
    ///     This is `nullptr` if the file is not open.
    /// \~russian @brief Дескриптор файла стандартной библиотеки C (`FILE*`).
    ///     Равен `nullptr`, если файл не открыт.
    ::FILE         *m_file = nullptr;

    /// \~english @brief The mode in which the file was opened (e.g., Read, Write).
    /// \~russian @brief Режим, в котором был открыт файл (например, Read, Write).
    TOpenMode       m_open_mode;

    /// \~english @brief A URI representation of the file path, constructed from `m_file_name`.
    /// \~russian @brief URI-представление пути к файлу, сконструированное из `m_file_name`.
    URI::TSharedPtr m_uri;
};


} // implementation
} // fs
} // io
} // iridium


#endif // HEADER_FILE_STREAM_6A05934B_514A_4E85_97E7_69A721D4ED29
