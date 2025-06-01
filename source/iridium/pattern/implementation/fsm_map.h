// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FSM_MAP_7D416CBE_F331_40CD_A51A_E2C41D1FF072
#define HEADER_FSM_MAP_7D416CBE_F331_40CD_A51A_E2C41D1FF072


#include "iridium/pattern/fsm.h"
#include "iridium/convertion/convert.h"


namespace iridium {
namespace pattern {
/// \~english @brief Contains concrete implementations of common software design patterns defined in the parent `iridium::pattern` namespace.
/// \~russian @brief Содержит конкретные реализации распространенных шаблонов проектирования, определенных в родительском пространстве имен `iridium::pattern`.
namespace implementation {


// -----interface

/// \~english @brief A concrete implementation of the `IFSM` interface using a transition list (map-like behavior).
///     This Finite State Machine manages states and transitions based on a predefined list of `TTransition` objects.
///     It supports associating event handlers with specific transitions and can maintain a history of transitions.
/// \~russian @brief Конкретная реализация интерфейса `IFSM`, использующая список переходов (поведение, подобное карте).
///     Этот Конечный Автомат управляет состояниями и переходами на основе предопределенного списка объектов `TTransition`.
///     Он поддерживает связывание обработчиков событий с конкретными переходами и может вести историю переходов.
/// \~english @tparam TEvent The type representing events that can trigger state transitions. Must be comparable for equality.
/// \~russian @tparam TEvent Тип, представляющий события, которые могут вызывать переходы состояний. Должен быть сравним на равенство.
/// \~english @tparam TState The type representing the states of the FSM. Must be comparable for equality. Typically an enum.
/// \~russian @tparam TState Тип, представляющий состояния КА. Должен быть сравним на равенство. Обычно это перечисление (enum).
template<typename TEvent, typename TState>
class CFSM: public IFSM<TEvent, TState> {
public:
    /// \~english @brief A function wrapper for transition event handlers. Handlers are parameterless void functions.
    /// \~russian @brief Обертка функции для обработчиков событий перехода. Обработчики - это функции void без параметров.
    typedef std::function<void()>   THandler;
    /// \~english @brief Represents a single transition rule in the Finite State Machine.
    /// \~russian @brief Представляет одно правило перехода в Конечном Автомате.
    struct TTransition {
        /// \~english @brief The event that triggers this transition.
        /// \~russian @brief Событие, которое вызывает этот переход.
        TEvent      event;
        /// \~english @brief The source state from which this transition occurs.
        /// \~russian @brief Исходное состояние, из которого происходит этот переход.
        TState      from;
        /// \~english @brief The target state to which the FSM moves after this transition.
        /// \~russian @brief Целевое состояние, в которое КА переходит после этого перехода.
        TState      to;
        /// \~english @brief An optional handler function to be executed when this transition occurs.
        /// \~russian @brief Опциональная функция-обработчик, которая будет выполнена при этом переходе.
        THandler    handler;
    };
    /// \~english @brief A list of `TTransition` objects defining all possible state transitions for the FSM.
    /// \~russian @brief Список объектов `TTransition`, определяющий все возможные переходы состояний для КА.
    typedef std::list<TTransition>  TTransitions;

    /// \~english @brief Defines standard implementation helpers for `CFSM`, such as smart pointer typedefs and a static `create` method.
    /// \~russian @brief Определяет стандартные вспомогательные средства реализации для `CFSM`, такие как typedef-ы умных указателей и статический метод `create`.
    DEFINE_CREATE(CFSM)
    /// \~english @brief Constructs a Finite State Machine.
    /// \~russian @brief Конструирует Конечный Автомат.
    /// \~english @param start_state The initial state of the FSM.
    /// \~russian @param start_state Начальное состояние КА.
    /// \~english @param history_size The maximum number of transitions to keep in the history.
    /// \~russian @param history_size Максимальное количество переходов для хранения в истории.
    /// \~english @param transitions A list of all valid transitions for this FSM.
    /// \~russian @param transitions Список всех допустимых переходов для этого КА.
    CFSM(TState const &start_state, size_t const &history_size, TTransitions const &transitions);
    /// \~english @brief Virtual default destructor.
    /// \~russian @brief Виртуальный деструктор по умолчанию.
    virtual ~CFSM() = default;

