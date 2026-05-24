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


// todo: multi mutex waiting; shared mutex wrapping
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
        bool wait(std::chrono::system_clock::duration const &timeout);
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

protected:
    virtual bool checkWaitingPredicate() const;

private:
    friend class Locker;

    void ensureCV() const {
        std::call_once(m_cv_init_flag,
            [this] {
                m_cv = std::make_unique<std::condition_variable>();
            }
        );
    }

    TMutex mutable
        m_mutex;
    std::unique_ptr<std::condition_variable> mutable
        m_cv;
    std::once_flag mutable
        m_cv_init_flag;
    std::atomic<bool> mutable
        m_is_interrupted { false };
};


// class implementation


template<typename TMutex, bool const is_tracable>
void Synchronized<TMutex, is_tracable>::interrupt() {
    m_is_interrupted = true;
    if (m_cv)
        m_cv->notify_all();
}


template<typename TMutex, bool const is_tracable>
bool Synchronized<TMutex, is_tracable>::checkWaitingPredicate() const {
    return true; // ----->
}


template<typename TMutex, bool const is_tracable>
Synchronized<TMutex, is_tracable>::Locker::Locker(
    Synchronized const * const s,
    char const *file, int line)
:
    m_s(s),
    m_l(s->m_mutex)
{
    if constexpr (is_tracable) {
        m_file = file;
        m_line = line;
        printf("%s LM\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            file, line);
    }
}


template<typename TMutex, bool const is_tracable>
Synchronized<TMutex, is_tracable>::Locker::~Locker() {
    if constexpr (is_tracable)
        printf("%s UM\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            m_file, m_line);

    m_l.unlock();
    if (m_s && m_s->m_cv)
        m_s->m_cv->notify_one();
}


template<typename TMutex, bool const is_tracable>
bool Synchronized<TMutex, is_tracable>::Locker::wait() {
    if (m_s->m_is_interrupted)
        return false; // ----->

    m_s->ensureCV();
    m_s->m_cv->wait(
        m_l,
        [this] () {
            return m_s->m_is_interrupted || m_s->checkWaitingPredicate();
        }
    );

    if constexpr (is_tracable)
        printf("%s WM -> %s\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            m_s->m_is_interrupted ? "interrupted" : "OK",
            m_file, m_line);

    return true; // ----->
}


template<typename TMutex, bool const is_tracable>
bool Synchronized<TMutex, is_tracable>::Locker::wait(std::chrono::system_clock::duration const &timeout) {
    if (m_s->m_is_interrupted)
        return false; // ----->

    bool result = true;

    m_s->ensureCV();
    result = m_s->m_cv->wait_for(
        m_l, timeout,
        [this] () {
            return m_s->m_is_interrupted || m_s->checkWaitingPredicate();
        }
    );

    if constexpr (is_tracable)
        printf("%s WM -> %s\n%s:%i\n",
            threading::IThread::getNameStatic().c_str(),
            result ? "OK" : m_s->m_is_interrupted ? "interrupted" : "timeout",
            m_file, m_line);

    return result; // ----->
}


} // namespace iridium::threading


/// \~english @def LOCK_SCOPE()
/// @brief Creates a `Synchronized::Locker` instance for the current scope.
/// \~russian @def LOCK_SCOPE()
/// @brief Создает экземпляр `Synchronized::Locker` для текущей области видимости.
#define LOCK_SCOPE() \
Synchronized::Locker _____locked_scope_##__LINE__(this, __FILE__, __LINE__)


/// \~english @def LOCK_SCOPE_TRY_WAIT(...)
/// @brief Tries to wait on the condition variable within the current locked scope.
/// \~russian @def LOCK_SCOPE_TRY_WAIT(...)
/// @brief Пытается ожидать на условной переменной внутри текущей заблокированной области.
#define LOCK_SCOPE_TRY_WAIT(...) \
_____locked_scope_##__LINE__.wait(__VA_ARGS__)


#endif // HEADER_PROTOCOL_FACTORY_BA993AE8_B05D_4A20_A8C6_38E965E820DD
