// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_MULTIPLEXER_CD0A061D_A439_4770_B4FD_89D8393DBA1E
#define HEADER_MULTIPLEXER_CD0A061D_A439_4770_B4FD_89D8393DBA1E


#include "iridium/pattern/initializable.h"

#include "event.h"

#include <chrono>


namespace iridium::io {


/// \~english @brief Interface for an I/O multiplexer.
/// \~english @details An I/O multiplexer monitors multiple I/O streams (e.g., sockets, file descriptors)
/// \~english and waits for one or more of them to become ready for I/O operations (like read or write).
/// \~english This is a central component in scalable, non-blocking I/O architectures.
/// \~russian @brief Интерфейс для мультиплексора ввода-вывода.
/// \~russian @details Мультиплексор ввода-вывода отслеживает несколько потоков ввода-вывода (например, сокеты, файловые дескрипторы)
/// \~russian и ожидает, пока один или несколько из них станут готовы для выполнения операций ввода-вывода (например, чтения или записи).
/// \~russian Это центральный компонент в масштабируемых, неблокирующих архитектурах ввода-вывода.
class IMultiplexer: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(IMultiplexer)
    static std::chrono::seconds const DEFAULT_WAITING_TIMEOUT;

    virtual std::list<Event::TSharedPtr> waitEvents() = 0;
    virtual void subscribe  (IStream::TSharedPtr const &stream) = 0;
    virtual void unsubscribe(IStream::TSharedPtr const &stream) = 0;
    virtual void wake(Event::TSharedPtr const &event) = 0;
};


} // namespace iridium::io


#endif // HEADER_MULTIPLEXER_CD0A061D_A439_4770_B4FD_89D8393DBA1E
