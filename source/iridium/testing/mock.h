#ifndef HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2
#define HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2


#include "iridium/smart_ptr.h"
#include "iridium/macros/va_args.h"

#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <string>
#include <any>


namespace iridium {
namespace testing {


// This file implements the following syntax:
//
//   ON_CALL(mock_object, Method(...))
//     .With(...) ?
//     .WillByDefault(...);
//
// where With() is optional and WillByDefault() must appear exactly
// once.
//
//   EXPECT_CALL(mock_object, Method(...))
//     .With(...) ?
//     .Times(...) ?
//     .InSequence(...) *
//     .WillOnce(...) *
//     .WillRepeatedly(...) ?
//     .RetiresOnSaturation() ? ;
//
// where all clauses are optional and WillOnce() can be repeated.


class Mock {
public:
    virtual ~Mock() = default;

    template<typename TSignature, typename ... TArgs>
    auto call(std::string const &name, TArgs && ... args) const;

//    class Expectation {
//    public:
//        Expectation() = default;
//        virtual ~Expectation() = default;

//        Expectation &with(std::function<bool(std::vector<std::any> const&)> pred);
//        Expectation &times(int expected_count = 1);
//        Expectation &inSequence();
//        Expectation &willOnce(std::function<std::any(std::vector<std::any>)> f);
//        Expectation &willRepeatedly(std::function<std::any(std::vector<std::any>)> f);
//        Expectation &retiresOnSaturation();

//        bool  checkCall(std::vector<std::any> const& args) const;
//        std::any invoke(std::vector<std::any> const& args);
//
//    private:
//        friend class Mock;
//        mutable int     actual_calls = 0;
//        int             expected_calls = -1;
//        bool            retire = false;
//
//        std::function<bool(std::vector<std::any> const&)>           arg_matcher;
//        std::vector<std::function<std::any(std::vector<std::any>)>> actions_once;
//        std::function<std::any(std::vector<std::any>)>              action_repeated;
//    };

    friend class Behavior;
    template<typename TSignature>
    class Behavior {
    public:
        Behavior(Mock &mock, std::string const &name);
        ///
        template<typename TLambda>
        Behavior &operator = (TLambda const &&l);
        ///
//        Behavior &operator , (Expectation const &) { return *this; };

    private:
        Mock               &m_mock;
        std::string const  &m_name;
    };

private:
    template<typename TSignature>
    void setBehavior(std::string const &name, std::function<TSignature> const &&f);

    mutable std::unordered_map<std::string, std::any> m_map_name_behavior;
};


// implementation


template<typename TSignature, typename ... TArgs>
auto Mock::call(std::string const &name, TArgs && ... args) const {
    auto i = m_map_name_behavior.find(name);
    if (i == m_map_name_behavior.end())
        throw std::runtime_error("mock calling error: unexpected call '" + name + "'");

    return std::any_cast< std::function<TSignature> >
        (i->second)(std::forward<TArgs>(args)...);
}


template<typename TSignature>
void Mock::setBehavior(std::string const &name, std::function<TSignature> const &&f) {
    m_map_name_behavior[name] = f;
}


template<typename TSignature>
Mock::Behavior<TSignature>::Behavior(Mock &mock, std::string const &name)
:
    m_mock(mock),
    m_name(name)
{}


//Mock::Expectation &Mock::Expectation::with() {
//    return *this;
//}
//
//
//Mock::Expectation &Mock::Expectation::times() {
//    return *this;
//}
//
//
//Mock::Expectation &Mock::Expectation::inSequence() {
//    return *this;
//}
//
//
//Mock::Expectation &Mock::Expectation::willOnce() {
//    return *this;
//}
//
//
//Mock::Expectation &Mock::Expectation::willRepeatedly() {
//    return *this;
//}


//Mock::Expectation &Mock::Expectation::retiresOnSaturation() {
//    return *this;
//}


template<typename TSignature>
template<typename TLambda>
Mock::Behavior<TSignature> &Mock::Behavior<TSignature>::operator = (TLambda const &&l) {
    m_mock.setBehavior<TSignature>(m_name, l);
    return *this;
}


} // testing
} // iridium


