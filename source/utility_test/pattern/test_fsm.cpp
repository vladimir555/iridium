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


DEFINE_ENUM(
    TState,
    LIGHT_OFF,
    LIGHT_ON
)

DEFINE_ENUM(
    TEvent,
    PUSH_BUTTON
)


class Light: public implementation::CFSM<TEvent, TState> {
public:
    Light();
    virtual ~Light() = default;

// todo:
//    template<TEvent::TEnumInternal, TState::TEnumInternal, TState::TEnumInternal>
//    class Handler {
//    public:
//        Handler(Light *parent): m_parent(parent) {}
//       ~Handler() = default;
//        void handle();
//    private:
//        Light *m_parent;
//    };
//    Handler<TEvent::PUSH_BUTTON, TState::LIGHT_OFF, TState::LIGHT_ON> h = this;

private:
    template<TEvent::TEnumInternal, TState::TEnumInternal, TState::TEnumInternal>
    void handle();

    int i = 0;
};


//template<>
//void Light::Handler<TEvent::PUSH_BUTTON, TState::LIGHT_OFF, TState::LIGHT_ON>::handle() {
//}


template<>
void Light::handle<TEvent::PUSH_BUTTON, TState::LIGHT_OFF, TState::LIGHT_ON>() {
    cout << "enable  " << i << std::endl;
    i++;
}


template<>
void Light::handle<TEvent::PUSH_BUTTON, TState::LIGHT_ON, TState::LIGHT_OFF>() {
    cout << "disable " << i << std::endl;
    i++;
}


Light::Light()
:
    implementation::CFSM<TEvent, TState>(TState::LIGHT_OFF, 5,
    implementation::CFSM<TEvent, TState>::TTransitions({
        DEFINE_FSM_MAP_TRANSITION(TEvent::PUSH_BUTTON, TState::LIGHT_OFF, TState::LIGHT_ON),
        DEFINE_FSM_MAP_TRANSITION(TEvent::PUSH_BUTTON, TState::LIGHT_ON , TState::LIGHT_OFF)
    }))
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


IMPLEMENT_ENUM(utility::pattern::TState)
IMPLEMENT_ENUM(utility::pattern::TEvent)


//#define DEFINE_FSM_SWOTCH(TState, TEvent)
