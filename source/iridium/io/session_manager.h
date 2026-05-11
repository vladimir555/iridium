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
#include "stream.h" // For IStreamPort


namespace iridium::io {


// todo: rm, deprecated
class ISessionManager: public pattern::IInitializable {
public:
    DEFINE_INTERFACE(ISessionManager)

    virtual void manage(
        IStreamPort::TSharedPtr const &stream_port,
        IProtocol::TSharedPtr   const &protocol
    ) = 0;
};


} // namespace iridium::io


#endif // HEADER_SESSION_MANAGER_BB5946FC_0AA1_4747_AE53_07DB1F105A72