    /// \~english @brief Processes an event, attempts to find a matching transition, and updates the FSM's state.
    ///     If a valid transition is found (matching current state and event), the associated handler (if any) is executed,
    ///     the state is updated, and the transition is recorded in history.
    /// \~russian @brief Обрабатывает событие, пытается найти соответствующий переход и обновляет состояние КА.
    ///     Если найден допустимый переход (соответствующий текущему состоянию и событию), выполняется связанный обработчик (если есть),
    ///     состояние обновляется, и переход записывается в историю.
    /// \~english @param event The event to process.
    /// \~russian @param event Событие для обработки.
    /// \~english @return The new state of the FSM after processing the event.
    /// \~russian @return Новое состояние КА после обработки события.
    /// \~english @exception std::runtime_error if no transition is found for the current state and given event.
    /// \~russian @exception std::runtime_error если не найден переход для текущего состояния и данного события.
    TState doAction(TEvent const &event) override;
    /// \~english @brief Retrieves the history of transitions.
    /// \~russian @brief Получает историю переходов.
    /// \~english @return A list of `TTransition` objects representing the recorded history, up to `history_size`.
    /// \~russian @return Список объектов `TTransition`, представляющий записанную историю, размером до `history_size`.
    TTransitions getHistory() const;

private:
    /// \~english @brief The current state of the Finite State Machine.
    /// \~russian @brief Текущее состояние Конечного Автомата.
    TState          m_state;
    /// \~english @brief The list of all defined transitions for this FSM.
    /// \~russian @brief Список всех определенных переходов для этого КА.
    TTransitions    m_transitions;
    /// \~english @brief A list storing the history of executed transitions, limited by `m_history_size`.
    /// \~russian @brief Список, хранящий историю выполненных переходов, ограниченный `m_history_size`.
    TTransitions    m_history;
    /// \~english @brief The maximum number of transitions to store in the `m_history` list.
    /// \~russian @brief Максимальное количество переходов для хранения в списке `m_history`.
    size_t          m_history_size;
};


// -----implementation


template<typename TEvent, typename TState>
CFSM<TEvent, TState>::CFSM(TState const &start_state, size_t const &history_size, TTransitions const &transitions)
:
    m_state         (start_state),
    m_transitions   (transitions),
    m_history_size  (history_size)
{}


template<typename TEvent, typename TState>
TState CFSM<TEvent, TState>::doAction(TEvent const &event) {
    for (auto const &i: m_transitions) {
        if (i.event == event && i.from == m_state) {
            if (i.handler)
                i.handler();
            m_state =  i.to;
            m_history.push_back(i);
            if (m_history.size() > m_history_size)
                m_history.pop_front();
            return m_state; // ----->
        }
    }
    throw std::runtime_error("fsm error: transition not found for state '"  +
        convertion::convert<std::string>(m_state) + "' with event '" +
        convertion::convert<std::string>(event)); // ----->
}


template<typename TEvent, typename TState>
typename CFSM<TEvent, TState>::TTransitions CFSM<TEvent, TState>::getHistory() const {
    return m_history; // ----->
}


} // implementation
} // pattern
} // iridium


/// \~english @def DEFINE_FSM_MAP_TRANSITION(Event, StateFrom, StateTo)
/// @brief Helper macro to define a `TTransition` structure, typically for initializing the transitions list for `CFSM`.
///     It assumes that the class in which this macro is used has a template method
///     `handle<Event, StateFrom, StateTo>()` which will serve as the transition handler.
///     This handler is bound to the current instance (`this`) using `std::bind`.
/// \~russian @def DEFINE_FSM_MAP_TRANSITION(Event, StateFrom, StateTo)
/// @brief Вспомогательный макрос для определения структуры `TTransition`, обычно для инициализации списка переходов для `CFSM`.
///     Предполагается, что класс, в котором используется этот макрос, имеет шаблонный метод
///     `handle<Event, StateFrom, StateTo>()`, который будет служить обработчиком перехода.
///     Этот обработчик привязывается к текущему экземпляру (`this`) с помощью `std::bind`.
/// \~english @param Event The specific event value that triggers this transition.
/// \~russian @param Event Конкретное значение события, которое вызывает этот переход.
/// \~english @param StateFrom The source state value from which this transition occurs.
/// \~russian @param StateFrom Значение исходного состояния, из которого происходит этот переход.
/// \~english @param StateTo The target state value to which the FSM moves after this transition.
/// \~russian @param StateTo Значение целевого состояния, в которое КА переходит после этого перехода.
#define DEFINE_FSM_MAP_TRANSITION(Event, StateFrom, StateTo) \
{ Event, StateFrom, StateTo, std::bind(&std::remove_pointer<decltype(this)>::type::handle<Event, StateFrom, StateTo>, this) }


#endif // HEADER_FSM_MAP_7D416CBE_F331_40CD_A51A_E2C41D1FF072
