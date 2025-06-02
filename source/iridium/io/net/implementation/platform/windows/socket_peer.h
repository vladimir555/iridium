// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#pragma once


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/net/socket.h"
#include "iridium/encryption/openssl.h"

#include "socket.h"
#include "socket_acceptor.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


/// \~english @brief Windows-specific peer socket implementation.
///     Represents a connection that has been accepted by a server.
///     This class currently appears to be a minimal skeleton or incomplete,
///     inheriting from `ISocketStream` but without further methods or members defined in this header.
///     Full functionality would likely involve managing a `SOCKET` handle obtained from an accept operation.
/// \~russian @brief Реализация сокета равноправного узла для Windows.
///     Представляет соединение, которое было принято сервером.
///     Этот класс в настоящее время выглядит как минимальный скелет или является неполным,
///     наследуясь от `ISocketStream`, но без дополнительных методов или членов, определенных в этом заголовочном файле.
///     Полная функциональность, вероятно, будет включать управление дескриптором `SOCKET`, полученным в результате операции accept.
class CSocketPeer : public ISocketStream {
public:
    DEFINE_IMPLEMENTATION(CSocketPeer)
    /// \~english @brief Default constructor for a Windows peer socket.
    ///     Further initialization (e.g., associating with an accepted SOCKET handle) would be needed.
    /// \~russian @brief Конструктор по умолчанию для сокета равноправного узла Windows.
    ///     Потребуется дальнейшая инициализация (например, связывание с принятым дескриптором SOCKET).
    CSocketPeer() = default;
};


}
}
}
}
}


#endif // WINDOWS_PLATFORM