#define DEFINE_MOCK_METHOD_2(ReturnType, MethodName) \
    ReturnType MethodName() override { \
        return this->call<ReturnType()>(#MethodName); \
    }

#define DEFINE_MOCK_METHOD_3(ReturnType, MethodName, A1) \
    ReturnType MethodName(A1 a1) override { \
        return this->call<ReturnType(A1)>(#MethodName, a1); \
    }

#define DEFINE_MOCK_METHOD_4(ReturnType, MethodName, A1, A2) \
public: \
    ReturnType MethodName(A1 a1, A2 a2) override { \
        return this->call<ReturnType(A1, A2)>(#MethodName, a1, a2); \
    }

#define DEFINE_MOCK_METHOD_5(ReturnType, MethodName, A1, A2, A3) \
    ReturnType MethodName(A1 a1, A2 a2, A3 a3) override { \
        return this->call<ReturnType(A1, A2, A3)>(#MethodName, a1, a2, a3); \
    }

#define DEFINE_MOCK_METHOD_6(ReturnType, MethodName, A1, A2, A3, A4) \
    ReturnType MethodName(A1 a1, A2 a2, A3 a3, A4 a4) override { \
        return this->call<ReturnType(A1, A2, A3, A4)>(#MethodName, a1, a2, a3, a4); \
    }

#define DEFINE_MOCK_METHOD(...) \
    dMACRO_CHOOSER(DEFINE_MOCK_METHOD, __VA_ARGS__)(__VA_ARGS__)

#define DEFINE_MOCK_METHOD_CONST_2(ReturnType, MethodName) \
    ReturnType MethodName() const override { \
        return this->call<ReturnType()>(#MethodName); \
    }

#define DEFINE_MOCK_METHOD_CONST_3(ReturnType, MethodName, A1) \
    ReturnType MethodName(A1 a1) const override { \
        return this->call<ReturnType(A1)>(#MethodName, a1); \
    }

#define DEFINE_MOCK_METHOD_CONST_4(ReturnType, MethodName, A1, A2) \
public: \
    ReturnType MethodName(A1 a1, A2 a2) const override { \
        return this->call<ReturnType(A1, A2)>(#MethodName, a1, a2); \
    }

#define DEFINE_MOCK_METHOD_CONST_5(ReturnType, MethodName, A1, A2, A3) \
    ReturnType MethodName(A1 a1, A2 a2, A3 a3) const override { \
        return this->call<ReturnType(A1, A2, A3)>(#MethodName, a1, a2, a3); \
    }

#define DEFINE_MOCK_METHOD_CONST_6(ReturnType, MethodName, A1, A2, A3, A4) \
    ReturnType MethodName(A1 a1, A2 a2, A3 a3, A4 a4) const override { \
        return this->call<ReturnType(A1, A2, A3, A4)>(#MethodName, a1, a2, a3, a4); \
    }

#define DEFINE_MOCK_METHOD(...) \
    dMACRO_CHOOSER(DEFINE_MOCK_METHOD, __VA_ARGS__)(__VA_ARGS__)

#define DEFINE_MOCK_METHOD_CONST(...) \
    dMACRO_CHOOSER(DEFINE_MOCK_METHOD_CONST, __VA_ARGS__)(__VA_ARGS__)

#define DEFINE_MOCK_CLASS(Interface) \
class Mock##Interface: public ::iridium::testing::Mock, public Interface

#define DEFINE_MOCK_CONSTRUCTOR(Interface) \
public: \
template<typename ... TArgs> \
Mock##Interface(TArgs ... args): Interface(args ...) {};

#define DEFINE_MOCK_BEHAVIOR(TResult, mockMethod, mock_object, ...) \
    ::iridium::testing::Mock::Behavior<TResult(__VA_ARGS__)>(mock_object, #mockMethod) = [&] (__VA_ARGS__)

#define DEFINE_MOCK_EXPECTATION() \
    , ::iridium::testing::Mock::Expectation()


#endif // HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2
