// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_EVENT_PROVIDER_7E3C9A5C_AD20_4D3F_A2C8_3B4BD3E09DA7
#define HEADER_EVENT_PROVIDER_7E3C9A5C_AD20_4D3F_A2C8_3B4BD3E09DA7


#include "iridium/platform.h"


//#ifdef EMSCRIPTEN_PLATFORM


#include "iridium/io/multiplexer.h"


/// \~english @file multiplexer.h
/// \~english @brief Defines a stub implementation of the `IMultiplexer` interface for the Emscripten platform.
/// \~russian @file multiplexer.h
/// \~russian @brief Определяет заглушечную реализацию интерфейса `IMultiplexer` для платформы Emscripten.

namespace iridium {
namespace io {
namespace implementation {
namespace platform {


// todo: rename CMultiplexer
/// \~english @brief Stub implementation of `IMultiplexer` for the Emscripten platform.
///     All methods currently throw `std::runtime_error("not implemented")`.
/// \~russian @brief Заглушечная реализация `IMultiplexer` для платформы Emscripten.
///     Все методы в настоящее время выбрасывают `std::runtime_error("not implemented")`.
class CMultiplexer: public IMultiplexer {
public:
    DEFINE_IMPLEMENTATION(CMultiplexer)
    /// \~english @brief Constructs a `CMultiplexer` (stub).
    /// \~russian @brief Конструирует `CMultiplexer` (заглушка).
    /// \~english @throws std::runtime_error as it's not implemented.
    /// \~russian @throws std::runtime_error, так как не реализовано.
    CMultiplexer(std::chrono::microseconds const &timeout = DEFAULT_WAITING_TIMEOUT) {
        throw std::runtime_error("not implemented");
    }

    /// \~english @brief Initializes the multiplexer (stub).
    /// \~russian @brief Инициализирует мультиплексор (заглушка).
    /// \~english @throws std::runtime_error as it's not implemented.
    /// \~russian @throws std::runtime_error, так как не реализовано.
    void initialize()   override {
        throw std::runtime_error("not implemented");
    }
    /// \~english @brief Finalizes the multiplexer (stub).
    /// \~russian @brief Завершает работу мультиплексора (заглушка).
    /// \~english @throws std::runtime_error as it's not implemented.
    /// \~russian @throws std::runtime_error, так как не реализовано.
    void finalize()     override {
        throw std::runtime_error("not implemented");
    }

    /// \~english @brief Waits for I/O events (stub).
    /// \~russian @brief Ожидает событий ввода-вывода (заглушка).
    /// \~english @throws std::runtime_error as it's not implemented.
    /// \~russian @throws std::runtime_error, так как не реализовано.
    /// \~english @return Does not return, always throws.
    /// \~russian @return Не возвращает значения, всегда выбрасывает исключение.
    std::list<IEvent::TSharedPtr> waitEvents() override {
        throw std::runtime_error("not implemented");
    }

    /// \~english @brief Subscribes a stream (stub).
    /// \~russian @brief Подписывает поток (заглушка).
    /// \~english @throws std::runtime_error as it's not implemented.
    /// \~russian @throws std::runtime_error, так как не реализовано.
    void subscribe  (IStream::TConstSharedPtr const &stream) override {
        throw std::runtime_error("not implemented");
    }
    /// \~english @brief Unsubscribes a stream (stub).
    /// \~russian @brief Отписывает поток (заглушка).
    /// \~english @throws std::runtime_error as it's not implemented.
    /// \~russian @throws std::runtime_error, так как не реализовано.
    void unsubscribe(IStream::TConstSharedPtr const &stream) override {
        throw std::runtime_error("not implemented");
    }
};


} // platform
} // implementation
} // io
} // iridium


//#endif // EMSCRIPTEN_PLATFORM


#endif // HEADER_EVENT_PROVIDER_7E3C9A5C_AD20_4D3F_A2C8_3B4BD3E09DA7
