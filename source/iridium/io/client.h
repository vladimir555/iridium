// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CLIENT_E242039D_4C9B_4227_97AA_5179FBC55F33
#define HEADER_CLIENT_E242039D_4C9B_4227_97AA_5179FBC55F33


#include "iridium/pattern/initializable.h"


namespace iridium {
namespace io {


/// \~english @brief Interface for a generic client.
///     Defines the basic contract for client objects that can be initialized.
///     Concrete client implementations (e.g., for specific network protocols like TCP, UDP, or higher-level protocols)
///     are expected to implement this interface. The client lifecycle typically involves
///     initialization (e.g., with connection parameters via `IInitializable` methods) before
///     it can perform its client duties (like connecting, sending/receiving data).
/// \~russian @brief Интерфейс для обобщенного клиента.
///     Определяет базовый контракт для клиентских объектов, которые могут быть инициализированы.
///     Предполагается, что конкретные реализации клиентов (например, для определенных сетевых протоколов,
///     таких как TCP, UDP, или протоколов более высокого уровня) будут реализовывать этот интерфейс.
///     Жизненный цикл клиента обычно включает инициализацию (например, с параметрами подключения через
///     методы `IInitializable`) перед тем, как он сможет выполнять свои клиентские функции
///     (такие как подключение, отправка/получение данных).
class IClient: public pattern::IInitializable {
public:
    /// \~english @brief Macro used to define common interface elements.
    ///     For an interface like `IClient`, this typically includes a virtual destructor
    ///     and potentially other common infrastructure needed for Iridium interfaces.
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса.
    ///     Для интерфейса, такого как `IClient`, это обычно включает виртуальный деструктор
    ///     и, возможно, другую общую инфраструктуру, необходимую для интерфейсов Iridium.
    DEFINE_INTERFACE(IClient)
    // Note: Specific client operations like connect, disconnect, send, receive are
    // expected to be defined in derived interfaces or concrete implementations.
};


} // io
} // iridium


#endif // HEADER_CLIENT_E242039D_4C9B_4227_97AA_5179FBC55F33
