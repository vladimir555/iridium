// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROCESS_70CE2A25_38CC_4D60_A1A7_B14F33DB94CF
#define HEADER_PROCESS_70CE2A25_38CC_4D60_A1A7_B14F33DB94CF


#include "iridium/io/stream.h"
#include "iridium/enum.h"


namespace iridium {
/// \~english @brief Provides classes, interfaces, and utilities for system-level operations.
///     This includes functionalities related to process management, inter-process communication,
///     and interaction with the underlying operating system environment.
/// \~russian @brief Предоставляет классы, интерфейсы и утилиты для операций системного уровня.
///     Включает функциональность, связанную с управлением процессами, межпроцессным взаимодействием
///     и взаимодействием с базовой средой операционной системы.
namespace system {

/// \~english @brief Interface for interacting with a system process.
///     It defines methods to get the state of a process and, by inheriting from `iridium::io::IStreamPort`,
///     it is expected to provide access to the process's standard I/O streams (stdin, stdout, stderr).
/// \~russian @brief Интерфейс для взаимодействия с системным процессом.
///     Определяет методы для получения состояния процесса и, наследуя от `iridium::io::IStreamPort`,
///     ожидается, что он предоставит доступ к стандартным потокам ввода-вывода процесса (stdin, stdout, stderr).
class IProcess: public virtual io::IStreamPort {
public:
    /// \~english @brief Defines common interface elements for `IProcess`, such as smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.).
    /// \~russian @brief Определяет общие элементы интерфейса для `IProcess`, такие как typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.).
    DEFINE_INTERFACE(IProcess)

    /// \~english @brief Represents the state of a system process, including its current condition and exit code if applicable.
    /// \~russian @brief Представляет состояние системного процесса, включая его текущее условие и код завершения, если применимо.
    struct TState {
        /// \~english @brief Enumerates the possible conditions or lifecycle states of a system process.
        /// \~russian @brief Перечисляет возможные условия или состояния жизненного цикла системного процесса.
        DEFINE_ENUM(
            TCondition,
            /// \~english @brief The process has completed its execution successfully.
            /// \~russian @brief Процесс успешно завершил свое выполнение.
            DONE,
            /// \~english @brief The process terminated due to an error or crash.
            /// \~russian @brief Процесс завершился из-за ошибки или сбоя.
            CRASHED,
            /// \~english @brief The process was interrupted (e.g., by a signal).
            /// \~russian @brief Процесс был прерван (например, сигналом).
            INTERRUPTED,
            /// \~english @brief The process is currently running.
            /// \~russian @brief Процесс в данный момент выполняется.
            RUNNING,
            /// \~english @brief The process exceeded a timeout limit.
            /// \~russian @brief Процесс превысил лимит времени ожидания.
            TIMEOUT
        );
        /// \~english @brief The current condition of the process (e.g., RUNNING, DONE).
        /// \~russian @brief Текущее условие процесса (например, RUNNING, DONE).
        TCondition              condition;
        /// \~english @brief The exit code of the process. This is typically valid if the condition is `DONE` or `CRASHED`.
        ///     It is a shared pointer to an int; a `nullptr` may indicate the process has not exited or the exit code is not available.
        /// \~russian @brief Код завершения процесса. Обычно действителен, если условие `DONE` или `CRASHED`.
        ///     Это разделяемый указатель на int; `nullptr` может указывать, что процесс не завершился или код завершения недоступен.
        std::shared_ptr<int>    exit_code;
    };

    /// \~english @brief Retrieves the current state of the process.
    /// \~russian @brief Получает текущее состояние процесса.
    /// \~english @return A `TState` object representing the current condition and potentially the exit code of the process.
    /// \~russian @return Объект `TState`, представляющий текущее условие и, возможно, код завершения процесса.
    virtual TState getState() = 0;
};


} // system
} // iridium


#endif // HEADER_PROCESS_70CE2A25_38CC_4D60_A1A7_B14F33DB94CF
