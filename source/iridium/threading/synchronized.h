// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `Synchronized` class template for creating synchronized blocks of code
///     and related helper macros. This is used to protect shared resources from concurrent access.
/// \~russian @file
/// @brief Определяет шаблон класса `Synchronized` для создания синхронизированных блоков кода
///     и связанные вспомогательные макросы. Используется для защиты общих ресурсов от одновременного доступа.

#ifndef HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
#define HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD


#include "thread.h"
#include "iridium/pattern/non_copyable.h"
#include "iridium/pattern/non_movable.h"
#include "iridium/convertion/convert.h"
#include "iridium/strings.h"
#include "iridium/platform.h"

#include <mutex>
#include <condition_variable>
#include <iostream>
#include <atomic>
#include <cstdio>


using iridium::convertion::convert;


namespace iridium::threading {


// ----- class definition


/// \~english @brief A template class providing synchronization primitives (mutex and condition variable)
///     to protect shared data and coordinate threads.
///     It uses a nested `Locker` class to manage scoped locks.
///     The `is_tracable` template parameter can enable debug printing for lock/unlock events.
/// \~russian @brief Шаблонный класс, предоставляющий примитивы синхронизации (мьютекс и условная переменная)
///     для защиты общих данных и координации потоков.
///     Использует вложенный класс `Locker` для управления блокировками в определенной области видимости.
///     Шаблонный параметр `is_tracable` может включать отладочную печать для событий блокировки/разблокировки.
/// \~english @tparam TMutex The type of mutex to be used (e.g., `std::mutex`, `std::recursive_mutex`).
/// \~russian @tparam TMutex Тип используемого мьютекса (например, `std::mutex`, `std::recursive_mutex`).
/// \~english @tparam is_tracable If true, enables diagnostic printf messages for lock, unlock, and wait operations. Defaults to false.
/// \~russian @tparam is_tracable Если true, включает диагностические сообщения printf для операций блокировки, разблокировки и ожидания. По умолчанию false.
template<typename TMutex, bool const is_tracable = false>
class Synchronized {
protected:
    /// \~english @brief Default constructor. Initializes the synchronization primitives.
    /// \~russian @brief Конструктор по умолчанию. Инициализирует примитивы синхронизации.
    Synchronized() = default;
    /// \~english @brief Default virtual destructor.
    /// \~russian @brief Виртуальный деструктор по умолчанию.
    virtual ~Synchronized() = default;

    /// \~english @brief Interrupts any threads waiting on this `Synchronized` object's condition variable.
    ///     Sets an internal flag to prevent further waits and notifies all waiting threads.
    /// \~russian @brief Прерывает все потоки, ожидающие на условной переменной этого объекта `Synchronized`.
    ///     Устанавливает внутренний флаг для предотвращения дальнейших ожиданий и уведомляет все ожидающие потоки.
    void interrupt();

