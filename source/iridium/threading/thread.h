// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `IThread` interface and related threading utility functions.
///     This includes the core interface for thread objects and helper functions like sleep and getting thread IDs.
/// \~russian @file
/// @brief Определяет интерфейс `IThread` и связанные служебные функции для работы с потоками.
///     Включает основной интерфейс для объектов потоков и вспомогательные функции, такие как `sleep` и получение идентификаторов потоков.

#ifndef HEADER_THREAD_EC50D1AB_7469_400A_90D9_4977B01B0A71
#define HEADER_THREAD_EC50D1AB_7469_400A_90D9_4977B01B0A71


#include "iridium/pattern/initializable.h"
#include "iridium/smart_ptr.h"

#include <string>
#include <chrono>
#include <thread>


namespace iridium::threading {


class IThread:
    public pattern::IInitializable
{
public:
    /// \~english @brief Defines standard smart pointer typedefs for `IThread` (e.g., `TSharedPtr`, `TUniquePtr`).
    /// \~russian @brief Определяет стандартные typedef-ы умных указателей для `IThread` (например, `TSharedPtr`, `TUniquePtr`).
    DEFINE_SMART_PTR(IThread)
    virtual ~IThread() = default;

    /// for std::map caching
    virtual bool operator < (IThread const &thread) const = 0;
    virtual std::string getName() const = 0;

    static std::string const &getNameStatic();
protected:
    static void setNameStatic(std::string const &name);
private:
    static thread_local std::string thread_name_static;
};


/// \~english @brief Pauses the execution of the current thread for a specified duration.
/// \~russian @brief Приостанавливает выполнение текущего потока на указанную продолжительность.
/// \~english @param milliseconds The duration to sleep, in milliseconds.
/// \~russian @param milliseconds Продолжительность сна в миллисекундах.
void sleep(int const &milliseconds);

/// \~english @brief Gets a string representation of the current thread's unique ID.
/// \~russian @brief Получает строковое представление уникального идентификатора текущего потока.
/// \~english @return A string representing the thread ID.
/// \~russian @return Строка, представляющая идентификатор потока.
std::string getThreadID();


} // namespace iridium::threading


#endif // HEADER_THREAD_EC50D1AB_7469_400A_90D9_4977B01B0A71
