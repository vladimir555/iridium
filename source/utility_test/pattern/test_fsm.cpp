#include <gtest/gtest.h>
#include <iostream>
#include <map>
#include <functional>
#include <tuple>
#include <list>


#include <utility/convertion/convert.h>
#include <utility/enum.h>
#include <utility/smart_ptr.h>
#include <utility/logging/logger.h>
#include <utility/pattern/implementation/fsm_map.h>

#include "fsm_switch.h"


using namespace std;



namespace utility {
namespace pattern {


/*
  state / input |   push_button
  -----------------------------
  light_off     |   light_on
  light_on      |   light_off
*/


class Light {
public:
    Light();
    virtual ~Light() = default;

    DEFINE_ENUM(
        TState,
        LIGHT_OFF,
        LIGHT_ON
    )

    DEFINE_ENUM(
        TEvent,
        PUSH_BUTTON
    )

    TState doAction(TEvent const &event);

private:
    template<TEvent::TEnumInternal, TState::TEnumInternal, TState::TEnumInternal>
    void handle();

    IFSM<TState, TEvent>::TSharedPtr m_fsm;
    int i = 0;
};


Light::TState Light::doAction(TEvent const &event) {
    return m_fsm->doAction(event);
}


template<>
void Light::handle<Light::TEvent::PUSH_BUTTON, Light::TState::LIGHT_OFF, Light::TState::LIGHT_ON>() {
    cout << "enable  " << i << std::endl;
    i++;
}

template<>
void Light::handle<Light::TEvent::PUSH_BUTTON, Light::TState::LIGHT_ON, Light::TState::LIGHT_OFF>() {
    cout << "disable " << i << std::endl;
    i++;
}


Light::Light()
:
    m_fsm(
    implementation::CFSM<Light>::create(TState::LIGHT_OFF, 5,
    implementation::CFSM<Light>::TTransitions({
        DEFINE_FSM_MAP_TRANSITION(TEvent::PUSH_BUTTON, TState::LIGHT_OFF, TState::LIGHT_ON),
        DEFINE_FSM_MAP_TRANSITION(TEvent::PUSH_BUTTON, TState::LIGHT_ON , TState::LIGHT_OFF)
    })))
{}


TEST(pattern, fsm_map) {
    logging::update(true);
    Light light;

    light.doAction(test::Light::TEvent::PUSH_BUTTON);
    light.doAction(test::Light::TEvent::PUSH_BUTTON);
    light.doAction(test::Light::TEvent::PUSH_BUTTON);
}


TEST(pattern, fsm_switch) {
    logging::update(true);
    test::Light light;

    light.doAction(test::Light::TEvent::PUSH_BUTTON);
    light.doAction(test::Light::TEvent::PUSH_BUTTON);
    light.doAction(test::Light::TEvent::PUSH_BUTTON);
}


} // pattern
} // utility


IMPLEMENT_ENUM(utility::pattern::Light::TState)
IMPLEMENT_ENUM(utility::pattern::Light::TEvent)


//#define DEFINE_FSM_SWOTCH(TState, TEvent)
