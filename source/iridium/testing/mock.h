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


//class IMock {
//public:
//    DEFINE_INTERFACE(IMock);
//
//    virtual void setupBehavior  (std::string const &method_name, std::function<void()> const &behavior) = 0;
//    virtual void callMethod     (std::string const &method_name) = 0;
//    virtual void verifyCall     (std::string const &method_name, int const &expected_calls = 1) const = 0;
//};


class Mock {
public:
    virtual ~Mock() = default;
//    DEFINE_IMPLEMENTATION(Mock)

    template<typename TSignature, typename ... TArgs>
    auto call(std::string const &name, TArgs && ... args) const;

    template<typename TSignature>
    void setBehavior(std::string const &name, std::function<TSignature> const &&f);

    template<typename TSignature>
    class Behavior {
    public:
        Behavior(Mock &mock, std::string const &name);

        Behavior &operator = (std::function<TSignature> const &&f);

        template<typename TLambda>
        Behavior &operator = (TLambda const &&l);

    private:
        Mock              &m_mock;
        std::string const  &m_name;
    };

private:
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


template<typename TSignature>
Mock::Behavior<TSignature> &Mock::Behavior<TSignature>::operator = (std::function<TSignature> const &&f)
{
//            mock_.setBehavior<TSignature>(name_, std::move(f));
    m_mock.setBehavior<TSignature>(m_name, f);
    return *this;
}


template<typename TSignature>
template<typename TLambda>
Mock::Behavior<TSignature> &Mock::Behavior<TSignature>::operator = (TLambda const &&l) {
//            std::function<TSignature> f = std::forward<TLambda>(l);
//            m_mock.setBehavior<TSignature>(m_name, std::move(f));
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

#define DEFINE_MOCK_BEHAVIOR(TResult, mockMethod, mock_object, ...) \
    ::iridium::testing::Mock::Behavior<TResult(__VA_ARGS__)>(mock_object, #mockMethod) = [&] (__VA_ARGS__)


#endif // HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2
