// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_METHOD_C791FE23_9B73_46FE_9B6F_F7CE6E978E77
#define HEADER_METHOD_C791FE23_9B73_46FE_9B6F_F7CE6E978E77


#include "iridium/enum.h"

#include <string>


namespace iridium {
namespace io {
namespace protocol {
namespace http {


/// \~english @brief Enumeration of HTTP request methods.
/// \~russian @brief Перечисление методов HTTP-запроса.
DEFINE_ENUM(
    TMethod,
    /// \~english @brief The GET method requests a representation of the specified resource. Requests using GET should only retrieve data.
    /// \~russian @brief Метод GET запрашивает представление указанного ресурса. Запросы с использованием GET должны только извлекать данные.
    GET,
    /// \~english @brief The HEAD method asks for a response identical to that of a GET request, but without the response body.
    /// \~russian @brief Метод HEAD запрашивает ответ, идентичный ответу на GET-запрос, но без тела ответа.
    HEAD,
    /// \~english @brief The POST method is used to submit an entity to the specified resource, often causing a change in state or side effects on the server.
    /// \~russian @brief Метод POST используется для отправки сущности на указанный ресурс, часто вызывая изменение состояния или побочные эффекты на сервере.
    POST,
    /// \~english @brief The PUT method replaces all current representations of the target resource with the request payload.
    /// \~russian @brief Метод PUT заменяет все текущие представления целевого ресурса полезной нагрузкой запроса.
    PUT,
    /// \~english @brief The DELETE method deletes the specified resource.
    /// \~russian @brief Метод DELETE удаляет указанный ресурс.
    DELETE,
    /// \~english @brief The CONNECT method establishes a tunnel to the server identified by the target resource.
    /// \~russian @brief Метод CONNECT устанавливает туннель к серверу, идентифицированному целевым ресурсом.
    CONNECT,
    /// \~english @brief The OPTIONS method is used to describe the communication options for the target resource.
    /// \~russian @brief Метод OPTIONS используется для описания опций связи для целевого ресурса.
    OPTIONS,
    /// \~english @brief The TRACE method performs a message loop-back test along the path to the target resource.
    /// \~russian @brief Метод TRACE выполняет тест обратной связи сообщения по пути к целевому ресурсу.
    TRACE
)


} // http
} // protocol
} // io
} // iridium



#endif // HEADER_METHOD_C791FE23_9B73_46FE_9B6F_F7CE6E978E77
