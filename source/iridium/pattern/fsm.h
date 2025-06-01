// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34
#define HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34


#include "iridium/smart_ptr.h"


//https://ru.wikipedia.org/wiki/%D0%90%D0%B2%D1%82%D0%BE%D0%BC%D0%B0%D1%82_%D1%81_%D0%BC%D0%B0%D0%B3%D0%B0%D0%B7%D0%B8%D0%BD%D0%BD%D0%BE%D0%B9_%D0%BF%D0%B0%D0%BC%D1%8F%D1%82%D1%8C%D1%8E
//https://ru.wikipedia.org/wiki/%D0%9A%D0%BE%D0%BD%D0%B5%D1%87%D0%BD%D1%8B%D0%B9_%D0%B0%D0%B2%D1%82%D0%BE%D0%BC%D0%B0%D1%82
//https://ru.wikipedia.org/wiki/%D0%A2%D0%B0%D0%B1%D0%BB%D0%B8%D1%86%D0%B0_%D0%BF%D1%80%D0%B8%D0%BD%D1%8F%D1%82%D0%B8%D1%8F_%D1%80%D0%B5%D1%88%D0%B5%D0%BD%D0%B8%D0%B9


namespace iridium {
namespace pattern {

/// \~english @brief Interface for a Finite State Machine (FSM).
///     This class template defines the core operation of an FSM, which involves
///     processing an incoming event and transitioning to a new state as a result.
///     Concrete FSM implementations will provide the specific state transition logic.
/// \~russian @brief Интерфейс для Конечного Автомата (Finite State Machine, FSM).
///     Этот шаблон класса определяет основную операцию КА, которая включает
///     обработку входящего события и переход в новое состояние в результате этого.
///     Конкретные реализации КА будут предоставлять специфическую логику переходов состояний.
/// \~english @tparam TEvent The type representing events that can trigger state transitions in the FSM.
/// \~russian @tparam TEvent Тип, представляющий события, которые могут вызывать переходы состояний в КА.
/// \~english @tparam TState The type representing the states of the FSM. This type is typically an enum or a class encapsulating state data.
/// \~russian @tparam TState Тип, представляющий состояния КА. Обычно это перечисление (enum) или класс, инкапсулирующий данные состояния.
template<typename TEvent, typename TState>
class IFSM {
public:
    /// \~english @brief Defines common interface elements for `IFSM`, such as smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.).
    /// \~russian @brief Определяет общие элементы интерфейса для `IFSM`, такие как typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.).
    DEFINE_INTERFACE(IFSM)
    /// \~english @brief Processes an incoming event and determines the next state of the FSM.
    ///     This method encapsulates the state transition logic of the FSM.
    /// \~russian @brief Обрабатывает входящее событие и определяет следующее состояние КА.
    ///     Этот метод инкапсулирует логику переходов состояний КА.
    /// \~english @param event The event that has occurred and needs to be processed by the FSM.
    /// \~russian @param event Событие, которое произошло и должно быть обработано КА.
    /// \~english @return The new state of the FSM after processing the event.
    /// \~russian @return Новое состояние КА после обработки события.
    virtual TState doAction(TEvent const &event) = 0;
};


} // pattern
} // iridium


#endif // HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34
