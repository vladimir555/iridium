// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_F2E1584B_C2F5_4504_8194_4002936076F6
#define HEADER_SOCKET_F2E1584B_C2F5_4504_8194_4002936076F6


#include "iridium/io/stream.h"
#include "iridium/io/uri.h"
#include "iridium/pattern/initializable.h"


namespace iridium {
namespace io {
namespace net {


/// \~english @brief Base interface for socket-related streams.
///     Inherits from `IStream` and serves as a common base for specific socket types like `ISocketStream` and `ISocketAcceptor`.
/// \~russian @brief Базовый интерфейс для потоков, связанных с сокетами.
///     Наследуется от `IStream` и служит общей основой для конкретных типов сокетов, таких как `ISocketStream` и `ISocketAcceptor`.
class ISocket: virtual public IStream {
public:
    DEFINE_INTERFACE(ISocket)
};


/// \~english @brief Interface for a socket stream that supports duplex communication (reading and writing).
///     Combines the `ISocket` interface with `IStreamPort` to represent a connected communication endpoint.
/// \~russian @brief Интерфейс для сокетного потока, поддерживающего дуплексную связь (чтение и запись).
///     Объединяет интерфейс `ISocket` с `IStreamPort` для представления подключенной конечной точки связи.
class ISocketStream: public ISocket, virtual public IStreamPort {
public:
    DEFINE_INTERFACE(ISocketStream)
};


/// \~english @brief Interface for a socket acceptor, typically a listening server socket.
///     It is responsible for accepting incoming client connections.
/// \~russian @brief Интерфейс для принимающего сокета, обычно слушающего серверного сокета.
///     Отвечает за прием входящих клиентских подключений.
class ISocketAcceptor: public ISocket {
public:
    DEFINE_INTERFACE(ISocketAcceptor)

    /// \~english @brief Accepts a new incoming client connection.
    ///     This is a blocking call that waits for a new connection and, upon success,
    ///     returns a stream port for communication with the connected client.
    /// \~russian @brief Принимает новое входящее клиентское подключение.
    ///     Это блокирующий вызов, который ожидает нового подключения и, в случае успеха,
    ///     возвращает потоковый порт для обмена данными с подключенным клиентом.
    /// \~english @return A shared pointer to an `ISocketStream` representing the connection to the client.
    ///     Returns `nullptr` or throws an exception if an error occurs or the acceptor is closed.
    /// \~russian @return Умный указатель на `ISocketStream`, представляющий подключение к клиенту.
    ///     Возвращает `nullptr` или выбрасывает исключение, если возникает ошибка или принимающий сокет закрыт.
    virtual ISocketStream::TSharedPtr accept() = 0;
};



} // net
} // io
} // iridium


#endif // HEADER_SOCKET_F2E1584B_C2F5_4504_8194_4002936076F6
