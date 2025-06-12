// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `CSinkFile` class, a concrete implementation of `ISink` for logging to files,
///     including support for log rotation.
/// \~russian @file
/// @brief Определяет класс `CSinkFile`, конкретную реализацию `ISink` для логирования в файлы,
///     включая поддержку ротации логов.

#ifndef HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573
#define HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573


#include "iridium/threading/worker.h"
#include "iridium/io/fs/file_stream.h"

#include "sink.h"


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
/// \~english @brief Contains classes, structures, and enumerations related to the logging framework.
/// \~russian @brief Содержит классы, структуры и перечисления, связанные с фреймворком логирования.
namespace logging {
/// \~english @brief Contains concrete implementations of the interfaces defined in the parent `threading` namespace.
/// \~russian @brief Содержит конкретные реализации интерфейсов, определенных в родительском пространстве имен `threading`.
namespace implementation {

/// \~english @brief A concrete sink implementation for logging messages to a file, with support for file rotation.
///     This sink writes formatted log messages to a specified file path and can rotate the
///     log file based on strategies like daily rotation or rotation on each initialization.
/// \~russian @brief Конкретная реализация приемника для вывода сообщений журнала в файл, с поддержкой ротации файлов.
///     Этот приемник записывает отформатированные сообщения журнала по указанному пути к файлу и может
///     ротировать лог-файл на основе стратегий, таких как ежедневная ротация или ротация при каждой инициализации.
class CSinkFile: public ISink {
public:
    /// \~english @brief Enumerates the available strategies for log file rotation.
    /// \~russian @brief Перечисляет доступные стратегии ротации лог-файлов.
    DEFINE_ENUM(
        TRotation,
        /// \~english @brief Rotate the log file on a daily basis. A new file with a date-stamped name will be created each day.
        /// \~russian @brief Ротировать лог-файл ежедневно. Новый файл с именем, содержащим дату, будет создаваться каждый день.
        DAILY,
        /// \~english @brief Rotate the log file each time the sink is initialized (e.g., typically on application startup).
        ///     A new file, often with a timestamp in its name, is created.
        /// \~russian @brief Ротировать лог-файл при каждой инициализации приемника (например, обычно при запуске приложения).
        ///     Создается новый файл, часто с временной меткой в имени.
        TIMELY
    );

    /// \~english @brief Defines standard implementation helpers for `CSinkFile`, such as smart pointer typedefs and a static `create` method.
    /// \~russian @brief Определяет стандартные вспомогательные средства реализации для `CSinkFile`, такие как typedef-ы умных указателей и статический метод `create`.
    DEFINE_IMPLEMENTATION(CSinkFile)
    /// \~english @brief Constructs a file sink with a specific minimum logging level, file path, and rotation strategy.
    /// \~russian @brief Конструирует файловый приемник с определенным минимальным уровнем логирования, путем к файлу и стратегией ротации.
    /// \~english @param level The minimum severity level of events that this sink will process.
    /// \~russian @param level Минимальный уровень серьезности событий, которые будет обрабатывать этот приемник.
    /// \~english @param path The base path for the log file. Depending on the rotation strategy, date/time stamps might be appended to this path.
    /// \~russian @param path Базовый путь для лог-файла. В зависимости от стратегии ротации, к этому пути могут добавляться метки даты/времени.
    /// \~english @param rotation The file rotation strategy to use. Defaults to `TRotation::UNKNOWN` (no rotation or rotation handled by other means).
    /// \~russian @param rotation Используемая стратегия ротации файлов. По умолчанию `TRotation::UNKNOWN` (без ротации или ротация обрабатывается другими средствами).
    CSinkFile(TEvent::TLevel const &level, std::string const &path, TRotation const &rotation = TRotation::UNKNOWN);

    /// \~english @brief Initializes the file sink. This involves opening/creating the log file according to the path and rotation strategy.
    /// \~russian @brief Инициализирует файловый приемник. Это включает открытие/создание лог-файла в соответствии с путем и стратегией ротации.
    void initialize() override;
    /// \~english @brief Finalizes the file sink, ensuring any buffered messages are flushed and the log file is closed.
    /// \~russian @brief Завершает работу файлового приемника, обеспечивая сброс всех буферизованных сообщений и закрытие лог-файла.
    void finalize() override;
    /// \~english @brief Logs a single event to the file if its level is sufficient and the file is open.
    ///     The event is formatted into a string line before being written. May trigger file rotation if conditions are met.
    /// \~russian @brief Логирует одно событие в файл, если его уровень достаточен и файл открыт.
    ///     Событие форматируется в строковую запись перед записью. Может вызвать ротацию файла, если выполнены условия.
    /// \~english @param event The log event to be processed.
    /// \~russian @param event Событие журнала для обработки.
    void log(TEvent::TConstSharedPtr const &event) override;
    /// \~english @brief Flushes any buffered messages to the log file.
    /// \~russian @brief Сбрасывает все буферизованные сообщения в лог-файл.
    void flush() override;

private:
    /// \~english @brief The minimum severity level that this sink will log.
    /// \~russian @brief Минимальный уровень серьезности, который будет логировать этот приемник.
    TEvent::TLevel  m_level;
    /// \~english @brief The current full path to the active log file. This path might be modified by rotation logic (e.g., by appending date/time).
    /// \~russian @brief Текущий полный путь к активному лог-файлу. Этот путь может быть изменен логикой ротации (например, путем добавления даты/времени).
    std::string     m_path;
    /// \~english @brief The original base path for the log file as provided in the constructor. Used as a template for generating rotated file names.
    /// \~russian @brief Исходный базовый путь для лог-файла, указанный в конструкторе. Используется как шаблон для генерации имен ротированных файлов.
    std::string     m_path_original;
    /// \~english @brief The configured file rotation strategy for this sink.
    /// \~russian @brief Сконфигурированная стратегия ротации файлов для этого приемника.
    TRotation       m_rotation;
    /// \~english @brief Timestamp of when the current log file was last initialized or rotated. Used to determine if rotation is needed (e.g., for daily rotation).
    /// \~russian @brief Временная метка последней инициализации или ротации текущего лог-файла. Используется для определения необходимости ротации (например, для ежедневной ротации).
    std::chrono::system_clock::time_point
                    m_last_initialization_time;
    /// \~english @brief Shared pointer to the file stream writer used to write to the log file.
    /// \~russian @brief Разделяемый указатель на объект записи в файловый поток, используемый для записи в лог-файл.
    io::fs::IFileStreamWriter::TSharedPtr
                    m_file_writer;
};


} // implementation
} // logging
} // iridium


#endif // HEADER_SINK_FILE_EE36E77A_D49D_46A5_9728_AD2D5D5B6573
