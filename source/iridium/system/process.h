// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROCESS_70CE2A25_38CC_4D60_A1A7_B14F33DB94CF
#define HEADER_PROCESS_70CE2A25_38CC_4D60_A1A7_B14F33DB94CF


#include "iridium/io/stream.h"
#include "iridium/enum.h"


namespace iridium::system {


/// \~english @brief An interface for controlling and interacting with a system process.
/// \~russian @brief Интерфейс для управления и взаимодействия с системным процессом.
class IProcess: public virtual io::IStreamPort {
public:
    DEFINE_INTERFACE(IProcess)

    /// \~english @brief Represents the state of a process.
    /// \~russian @brief Представляет состояние процесса.
    struct TState {
        /// \~english @brief The condition of the process.
        /// \~russian @brief Состояние процесса.
        DEFINE_ENUM(TCondition, DONE, CRASHED, INTERRUPTED, RUNNING, TIMEOUT);
        TCondition              condition; ///< \~english The current condition of the process. \~russian Текущее состояние процесса.
        std::shared_ptr<int>    exit_code; ///< \~english The exit code of the process, if it has terminated. \~russian Код завершения процесса, если он завершился.
    };

    /// \~english @brief Signals that can be sent to a process.
    /// \~russian @brief Сигналы, которые могут быть отправлены процессу.
    DEFINE_ENUM(
        TSignal,
        INTERRUPT, ///< \~english Interrupt signal. \~russian Сигнал прерывания.
        TERMINATE, ///< \~english Terminate signal. \~russian Сигнал завершения.
        KILL ///< \~english Kill signal. \~russian Сигнал "убийства".
    );

    /// \~english @brief Gets the current state of the process.
    /// \~russian @brief Возвращает текущее состояние процесса.
    virtual TState getState()   = 0;
    /// \~english @brief Sends a signal to the process.
    /// \~russian @brief Отправляет сигнал процессу.
    virtual void   sendSignal(TSignal const &signal) = 0;
};


} // namespace iridium::system


//DEFINE_ENUM_CONVERT(iridium::system::IProcess::TState::TCondition)
//DEFINE_ENUM_CONVERT(iridium::system::IProcess::TSignal)


#endif // HEADER_PROCESS_70CE2A25_38CC_4D60_A1A7_B14F33DB94CF
