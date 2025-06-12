// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines the interface for a session manager.
/// \~russian @brief Определяет интерфейс для менеджера сессий.
#ifndef HEADER_SESSION_MANAGER_BB5946FC_0AA1_4747_AE53_07DB1F105A72
#define HEADER_SESSION_MANAGER_BB5946FC_0AA1_4747_AE53_07DB1F105A72


#include "iridium/pattern/initializable.h" // For IInitializable
#include "iridium/smart_ptr.h"           // For DEFINE_INTERFACE and TSharedPtr

#include "protocol.h" // For IProtocol::TSharedPtr
// Assuming IStreamPort::TSharedPtr is a valid type, likely defined in a header included transitively
// or in a common types header for iridium::io. It represents an I/O channel endpoint.
// For example, it could be a typedef for IStream::TSharedPtr or a specific stream port class.
#include "stream.h" // Assuming IStreamPort might be related to or a typedef of IStream


namespace iridium {
namespace io {


/// \~english @brief Interface for managing communication sessions.
///     A session manager is responsible for overseeing the lifecycle of individual sessions,
///     which typically involve an I/O stream (or stream port) and a specific protocol handler.
///     It inherits from `IInitializable`, suggesting that the manager itself might require
///     an initialization step before it can manage sessions.
/// \~russian @brief Интерфейс для управления сессиями обмена данными.
///     Менеджер сессий отвечает за контроль жизненного цикла отдельных сессий,
///     которые обычно включают поток ввода-вывода (или порт потока) и определенный обработчик протокола.
///     Наследуется от `IInitializable`, что предполагает, что сам менеджер может требовать
///     этапа инициализации перед тем, как сможет управлять сессиями.
class ISessionManager: public pattern::IInitializable {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(ISessionManager)

    /// \~english @brief Instructs the session manager to begin managing a new session based on the provided stream/port and protocol.
    ///     This method is typically called when a new connection is accepted or a new communication channel is established.
    ///     The manager will then be responsible for the lifecycle of this session, including event handling and data processing
    ///     as defined by the protocol on the given stream/port.
    /// \~russian @brief Указывает менеджеру сессий начать управление новой сессией на основе предоставленного потока/порта и протокола.
    ///     Этот метод обычно вызывается при принятии нового соединения или установлении нового канала связи.
    ///     Менеджер затем будет отвечать за жизненный цикл этой сессии, включая обработку событий и данных,
    ///     как определено протоколом для данного потока/порта.
    /// \~english @param streamPort A shared pointer to an `IStreamPort` (or a similar I/O channel abstraction like `IStream`)
    ///     representing the communication endpoint for the session.
    /// \~russian @param streamPort Умный указатель на `IStreamPort` (или аналогичную абстракцию канала ввода-вывода, такую как `IStream`),
    ///     представляющий конечную точку связи для сессии.
    /// \~english @param protocol A shared pointer to an `IProtocol` that will handle the data communication logic for this session.
    /// \~russian @param protocol Умный указатель на `IProtocol`, который будет обрабатывать логику обмена данными для этой сессии.
    /// \~english @throws iridium::Exception or similar if the manager is unable to start managing the session.
    /// \~russian @throws iridium::Exception или аналогичное, если менеджер не может начать управление сессией.
    virtual void manage(
        IStream::TSharedPtr const &streamPort, // Assuming IStreamPort might be a typedef or compatible with IStream
        IProtocol::TSharedPtr   const &protocol
    ) = 0;
};


} // io
} // iridium


#endif // HEADER_SESSION_MANAGER_BB5946FC_0AA1_4747_AE53_07DB1F105A72
