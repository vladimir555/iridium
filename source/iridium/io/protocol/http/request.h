// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_REQUEST_E86B18F8_93B2_4E65_B812_2A46EF802DC6
#define HEADER_REQUEST_E86B18F8_93B2_4E65_B812_2A46EF802DC6


#include  "iridium/parsing/serialization/node.h"
#include "iridium/enum.h"

#include <string>

#include "method.h"


namespace iridium {
namespace io {
namespace protocol {
namespace http {
namespace request {


DEFINE_ROOT_NODE_BEGIN(Http)
    /// \~english @brief Represents the first line of an HTTP request, also known as the request line.
    /// \~russian @brief Представляет первую строку HTTP-запроса, также известную как строка запроса.
    struct TMessageLine {
        /// \~english @brief The HTTP method (e.g., GET, POST).
        /// \~russian @brief HTTP-метод (например, GET, POST).
        TMethod     method;
        /// \~english @brief The request URI (e.g., "/index.html", "http://example.com/path").
        /// \~russian @brief URI запроса (например, "/index.html", "http://example.com/path").
        std::string uri;
        /// \~english @brief The HTTP protocol version string (e.g., "HTTP/1.1").
        /// \~russian @brief Строка версии HTTP-протокола (например, "HTTP/1.1").
        std::string protocol;
    };
    /// \~english @brief The message line of the HTTP request.
    /// \~russian @brief Строка сообщения HTTP-запроса.
    DEFINE_ATTRIBUTE(TMessageLine, Message, TMessageLine({TMethod::GET, "", ""}))
    /// \~english @brief The headers of the HTTP request.
    /// \~russian @brief Заголовки HTTP-запроса.
    DEFINE_NODE_BEGIN(Headers)
        /// \~english @brief The Host header specifies the domain name of the server.
        /// \~russian @brief Заголовок Host указывает доменное имя сервера.
        DEFINE_ATTRIBUTE(std::string, Host, "")
        /// \~english @brief The User-Agent header contains a characteristic string that lets servers identify the application, operating system, vendor, and/or version of the requesting user agent.
        /// \~russian @brief Заголовок User-Agent содержит характерную строку, которая позволяет серверам идентифицировать приложение, операционную систему, поставщика и/или версию запрашивающего пользовательского агента.
        DEFINE_ATTRIBUTE(std::string, UserAgent, "")
        /// \~english @brief The Connection header controls whether the network connection stays open after the current transaction finishes.
        /// \~russian @brief Заголовок Connection управляет тем, остается ли сетевое соединение открытым после завершения текущей транзакции.
        DEFINE_ATTRIBUTE(std::string, Connection, "")
        /// \~english @brief The Accept header lists the MIME types of media resources that the agent is willing to process.
        /// \~russian @brief Заголовок Accept перечисляет MIME-типы медиа-ресурсов, которые агент готов обрабатывать.
        DEFINE_ATTRIBUTE_LIST(std::string, Accept)
        /// \~english @brief The Accept-Language header indicates the natural language and locale that the client prefers.
        /// \~russian @brief Заголовок Accept-Language указывает предпочтительный для клиента естественный язык и локаль.
        DEFINE_ATTRIBUTE_LIST(std::string, AcceptLanguage)
        /// \~english @brief The Accept-Encoding header indicates the content encoding that the client is able to understand.
        /// \~russian @brief Заголовок Accept-Encoding указывает кодировку содержимого, которую клиент может понять.
        DEFINE_ATTRIBUTE_LIST(std::string, AcceptEncoding)
        /// \~english @brief The Accept-Charset header indicates the character sets that the client is able to understand.
        /// \~russian @brief Заголовок Accept-Charset указывает наборы символов, которые клиент может понять.
        DEFINE_ATTRIBUTE_LIST(std::string, AcceptCharset)
        /// \~english @brief The Cookie header contains stored HTTP cookies previously sent by the server with the Set-Cookie header.
        /// \~russian @brief Заголовок Cookie содержит сохраненные HTTP-куки, ранее отправленные сервером с помощью заголовка Set-Cookie.
        DEFINE_ATTRIBUTE_LIST(std::string, Cookie)
    // todo: Keep-Alive: timeout=5, max=1000
    DEFINE_NODE_END(Headers)
    /// \~english @brief The body of the HTTP request.
    /// \~russian @brief Тело HTTP-запроса.
    DEFINE_ATTRIBUTE(std::string, Body, "")
DEFINE_ROOT_NODE_END()


} // request
} // http
} // protocol
} // io
} // iridium


DEFINE_CONVERT(std::string, iridium::io::protocol::http::request::THttp::TMessageLine)
DEFINE_CONVERT(iridium::io::protocol::http::request::THttp::TMessageLine, std::string)


#endif // HEADER_REQUEST_E86B18F8_93B2_4E65_B812_2A46EF802DC6
