// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F
#define HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F


#include "iridium/smart_ptr.h"
#include "iridium/logging/event.h"

#include "sink.h"


namespace iridium {
namespace logging {
namespace implementation {

/// \~english @brief A concrete sink implementation for logging messages to the standard console/terminal.
///     This sink writes formatted log messages to `stdout` or `stderr` (typically `stdout`).
/// \~russian @brief Конкретная реализация приемника для вывода сообщений журнала в стандартную консоль/терминал.
///     Этот приемник записывает отформатированные сообщения журнала в `stdout` или `stderr` (обычно `stdout`).
class CSinkConsole: public ISink, pattern::NonCopyable {
public:
    /// \~english @brief Defines standard implementation helpers for `CSinkConsole`, such as smart pointer typedefs and a static `create` method.
    /// \~russian @brief Определяет стандартные вспомогательные средства реализации для `CSinkConsole`, такие как typedef-ы умных указателей и статический метод `create`.
    DEFINE_IMPLEMENTATION(CSinkConsole)
    /// \~english @brief Constructs a console sink with a specific minimum logging level.
    /// \~russian @brief Конструирует консольный приемник с определенным минимальным уровнем логирования.
    /// \~english @param level The minimum severity level of events that this sink will process. Events below this level will be ignored by this sink.
    /// \~russian @param level Минимальный уровень серьезности событий, которые будет обрабатывать этот приемник. События ниже этого уровня будут проигнорированы этим приемником.
    explicit CSinkConsole(TEvent::TLevel const &level);

    /// \~english @brief Initializes the console sink. This is currently a no-operation for this sink type.
    /// \~russian @brief Инициализирует консольный приемник. В настоящее время это операция без действия для данного типа приемника.
    void initialize() override;
    /// \~english @brief Finalizes the console sink. This is currently a no-operation for this sink type.
    /// \~russian @brief Завершает работу консольного приемника. В настоящее время это операция без действия для данного типа приемника.
    void finalize() override;
    /// \~english @brief Logs a single event to the console if its level is sufficient.
    ///     The event is formatted into a string line before being outputted.
    /// \~russian @brief Логирует одно событие в консоль, если его уровень достаточен.
    ///     Событие форматируется в строковую запись перед выводом.
    /// \~english @param event The log event to be processed.
    /// \~russian @param event Событие журнала для обработки.
    void log(TEvent::TConstSharedPtr const &event) override;
    /// \~english @brief Flushes any buffered messages. For the console sink, this typically involves flushing the standard output stream.
    /// \~russian @brief Сбрасывает все буферизованные сообщения. Для консольного приемника это обычно включает сброс стандартного потока вывода.
    void flush() override;

private:
    /// \~english @brief The minimum severity level that this sink will log. Events with a level below this will be ignored.
    /// \~russian @brief Минимальный уровень серьезности, который будет логировать этот приемник. События с уровнем ниже этого будут проигнорированы.
    TEvent::TLevel m_level;
};


} // implementation
} // logging
} // iridium


#endif // HEADER_SINK_CONSOLE_6E4AF8A2_1104_46D6_AC9B_29A482C3FC6F
