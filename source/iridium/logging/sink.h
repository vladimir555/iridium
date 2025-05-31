// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SINK_3A3CF594_E847_4869_B666_7DF6E46ADCCA
#define HEADER_SINK_3A3CF594_E847_4869_B666_7DF6E46ADCCA


#include "iridium/smart_ptr.h"
#include "iridium/pattern/initializable.h"

#include "iridium/enum.h"

#include "event.h"

#include <atomic>


namespace iridium {
namespace logging {

/// \~english @brief Interface for log event sinks, also known as appenders.
///     Defines the contract for classes that consume log events and output them
///     to a specific destination (e.g., console, file, network).
/// \~russian @brief Интерфейс для приемников событий журнала, также известных как аппендеры.
///     Определяет контракт для классов, которые потребляют события журнала и выводят их
///     в определенное место назначения (например, консоль, файл, сеть).
class ISink : public pattern::IInitializable {
public:
    /// \~english @brief Defines standard smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.) for the ISink interface.
    /// \~russian @brief Определяет стандартные typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.) для интерфейса ISink.
    DEFINE_SMART_PTR(ISink)
    /// \~english @brief Enumerates the general types of log sinks.
    /// \~russian @brief Перечисляет общие типы приемников логов.
    DEFINE_ENUM(
        TType,
        /// \~english @brief File sink type. Logs events to a file.
        /// \~russian @brief Тип файлового приемника. Логирует события в файл.
        FILE,
        /// \~english @brief Console sink type. Logs events to the standard console output/error.
        /// \~russian @brief Тип консольного приемника. Логирует события в стандартный консольный вывод/ошибку.
        CONSOLE
    )
    /// \~english @brief Virtual destructor to ensure proper cleanup of derived class resources.
    /// \~russian @brief Виртуальный деструктор для обеспечения корректного освобождения ресурсов производного класса.
    virtual ~ISink() = default;
    /// \~english @brief Processes and logs a single log event. This is a pure virtual function that must be implemented by derived sink classes.
    /// \~russian @brief Обрабатывает и логирует одно событие журнала. Это чисто виртуальная функция, которая должна быть реализована производными классами приемников.
    /// \~english @param e A constant shared pointer to the log event to be processed.
    /// \~russian @param e Константный разделяемый указатель на событие журнала для обработки.
    virtual void log(TEvent::TConstSharedPtr const &e) = 0;
    /// \~english @brief Flushes any buffered log messages to the underlying storage or output stream.
    ///     This is a pure virtual function that must be implemented by derived sink classes.
    /// \~russian @brief Сбрасывает все буферизованные сообщения журнала в базовое хранилище или выходной поток.
    ///     Это чисто виртуальная функция, которая должна быть реализована производными классами приемников.
    virtual void flush() = 0;
    /// \~english @brief Formats a log event into a standardized string line.
    ///     This static utility method can be used by sink implementations to create a consistent log message format.
    /// \~russian @brief Форматирует событие журнала в стандартизированную строковую запись.
    ///     Этот статический служебный метод может использоваться реализациями приемников для создания согласованного формата сообщений журнала.
    /// \~english @param e A constant shared pointer to the log event to be formatted.
    /// \~russian @param e Константный разделяемый указатель на событие журнала для форматирования.
    /// \~english @return A string representing the formatted log line.
    /// \~russian @return Строка, представляющая отформатированную запись журнала.
    static std::string makeLine(TEvent::TConstSharedPtr const &e);
    /// \~english @brief Global atomic flag indicating whether timestamps in log messages should be in GMT/UTC.
    ///     If true, GMT/UTC time is used; otherwise, local time is used. This flag affects the output of `makeLine`.
    /// \~russian @brief Глобальный атомарный флаг, указывающий, должны ли временные метки в сообщениях журнала быть в GMT/UTC.
    ///     Если true, используется время GMT/UTC; в противном случае используется локальное время. Этот флаг влияет на вывод `makeLine`.
    static std::atomic<bool> is_gmt_time;
};


} // logging
} // iridium


#endif // HEADER_SINK_3A3CF594_E847_4869_B666_7DF6E46ADCCA
