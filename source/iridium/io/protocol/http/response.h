// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA
#define HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA


#include "iridium/parsing/serialization/node.h"
#include "iridium/io/buffer.h"

#include <string>
#include <chrono>


namespace iridium::io::protocol::http::response {


DEFINE_ROOT_NODE_BEGIN(Http)
    /// \~english @brief Default server name string to be used in the "Server" HTTP header.
    /// \~russian @brief Строка имени сервера по умолчанию для использования в HTTP-заголовке "Server".
    static std::string const DEFAULT_SERVER_NAME;

    /// \~english @brief Represents the first line of an HTTP response, also known as the status line.
    /// \~russian @brief Представляет первую строку HTTP-ответа, также известную как строка состояния.
    struct TMessageLine {
        /// \~english @brief The HTTP protocol version string (e.g., "HTTP/1.1").
        /// \~russian @brief Строка версии HTTP-протокола (например, "HTTP/1.1").
        std::string protocol;
        /// \~english @brief The HTTP status code (e.g., 200, 404).
        /// \~russian @brief Код состояния HTTP (например, 200, 404).
        int         code;
        /// \~english @brief The reason phrase associated with the status code (e.g., "OK", "Not Found").
        /// \~russian @brief Фраза-причина, связанная с кодом состояния (например, "OK", "Not Found").
        std::string reason;
    };
    /// \~english @brief The message line of the HTTP response.
    /// \~russian @brief Строка сообщения HTTP-ответа.
    DEFINE_ATTRIBUTE(TMessageLine, Message, TMessageLine( {"HTTP/1.1", 200, "OK"} ))
    /// \~english @brief The headers of the HTTP response.
    /// \~russian @brief Заголовки HTTP-ответа.
    DEFINE_NODE_BEGIN(Headers)
        /// \~english @brief Represents an HTTP date and time, typically for headers like "Date" or "Last-Modified".
        /// \~russian @brief Представляет дату и время HTTP, обычно для заголовков типа "Date" или "Last-Modified".
        struct THTTPDate {
            /// \~english @brief The date and time point.
            /// \~russian @brief Момент времени и дата.
            std::chrono::system_clock::time_point date;
        };
        /// \~english @brief The Date header, representing the time the response was generated.
        /// \~russian @brief Заголовок Date, представляющий время создания ответа.
        DEFINE_ATTRIBUTE(THTTPDate, Date, THTTPDate())
        /// \~english @brief The Server header, containing information about the software used by the origin server.
        /// \~russian @brief Заголовок Server, содержащий информацию о программном обеспечении, используемом исходным сервером.
        DEFINE_ATTRIBUTE(std::string, server, DEFAULT_SERVER_NAME)
        /// \~english @brief The Last-Modified header, indicating the last modification date of the resource.
        /// \~russian @brief Заголовок Last-Modified, указывающий дату последнего изменения ресурса.
        DEFINE_ATTRIBUTE(THTTPDate, LastModified, THTTPDate())
        /// \~english @brief The Content-Length header, indicating the size of the response body in bytes.
        /// \~russian @brief Заголовок Content-Length, указывающий размер тела ответа в байтах.
        DEFINE_ATTRIBUTE(size_t, ContentLength, 0)
        /// \~english @brief The Content-Type header, indicating the media type of the resource.
        /// \~russian @brief Заголовок Content-Type, указывающий медиатип ресурса.
        DEFINE_ATTRIBUTE(std::string, ContentType, "text/html")
//        DEFINE_ATTRIBUTE(std::string, Connection, "")
    DEFINE_NODE_END(Headers)
    /// \~english @brief The body of the HTTP response.
    /// \~russian @brief Тело HTTP-ответа.
    DEFINE_ATTRIBUTE(Buffer, Body, Buffer())
DEFINE_ROOT_NODE_END()


} // iridium::io::protocol::http::response


DEFINE_CONVERT(iridium::io::protocol::http::response::THttp::TMessageLine, std::string)
DEFINE_CONVERT(std::string, iridium::io::protocol::http::response::THttp::TMessageLine)

DEFINE_CONVERT(iridium::io::protocol::http::response::THttp::TMessage, std::string)
DEFINE_CONVERT(std::string, iridium::io::protocol::http::response::THttp::TMessage)

DEFINE_CONVERT(iridium::io::protocol::http::response::THttp::THeaders::THTTPDate, std::string)
DEFINE_CONVERT(std::string, iridium::io::protocol::http::response::THttp::THeaders::THTTPDate)


#endif // HEADER_RESPONSE_06EB69FA_CFFD_4698_B80D_CC83F2467ABA
