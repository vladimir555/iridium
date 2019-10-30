#ifndef HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34
#define HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34


#include "iridium/smart_ptr.h"


//https://ru.wikipedia.org/wiki/%D0%90%D0%B2%D1%82%D0%BE%D0%BC%D0%B0%D1%82_%D1%81_%D0%BC%D0%B0%D0%B3%D0%B0%D0%B7%D0%B8%D0%BD%D0%BD%D0%BE%D0%B9_%D0%BF%D0%B0%D0%BC%D1%8F%D1%82%D1%8C%D1%8E
//https://ru.wikipedia.org/wiki/%D0%9A%D0%BE%D0%BD%D0%B5%D1%87%D0%BD%D1%8B%D0%B9_%D0%B0%D0%B2%D1%82%D0%BE%D0%BC%D0%B0%D1%82
//https://ru.wikipedia.org/wiki/%D0%A2%D0%B0%D0%B1%D0%BB%D0%B8%D1%86%D0%B0_%D0%BF%D1%80%D0%B8%D0%BD%D1%8F%D1%82%D0%B8%D1%8F_%D1%80%D0%B5%D1%88%D0%B5%D0%BD%D0%B8%D0%B9


namespace iridium {
namespace pattern {


template<typename TEvent, typename TState>
class IFSM {
public:
    DEFINE_INTERFACE(IFSM)
    virtual TState doAction(TEvent const &event) = 0;
};


} // pattern
} // iridium


#endif // HEADER_FSM_732D8850_143F_4B86_ABD1_CB9CB17B9C34
