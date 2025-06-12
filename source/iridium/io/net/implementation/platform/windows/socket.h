// Copyright � 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SOCKET_846B93DA_90F4_4B30_BBFF_EC3D1526964B
#define HEADER_SOCKET_846B93DA_90F4_4B30_BBFF_EC3D1526964B


#include "iridium/platform.h"


#ifdef WINDOWS_PLATFORM


#include "iridium/io/uri.h"
#include "iridium/io/net/socket.h"


namespace iridium {
namespace io {
namespace net {
namespace implementation {
namespace platform {


/// \~english @brief Windows-specific socket implementation.
///     This class is intended to implement the `ISocket` interface for Windows,
///     likely using Winsock (WSA) functions for socket operations.
///     Many methods and members are currently commented out, suggesting it might be a partial or evolving implementation.
/// \~russian @brief Реализация сокета для Windows.
///     Этот класс предназначен для реализации интерфейса `ISocket` для Windows,
///     вероятно, используя функции Winsock (WSA) для операций с сокетами.
///     Многие методы и члены в настоящее время закомментированы, что предполагает, что это частичная или развивающаяся реализация.
class CSocket : public ISocket {
public:
	DEFINE_IMPLEMENTATION(CSocket)
	/// \~english @brief Constructs a `CSocket` instance.
	/// \~russian @brief Конструирует экземпляр `CSocket`.
	/// \~english @param uri The URI representing the socket endpoint (e.g., for connection or listening).
	/// \~russian @param uri URI, представляющий конечную точку сокета (например, для подключения или прослушивания).
	/// \~english @param is_server_mode Boolean indicating if this socket is intended to be used in server mode (e.g., for listening).
	/// \~russian @param is_server_mode Булево значение, указывающее, предназначен ли этот сокет для использования в серверном режиме (например, для прослушивания).
	CSocket(URI const &uri, bool const &is_server_mode);

	/// \~english @brief Initializes the socket.
	///     For Windows, this would typically involve `WSAStartup` (if not globally handled),
	///     creating a socket using `socket()` or `WSASocket()`, and then potentially `bind()`, `listen()`, or `connect()`.
	/// \~russian @brief Инициализирует сокет.
	///     Для Windows это обычно включает `WSAStartup` (если не обрабатывается глобально),
	///     создание сокета с использованием `socket()` или `WSASocket()`, а затем, возможно, `bind()`, `listen()` или `connect()`.
	void    initialize() override;

	/// \~english @brief Finalizes the socket.
	///     For Windows, this would typically involve `closesocket()` and potentially `WSACleanup` (if not globally handled).
	/// \~russian @brief Завершает работу сокета.
	///     Для Windows это обычно включает `closesocket()` и, возможно, `WSACleanup` (если не обрабатывается глобально).
	void    finalize() override;

	// \~english @brief Accepts a new connection (commented out in source).
	// \~russian @brief Принимает новое соединение (закомментировано в исходном коде).
	//ISocket::TSharedPtr accept() override;

	// \~english @brief Gets the URI of this socket (commented out in source).
	// \~russian @brief Получает URI этого сокета (закомментировано в исходном коде).
	//URI     getURI() const override;

	// \~english @brief Gets an identifier for this socket (commented out in source).
	// \~russian @brief Получает идентификатор этого сокета (закомментировано в исходном коде).
	//int     getID() const override;

	// \~english @brief Writes data to the socket (commented out in source).
	// \~russian @brief Записывает данные в сокет (закомментировано в исходном коде).
	//size_t write(Buffer::TSharedPtr const& buffer) override;

	// \~english @brief Reads data from the socket (commented out in source).
	// \~russian @brief Читает данные из сокета (закомментировано в исходном коде).
	//Buffer::TSharedPtr read(size_t const &size) override;

	// \~english @brief Flushes any buffered data (commented out in source).
	// \~russian @brief Сбрасывает все буферизованные данные (закомментировано в исходном коде).
	//    void    flush() override;

//private:
//	CSocket(URI const &uri, int const &fd);
//
//	URI     getPeerURI(int const &fd);
//	void    setBlockingMode(bool const &is_blocking);
//
//	bool    m_is_blocking;
//	bool    m_is_server_mode;
//	URI     m_uri;
//	int     m_socket; // Likely to be SOCKET type on Windows
};


} // platform
} // implementation
} // net
} // io
} // iridium


#endif // WINDOWS_PLATFORM


#endif // HEADER_SOCKET_846B93DA_90F4_4B30_BBFF_EC3D1526964B
