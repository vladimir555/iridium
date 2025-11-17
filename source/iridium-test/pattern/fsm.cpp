#include <iridium/testing/tester.h>

#include <iridium/enum.h>
#include <iridium/logging/logger.h>
#include <iridium/pattern/fsm.h>


using namespace std;


namespace iridium::pattern {


/*
  state / input |   push_button
  -----------------------------
  light_off     |   light_on
  light_on      |   light_off
*/


// todo: fsm table
// State | Action | Result State
// off   | push   | on
// on    | push   | off


DEFINE_ENUM(
    TState,
    LIGHT_OFF,
    LIGHT_ON
)

DEFINE_ENUM(
    TEvent,
    PUSH_BUTTON
)


class Light {
public:
    Light() {
        using namespace std::placeholders;
        m_fsm.addTransition(TEvent::PUSH_BUTTON, TState::LIGHT_OFF, TState::LIGHT_ON,
            std::bind(&Light::onEnable, this));
        m_fsm.addTransition(TEvent::PUSH_BUTTON, TState::LIGHT_ON, TState::LIGHT_OFF,
            std::bind(&Light::onDisable, this));
    }

    void doAction(TEvent const &event) {
        m_fsm.doAction(event);
    }

    std::list<std::string> getLastActions() const {
        return m_actions;
    }

private:
    void onEnable() {
        m_actions.push_back("enable  " + std::to_string(m_counter++));
    }

    void onDisable() {
        m_actions.push_back("disable " + std::to_string(m_counter++));
    }

    FSM<TState, TEvent> m_fsm { TState::LIGHT_OFF };
    std::list<std::string> m_actions;
    int m_counter = 0;
};


TEST(fsm_map) {
    Light light;

    light.doAction(TEvent::PUSH_BUTTON);
    light.doAction(TEvent::PUSH_BUTTON);
    light.doAction(TEvent::PUSH_BUTTON);

    ASSERT(
        std::list<std::string>({
            "enable  0",
            "disable 1",
            "enable  2"
        }),
        equal,
        light.getLastActions()
    );
}


} // namespace iridium::pattern


DEFINE_ENUM_CONVERT(iridium::pattern::TState)
DEFINE_ENUM_CONVERT(iridium::pattern::TEvent)


IMPLEMENT_ENUM(iridium::pattern::TState)
IMPLEMENT_ENUM(iridium::pattern::TEvent)


//#define DEFINE_FSM_SWOTCH(TState, TEvent)
