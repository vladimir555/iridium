// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34
#define HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34


#include "iridium/smart_ptr.h"

#include <unordered_map>
#include <functional>
//#include <>


namespace iridium::pattern {


template<typename TState, typename TEvent>
class FSM {
public:
    DEFINE_CREATE(FSM)
    using THandler = std::function<void()>;
    struct TTransition {
        TState   from; ///< \~english The source state. \~russian Исходное состояние.
        TEvent   event; ///< \~english The event that triggers the transition. \~russian Событие, которое вызывает переход.
        TState   to; ///< \~english The destination state. \~russian Конечное состояние.
        THandler handler; ///< \~english The handler to be executed on transition. \~russian Обработчик, который будет выполнен при переходе.
    };
    FSM(TState const &initial_state);
    void addTransition(
        TEvent   const &event,
        TState   const &from,
        TState   const &to,
        THandler const &handler = {});
    TState doAction(TEvent const &event);

private:
    /// \~english @brief A hash function for a pair of state and event.
    /// \~russian @brief Хеш-функция для пары состояния и события.
    struct TPairStateEventHash {
        size_t operator()(std::pair<TState, TEvent> const &p) const;
    };

    /// \~english @brief The current state of the FSM.
    /// \~russian @brief Текущее состояние FSM.
    TState m_state;
    /// \~english @brief The transition table of the FSM.
    /// \~russian @brief Таблица переходов FSM.
    std::unordered_map<std::pair<TState, TEvent>, TTransition, TPairStateEventHash> m_table;
};


// implementation


template<typename TState, typename TEvent>
FSM<TState, TEvent>::FSM(TState const &initial_state)
:
    m_state(initial_state)
{}


template<typename TState, typename TEvent>
void FSM<TState, TEvent>::addTransition(
    TEvent   const &event,
    TState   const &from,
    TState   const &to,
    THandler const &handler)
{
    auto key = std::make_pair(from, event);

    if (m_table.find(key) != m_table.end()) {
        throw std::runtime_error(
            "fsm adding transition error: duplicate transition for state '" +
            convertion::convert<std::string>(from)  + "' and event '" +
            convertion::convert<std::string>(event) + "'");
    }

    m_table[key] = TTransition{from, event, to, handler};
}


template<typename TState, typename TEvent>
TState FSM<TState, TEvent>::doAction(TEvent const &event) {
    auto key    = std::make_pair(m_state, event);
    auto i      = m_table.find(key);

    if (i == m_table.end())
        throw std::runtime_error(
            "fsm action transit error: transition not found for event '" +
            convertion::convert<std::string>(event) + "' on state '" +
            convertion::convert<std::string>(m_state) + "'");

    if (i->second.handler)
        i->second.handler();

    m_state = i->second.to;

    return m_state;
}


template<typename TState, typename TEvent>
size_t FSM<TState, TEvent>::TPairStateEventHash::operator()(std::pair<TState, TEvent> const &p) const {
    size_t h1 = std::hash<TState> {} (p.first);
    size_t h2 = std::hash<TEvent> {} (p.second);
    return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
};


} // namespace iridium::pattern


#endif // HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34