    /// \~english @brief A scoped locker class that acquires the `Synchronized` object's mutex on construction
    ///     and releases it on destruction. Also provides methods for waiting on a condition variable.
    ///     Inherits from `NonCopyable` and `NonMovable` to prevent accidental misuse.
    /// \~russian @brief Класс для области видимости блокировки, который захватывает мьютекс объекта `Synchronized`
    ///     при создании и освобождает его при уничтожении. Также предоставляет методы для ожидания на условной переменной.
    ///     Наследует от `NonCopyable` и `NonMovable` для предотвращения случайного неправильного использования.
    class Locker:
        public pattern::NonCopyable,
        public pattern::NonMovable
    {
    public:
        /// \~english @brief Constructs a `Locker` and acquires the mutex of the associated `Synchronized` object.
        ///     If `is_tracable` is true on the parent `Synchronized` object, it logs the lock attempt.
        /// \~russian @brief Конструирует `Locker` и захватывает мьютекс связанного объекта `Synchronized`.
        ///     Если `is_tracable` истинно для родительского объекта `Synchronized`, логирует попытку блокировки.
        /// \~english @param s Pointer to the `Synchronized` object whose mutex is to be locked.
        /// \~russian @param s Указатель на объект `Synchronized`, мьютекс которого должен быть заблокирован.
        /// \~english @param file The source file name (used for tracing if `is_tracable` is true).
        /// \~russian @param file Имя исходного файла (используется для трассировки, если `is_tracable` истинно).
        /// \~english @param line The line number in the source file (used for tracing if `is_tracable` is true).
        /// \~russian @param line Номер строки в исходном файле (используется для трассировки, если `is_tracable` истинно).
        Locker(Synchronized const * const s, char const *file, int line);

        /// \~english @brief Destroys the `Locker` and releases the acquired mutex.
        ///     Notifies one waiting thread on the condition variable.
        ///     If `is_tracable` is true on the parent `Synchronized` object, it logs the unlock event.
        /// \~russian @brief Уничтожает `Locker` и освобождает захваченный мьютекс.
        ///     Уведомляет один ожидающий поток на условной переменной.
        ///     Если `is_tracable` истинно для родительского объекта `Synchronized`, логирует событие разблокировки.
       ~Locker();

        /// \~english @brief Waits on the `Synchronized` object's condition variable until notified or interrupted.
        ///     The mutex must be locked by this `Locker` before calling `wait()`.
        /// \~russian @brief Ожидает на условной переменной объекта `Synchronized` до уведомления или прерывания.
        ///     Мьютекс должен быть заблокирован этим `Locker` перед вызовом `wait()`.
        /// \~english @return `true` if the wait was successful and not interrupted, `false` if interrupted.
        /// \~russian @return `true`, если ожидание прошло успешно и не было прервано, `false` в случае прерывания.
        bool wait();

        /// \~english @brief Waits on the `Synchronized` object's condition variable for a specified duration, or until notified or interrupted.
        ///     The mutex must be locked by this `Locker` before calling `wait()`.
        /// \~russian @brief Ожидает на условной переменной объекта `Synchronized` в течение указанного времени, или до уведомления, или прерывания.
        ///     Мьютекс должен быть заблокирован этим `Locker` перед вызовом `wait()`.
        /// \~english @param timeout The maximum duration to wait.
        /// \~russian @param timeout Максимальная продолжительность ожидания.
        /// \~english @return `true` if the wait was successful (not timed out and not interrupted), `false` if timed out or interrupted.
        /// \~russian @return `true`, если ожидание прошло успешно (не по тайм-ауту и не прервано), `false` в случае тайм-аута или прерывания.
        bool wait(std::chrono::nanoseconds const &timeout);
    private:
        /// \~english @brief Pointer to the parent `Synchronized` object.
        /// \~russian @brief Указатель на родительский объект `Synchronized`.
        Synchronized const * const
            m_s;
        /// \~english @brief The unique lock managing the mutex of the `Synchronized` object.
        /// \~russian @brief Уникальная блокировка, управляющая мьютексом объекта `Synchronized`.
        std::unique_lock<TMutex>
            m_l;
        /// \~english @brief Source file name for tracing, used if `is_tracable` is true.
        /// \~russian @brief Имя исходного файла для трассировки, используется, если `is_tracable` истинно.
        char const *
            m_file = nullptr;
        /// \~english @brief Line number for tracing, used if `is_tracable` is true.
        /// \~russian @brief Номер строки для трассировки, используется, если `is_tracable` истинно.
        int m_line = 0;
    };
private:
    friend class Locker;

