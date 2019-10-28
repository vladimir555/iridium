#ifndef HEADER_FSM_MAP_7D416CBE_F331_40CD_A51A_E2C41D1FF072
#define HEADER_FSM_MAP_7D416CBE_F331_40CD_A51A_E2C41D1FF072


#include "iridium/pattern/fsm.h"
#include "iridium/convertion/convert.h"


namespace iridium {
namespace pattern {
namespace implementation {


// -----interface


template<typename TEvent, typename TState>
class CFSM: public IFSM<TEvent, TState> {
public:
    typedef std::function<void()>   THandler;
    struct TTransition {
        TEvent      event;
        TState      from;
        TState      to;
        THandler    handler;
    };
    typedef std::list<TTransition>  TTransitions;

    DEFINE_CREATE(CFSM)
    CFSM(TState const &start_state, size_t const &history_size, TTransitions const &transitions);
    virtual ~CFSM() = default;

    TState doAction(TEvent const &event) override;
    TTransitions getHistory() const;

private:
    TState          m_state;
    TTransitions    m_transitions;
    TTransitions    m_history;
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


#define DEFINE_FSM_MAP_TRANSITION(Event, StateFrom, StateTo) \
{ Event, StateFrom, StateTo, std::bind(&std::remove_pointer<decltype(this)>::type::handle<Event, StateFrom, StateTo>, this) }


#endif // HEADER_FSM_MAP_7D416CBE_F331_40CD_A51A_E2C41D1FF072
