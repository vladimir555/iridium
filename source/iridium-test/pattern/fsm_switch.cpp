#include "fsm_switch.h"

#include <iostream>


IMPLEMENT_ENUM(iridium::pattern::test::Light::TState)
IMPLEMENT_ENUM(iridium::pattern::test::Light::TEvent)


namespace iridium {
namespace pattern {
namespace test {


#define IMPLEMENT_FSM_SWITCH(TClass) \
Light::TState Light::doAction(TEvent const &event) { \
    auto exception = [this, event]() -> std::runtime_error { return std::runtime_error("fsm error: transition not found for state '" + \
        iridium::convertion::convert<std::string>(m_state) + "' with event '" + \
        iridium::convertion::convert<std::string>(event)); }; \
    switch (m_state) {

#define IMPLEMENT_FSM_SWITCH_TRANSITION(Event, StateFrom, StateTo)

template<>
void Light::handle<Light::TEvent::PUSH_BUTTON, Light::TState::LIGHT_OFF, Light::TState::LIGHT_ON>() {
    actions.push_back("enable  " + convertion::convert<std::string>(i));
    i++;
}


template<>
void Light::handle<Light::TEvent::PUSH_BUTTON, Light::TState::LIGHT_ON, Light::TState::LIGHT_OFF>() {
    actions.push_back("disable " + convertion::convert<std::string>(i));
    i++;
}


Light::TState Light::doAction(TEvent const &event) {
    auto exception = [this, event]() -> std::runtime_error { return std::runtime_error("fsm error: transition not found for state '" +
        iridium::convertion::convert<std::string>(m_state) + "' with event '" +
        iridium::convertion::convert<std::string>(event)); };
    switch (m_state) {
    case TState::LIGHT_OFF:
        switch (event) {
        case TEvent::PUSH_BUTTON:
            handle<TEvent::PUSH_BUTTON, TState::LIGHT_OFF, TState::LIGHT_ON>();
            m_state = TState::LIGHT_ON;
            return m_state;
        default:
            throw exception();
        }
    case TState::LIGHT_ON:
        switch (event) {
        case TEvent::PUSH_BUTTON:
            handle<TEvent::PUSH_BUTTON, TState::LIGHT_ON, TState::LIGHT_OFF>();
            m_state = TState::LIGHT_OFF;
            return m_state;
        default:
            throw exception();
        }
    default:
        throw exception();
    }
    //throw exception();
}


} // test
} // pattern
} // iridium
