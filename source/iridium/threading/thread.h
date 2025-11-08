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


// No specific Doxygen for iridium namespace itself here, usually documented at a higher level.
namespace iridium {
namespace threading {


/// \~english @brief Interface for a thread object.
///     Provides a base for thread implementations, including initialization, naming, and comparison.
///     Inherits from `IInitializable` suggesting threads might have explicit setup/teardown phases.
/// \~russian @brief Интерфейс для объекта потока.
///     Предоставляет базу для реализаций потоков, включая инициализацию, именование и сравнение.
///     Наследует от `IInitializable`, предполагая, что потоки могут иметь явные фазы настройки/завершения.
class IThread: 
    public pattern::IInitializable
{
public:
    /// \~english @brief Defines standard smart pointer typedefs for `IThread` (e.g., `TSharedPtr`, `TUniquePtr`).
    /// \~russian @brief Определяет стандартные typedef-ы умных указателей для `IThread` (например, `TSharedPtr`, `TUniquePtr`).
    DEFINE_SMART_PTR(IThread)

    /// \~english @brief Virtual destructor for proper cleanup of derived thread classes.
    /// \~russian @brief Виртуальный деструктор для корректной очистки производных классов потоков.
    virtual ~IThread() = default;

    /// \~english @brief Comparison operator for ordering `IThread` objects, e.g., for use in sorted containers.
    ///     Typically compares based on thread ID or another unique identifier.
    /// \~russian @brief Оператор сравнения для упорядочивания объектов `IThread`, например, для использования в отсортированных контейнерах.
    ///     Обычно сравнивает на основе идентификатора потока или другого уникального идентификатора.
    /// \~english @param thread The other `IThread` object to compare against.
    /// \~russian @param thread Другой объект `IThread` для сравнения.
    /// \~english @return True if this thread should be ordered before the other thread, false otherwise.
    /// \~russian @return True, если этот поток должен быть упорядочен перед другим потоком, иначе false.
    virtual bool operator < (IThread const &thread) const = 0;

    /// \~english @brief Gets the name of this specific thread instance.
    ///     The name might be assigned during thread creation or initialization.
    /// \~russian @brief Получает имя этого конкретного экземпляра потока.
    ///     Имя может быть присвоено во время создания или инициализации потока.
    /// \~english @return A string representing the name of the thread.
    /// \~russian @return Строка, представляющая имя потока.
    virtual std::string getName() const = 0;

    /// \~english @brief Gets the name of the currently executing thread (statically, from thread-local storage).
    /// \~russian @brief Получает имя текущего выполняющегося потока (статически, из локального хранилища потока).
    /// \~english @return A const reference to a string representing the name of the current thread.
    /// \~russian @return Константная ссылка на строку, представляющую имя текущего потока.
    static std::string const &getNameStatic();
protected:
    /// \~english @brief Sets the name for the currently executing thread (statically, in thread-local storage).
    ///     This is typically called by the thread itself during its initialization.
    /// \~russian @brief Устанавливает имя для текущего выполняющегося потока (статически, в локальном хранилище потока).
    ///     Обычно вызывается самим потоком во время его инициализации.
    /// \~english @param name The name to assign to the current thread.
    /// \~russian @param name Имя, присваиваемое текущему потоку.
    static void setNameStatic(std::string const &name);
private:
    /// \~english @brief Thread-local storage for the name of the current thread.
    /// \~russian @brief Локальное хранилище потока для имени текущего потока.
    static thread_local std::string thread_name_static;
    
    
//    static std::__thread_id const thread_main_id;
//    static std::__thread_id initMainThreadID();
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


} // threading
} // iridium


#endif // HEADER_THREAD_EC50D1AB_7469_400A_90D9_4977B01B0A71
