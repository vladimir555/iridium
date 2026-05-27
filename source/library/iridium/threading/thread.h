// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_THREAD_EC50D1AB_7469_400A_90D9_4977B01B0A71
#define HEADER_THREAD_EC50D1AB_7469_400A_90D9_4977B01B0A71


#include "iridium/pattern/initializable.h"
#include "iridium/smart_ptr.h"

#include <string>
#include <chrono>
#include <thread>


namespace iridium::threading {


/// \~english @brief Interface for thread management and identification.
/// \~russian @brief Интерфейс для управления потоками и их идентификации.
class IThread:
    public pattern::IInitializable
{
public:
    DEFINE_SMART_PTR(IThread)
    virtual ~IThread() = default;

    /// \~english @brief Comparison operator for std::map caching.
    /// \~russian @brief Оператор сравнения для кэширования в std::map.
    virtual bool operator < (IThread const &thread) const = 0;
    /// \~english @brief Returns the name of the thread.
    /// \~russian @brief Возвращает имя потока.
    virtual std::string getName() const = 0;

    /// \~english @brief Returns the name of the current thread (static).
    /// \~russian @brief Возвращает имя текущего потока (статический метод).
    static std::string const &getNameStatic();
protected:
    /// \~english @brief Sets the name of the current thread (static).
    /// \~russian @brief Устанавливает имя текущего потока (статический метод).
    static void setNameStatic(std::string const &name);
private:
    static thread_local std::string thread_name_static;
};


/// \~english @brief Suspends execution of the current thread for a specified period.
/// \~russian @brief Приостанавливает выполнение текущего потока на заданный период.
void sleep(int const &milliseconds);
/// \~english @brief Returns a string representation of the current thread ID.
/// \~russian @brief Возвращает строковое представление идентификатора текущего потока.
std::string getThreadID();


} // namespace iridium::threading


#endif // HEADER_THREAD_EC50D1AB_7469_400A_90D9_4977B01B0A71
