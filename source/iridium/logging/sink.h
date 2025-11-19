// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `ISink` interface for log event consumers (appenders).
///     This includes the interface itself, an enumeration for sink types, and helper utilities.
/// \~russian @file
/// @brief Определяет интерфейс `ISink` для потребителей событий журнала (аппендеров).
///     Включает сам интерфейс, перечисление типов приемников и вспомогательные утилиты.

#ifndef HEADER_SINK_3A3CF594_E847_4869_B666_7DF6E46ADCCA
#define HEADER_SINK_3A3CF594_E847_4869_B666_7DF6E46ADCCA


#include "iridium/smart_ptr.h"
#include "iridium/pattern/initializable.h"

#include "iridium/enum.h"

#include "event.h"

#include <atomic>


namespace iridium::logging {


class ISink : public pattern::IInitializable {
public:
    /// \~english @brief Defines standard smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.) for the ISink interface.
    /// \~russian @brief Определяет стандартные typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.) для интерфейса ISink.
    DEFINE_SMART_PTR(ISink)
    DEFINE_ENUM(
        TType,
        /// \~english @brief File sink type. Logs events to a file.
        /// \~russian @brief Тип файлового приемника. Логирует события в файл.
        FILE,
        /// \~english @brief Console sink type. Logs events to the standard console output/error.
        /// \~russian @brief Тип консольного приемника. Логирует события в стандартный консольный вывод/ошибку.
        CONSOLE
    )
    virtual ~ISink() = default;
    virtual void log(TEvent::TConstSharedPtr const &e) = 0;
    virtual void flush() = 0;
    static std::string makeLine(TEvent::TConstSharedPtr const &e);
    static std::atomic<bool> is_gmt_time;
};


} // namespace iridium::logging


DEFINE_ENUM_CONVERT(iridium::logging::ISink::TType)


#endif // HEADER_SINK_3A3CF594_E847_4869_B666_7DF6E46ADCCA
