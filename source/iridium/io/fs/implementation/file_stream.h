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


namespace iridium::io::fs::implementation {


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

    void initialize() override;
    void finalize() override;

    TMapHandleTypeIdent
        getHandles() const override;
    URI::TSharedPtr
        getURI() const override;
    TFileStatus
        getStatus() const override;

    Buffer::TSharedPtr
        read(size_t const &size) override;
    size_t
        write(Buffer::TSharedPtr const &buffer) override;
    void
        flush() override;

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


} // iridium::io::fs::implementation


//DEFINE_ENUM_CONVERT(iridium::io::fs::implementation::CFileStream::TOpenMode)


#endif // HEADER_FILE_STREAM_6A05934B_514A_4E85_97E7_69A721D4ED29