    /// \~english @brief The mutex used for synchronization by this object and its `Locker`.
    /// \~russian @brief Мьютекс, используемый для синхронизации этим объектом и его `Locker`.
    TMutex mutable
        m_mutex;
    /// \~english @brief The condition variable used for thread coordination (wait/notify) by this object and its `Locker`.
    /// \~russian @brief Условная переменная, используемая для координации потоков (ожидание/уведомление) этим объектом и его `Locker`.
    std::condition_variable mutable
        m_cv;
    /// \~english @brief Atomic flag indicating if threads are allowed to wait on the condition variable.
    ///     Set to false by `interrupt()` to stop further waits.
    /// \~russian @brief Атомарный флаг, указывающий, разрешено ли потокам ожидать на условной переменной.
    ///     Устанавливается в false методом `interrupt()` для прекращения дальнейших ожиданий.
    std::atomic<bool> mutable
        m_is_waitable = true;
};


// class implementation


template<typename TMutex, bool const is_tracable>
void Synchronized<TMutex, is_tracable>::interrupt() {
    m_is_waitable = false;
    m_cv.notify_all();
}


template<typename TMutex, bool const is_tracable>
Synchronized<TMutex, is_tracable>::Locker::Locker(
    Synchronized const * const s,
    char const *file, int line)
:
    m_s         (s),
    m_l         (s->m_mutex)
{
    if (is_tracable) {
        m_file = file;
        m_line = line;
        printf("%s LM\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            file, line);
    }
}


template<typename TMutex, bool const is_tracable>
Synchronized<TMutex, is_tracable>::Locker::~Locker() {
    if (is_tracable)
        printf("%s UM\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            m_file, m_line);

    m_l.unlock();
    m_s->m_cv.notify_one();
}


template<typename TMutex, bool const is_tracable>
bool Synchronized<TMutex, is_tracable>::Locker::wait() {
    if (m_s->m_is_waitable)
        m_s->m_cv.wait(m_l);

    if (is_tracable)
        printf("%s WM -> %s\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            m_s->m_is_waitable ? "OK" : "interrupted",
            m_file, m_line);

    return m_s->m_is_waitable; // ----->
}


template<typename TMutex, bool const is_tracable>
bool Synchronized<TMutex, is_tracable>::Locker::wait(std::chrono::nanoseconds const &timeout) {
    bool result =
        m_s->m_is_waitable &&
        m_s->m_cv.wait_for(m_l, timeout) == std::cv_status::no_timeout;
    if (is_tracable)
        printf("%s WM -> %s\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            result ? "OK" : m_s->m_is_waitable ? "timeout" : "interrupted",
            m_file, m_line);
    return result; // ----->
}


} // namespace iridium::threading


/// \~english @def LOCK_SCOPE()
/// @brief Creates a `Synchronized::Locker` instance named `_____locked_scope_____` for the current scope.
///     This macro is used within a method of a class that inherits from or uses `Synchronized`.
///     It locks the associated mutex and ensures it's unlocked when `_____locked_scope_____` goes out of scope.
///     Requires `this` to point to an instance of a class derived from `Synchronized<TMutex, is_tracable>`.
/// \~russian @def LOCK_SCOPE()
/// @brief Создает экземпляр `Synchronized::Locker` с именем `_____locked_scope_____` для текущей области видимости.
///     Этот макрос используется внутри метода класса, который наследует от `Synchronized` или использует его.
///     Он блокирует связанный мьютекс и гарантирует его разблокировку, когда `_____locked_scope_____` выходит из области видимости.
///     Требует, чтобы `this` указывал на экземпляр класса, производного от `Synchronized<TMutex, is_tracable>`.
#define LOCK_SCOPE() \
Synchronized::Locker _____locked_scope_____(this, __FILE__, __LINE__)


/// \~english @def LOCK_SCOPE_TRY_WAIT(timeout)
/// @brief Calls the `wait(timeout)` method on a `Synchronized::Locker` instance named `_____locked_scope_____`.
///     This macro must be used after `LOCK_SCOPE()` in the same scope.
///     It waits on the condition variable associated with the `Synchronized` object for the specified timeout.
/// \~russian @def LOCK_SCOPE_TRY_WAIT(timeout)
/// @brief Вызывает метод `wait(timeout)` для экземпляра `Synchronized::Locker` с именем `_____locked_scope_____`.
///     Этот макрос должен использоваться после `LOCK_SCOPE()` в той же области видимости.
///     Ожидает на условной переменной, связанной с объектом `Synchronized`, в течение указанного тайм-аута.
/// \~english @param timeout The maximum duration to wait (e.g., `std::chrono::milliseconds(100)`).
/// \~russian @param timeout Максимальная продолжительность ожидания (например, `std::chrono::milliseconds(100)`).
/// \~english @return `true` if the wait was successful (not timed out and not interrupted), `false` otherwise.
/// \~russian @return `true`, если ожидание прошло успешно (не по тайм-ауту и не прервано), `false` в противном случае.
#define LOCK_SCOPE_TRY_WAIT(timeout) \
_____locked_scope_____.wait(timeout)


#endif // HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
