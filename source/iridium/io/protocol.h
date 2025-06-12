/// \~english @brief Defines the interface for a generic I/O protocol handler.
/// \~russian @brief Определяет интерфейс для обработчика обобщенного протокола ввода-вывода.
#ifndef HEADER_PROTOCOL_A4280CC9_7465_4C74_805A_D60B575171BB
#define HEADER_PROTOCOL_A4280CC9_7465_4C74_805A_D60B575171BB


#include "context.h"  // For IPipeManager (used in IProtocol::control)
#include "event.h"    // For Event::TSharedPtr (used in IProtocol::control)
#include "iridium/smart_ptr.h" // For DEFINE_INTERFACE


namespace iridium {
namespace io {


/// \~english @brief Interface for a protocol handler.
///     A protocol handler is responsible for implementing the logic of a specific
///     communication protocol. This includes parsing incoming data, formatting outgoing data,
///     managing protocol states, and reacting to I/O events.
/// \~russian @brief Интерфейс для обработчика протокола.
///     Обработчик протокола отвечает за реализацию логики конкретного
///     протокола обмена данными. Это включает разбор входящих данных, форматирование исходящих данных,
///     управление состояниями протокола и реакцию на события ввода-вывода.
class IProtocol {
public:
    /// \~english @brief Macro used to define common interface elements (e.g., virtual destructor).
    /// \~russian @brief Макрос, используемый для определения общих элементов интерфейса (например, виртуального деструктора).
    DEFINE_INTERFACE(IProtocol)

    /// \~english @brief Controls the protocol's behavior in response to an I/O event.
    ///     This is the primary method where the protocol logic is implemented. It processes
    ///     an incoming event (e.g., data received, connection state change) and may interact
    ///     with I/O streams or pipes via the provided `pipe_manager`.
    /// \~russian @brief Управляет поведением протокола в ответ на событие ввода-вывода.
    ///     Это основной метод, в котором реализуется логика протокола. Он обрабатывает
    ///     входящее событие (например, получение данных, изменение состояния соединения) и может взаимодействовать
    ///     с потоками ввода-вывода или каналами через предоставленный `pipe_manager`.
    /// \~english @param event A shared pointer to the `Event` that has occurred and needs to be processed by the protocol.
    /// \~russian @param event Умный указатель на `Event`, которое произошло и должно быть обработано протоколом.
    /// \~english @param pipe_manager A shared pointer to an `IPipeManager` that the protocol can use to
    ///     access or manage underlying data pipes or streams associated with the event or session.
    /// \~russian @param pipe_manager Умный указатель на `IPipeManager`, который протокол может использовать для
    ///     доступа или управления базовыми каналами данных или потоками, связанными с событием или сессией.
    /// \~english @return `true` if the event was successfully processed or if the protocol state allows for further operations;
    ///     `false` might indicate that the event could not be handled, the protocol reached a terminal state,
    ///     or an error occurred that requires closing the connection/pipe.
    /// \~russian @return `true`, если событие было успешно обработано или если состояние протокола допускает дальнейшие операции;
    ///     `false` может указывать на то, что событие не удалось обработать, протокол достиг конечного состояния
    ///     или произошла ошибка, требующая закрытия соединения/канала.
    virtual bool control(Event::TSharedPtr const &event, IPipeManager::TSharedPtr const &pipe_manager) = 0;
};


} // io
} // iridium


#endif // HEADER_PROTOCOL_A4280CC9_7465_4C74_805A_D60B575171BB
