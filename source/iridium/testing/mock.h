#ifndef HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2
#define HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2


//#include "mock_registry.h"

#include "iridium/macros/va_args.h"
#include "iridium/items.h"

#include <typeinfo>
#include <functional>
#include <any>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "iridium/pattern/non_copyable.h"
#include "iridium/pattern/non_movable.h"


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


template<typename TClassMock>
class MockSequence;


template<typename TClass>
class Mock {
public:
    using TOriginalClass = TClass;

    Mock();
    virtual ~Mock() = default;

    template<typename ... TArgs>
    static ::std::shared_ptr<TClass> create(TArgs && ... args);

    friend class Behavior;
    template<typename TResult, typename ... TArgs>
    class Behavior;

    template<typename TResult, typename ... TArgs>
    class Behavior<TResult(TClass::*)(TArgs...)>:
        public pattern::NonCopyable, public pattern::NonMovable
    {
    public:
        explicit Behavior(Mock &mock, std::type_info const *method);

        template<typename TLambda>
        Behavior &operator=(TLambda &&l);

    private:
        Mock &m_mock;
        std::type_info const *m_method;
    };

    template<typename TResult, typename ... TArgs>
    class Behavior<TResult(TClass::*)(TArgs...) const>:
        public pattern::NonCopyable, public pattern::NonMovable
    {
    public:
        explicit Behavior(Mock &mock, std::type_info const *method);

        template<typename TLambda>
        Behavior &operator=(TLambda &&l);

    private:
        Mock &m_mock;
        std::type_info const *m_method;
    };

protected:
    template<typename TSignature, typename ... TArgs>
    auto call(std::type_info const *method, TArgs && ... args) const;

private:
    template<typename TResult, typename ... TArgs>
    void setBehavior(std::type_info const *method, std::function<TResult(TArgs...)> const &&f);

    template<typename TResult, typename ... TArgs>
    void setBehavior(std::type_info const *method, std::function<TResult(TArgs...) const> const &&f);

    mutable std::unordered_map<std::type_info const *, std::any>   m_map_name_behavior;
    mutable std::unordered_map<void *, std::type_info const *>     m_map_behavior_name;

    static std::list<Mock<TClass> *>    m_mock_objects;
    static bool                         m_is_mocked;
};


template<typename TClassMock>
class MockSequence {
public:
    MockSequence(TClassMock &mock, std::string const &file_line)
    :
        m_mock      (mock),
        m_file_line (file_line)
    {}

    template<typename TResult, typename ... TMethodArgs, typename... TCallArgs>
    MockSequence &addExpectation(
        std::string const &file_line,
        size_t      const &call_count_max,
        size_t      const &call_count_min,
        TResult (TClassMock::*method)(TMethodArgs...),
        TCallArgs && ... args)
    {
        TExpectation expectation(
            file_line,
            call_count_min,
            call_count_max,
           &typeid(static_cast<TResult (TClassMock::TOriginalClass::*)(TMethodArgs...)>(method)),
            std::tuple<std::decay_t<TMethodArgs> ...>(std::forward<TCallArgs>(args) ...));
        m_expectations.push_back(expectation);
        return *this;
    }

    template<typename TResult, typename... TMethodArgs, typename... TCallArgs>
    MockSequence &addExpectation(
        std::string const &file_line,
        size_t      const &call_count_max,
        size_t      const &call_count_min,
        TResult (TClassMock::*method)(TMethodArgs...) const,
        TCallArgs && ... args)
    {
        TExpectation expectation(
            file_line,
            call_count_min,
            call_count_max,
           &typeid(static_cast<TResult (TClassMock::TOriginalClass::*)(TMethodArgs...)>(method)),
            std::tuple<std::decay_t<TMethodArgs> ...>(std::forward<TCallArgs>(args) ...));
        m_expectations.push_back(expectation);
        return *this;
    }

    template<typename TClass, typename TResult, typename... TMethodArgs, typename... TCallArgs>
    void step(
        TResult (TClass::*method)(TMethodArgs...),
        TCallArgs && ... args)
    {
        if (m_expectations.empty()) {
            throw std::runtime_error(std::string("sequence at '") + m_file_line + "' expectation '" +
                typeid(TClass).name() + "' error: unexpected call '" +
                typeid(method).name() + ", sequence is empty");
        }
        auto expectation = m_expectations.front();
//        if (expectation.arguments.has_value() &&
//            expectation.arguments != std::any(std::tuple<std::decay_t<TMethodArgs> ...>(std::forward<TCallArgs>(args) ...)))
//        {
//            throw std::runtime_error(std::string("sequence at '") + expectation.m_file_line + "' expectation '" +
//                typeid(TClass).name() + "' error: unexpected call '" +
//                typeid(method).name() + "' arguments");
//        }
        if (expectation.call_count > 0)
            expectation.call_count--;
        else
            m_expectations.pop_front();
    }

private:
    struct TExpectation {
        TExpectation(
            std::string     const file_line,
            size_t          const &call_count_min,
            size_t          const &call_count_max,
            std::type_info  const *method_name,
            std::any        const &arguments)
        :
            file_line       (file_line),
            call_count_min  (call_count_min),
            call_count_max  (call_count_max),
            method_name     (method_name),
            arguments       (arguments),
            call_count      (0)
        {}
        std::string             const file_line;
        size_t                  const call_count_min;
        size_t                  const call_count_max;
        std::type_info  const * const method_name;
        std::any                const arguments;
        size_t                        call_count;
    };

    TClassMock             &m_mock;
    std::string             m_file_line;
    std::list<TExpectation> m_expectations;
};


// implementation


template<typename TClass>
std::list<Mock<TClass> *> Mock<TClass>::m_mock_objects;


template<typename TClass>
bool Mock<TClass>::m_is_mocked = false;


template<typename TClass>
Mock<TClass>::Mock() {
    m_is_mocked = true;
    m_mock_objects.push_back(dynamic_cast<Mock<TClass> *>(this));
}


template<typename TClass>
template<typename ... TArgs>
::std::shared_ptr<TClass> Mock<TClass>::create(TArgs && ... args) {
    if (m_is_mocked) {
        if (m_mock_objects.empty()) {
            throw std::runtime_error(
                "getting regeistered mock of class '" + std::string(typeid(TClass).name()) +
                "' object error: not enough such registered mock objects");
        } else {
            auto mock = m_mock_objects.front();
            m_mock_objects.pop_front();
            // shared_ptr without destructor
            return std::shared_ptr<TClass>(dynamic_cast<TClass *>(mock), [] (TClass *) {});
        }
    } else {
        return ::std::make_shared<TClass>(std::forward<TArgs>(args)...);
    }
}


template<typename TClass>
template<typename TSignature, typename ... TArgs>
auto Mock<TClass>::call(std::type_info const *method, TArgs && ... args) const {
    auto i = m_map_name_behavior.find(method);
    if (i == m_map_name_behavior.end())
        throw std::runtime_error("mock calling error: unexpected call '" + std::string(method->name()) + "'");

    using TMethod = std::function<TSignature>;

    try {
        const auto& method = std::any_cast<const TMethod&>(i->second);
        return method(std::forward<TArgs>(args)...);
    } catch (const std::bad_any_cast&) {
        throw std::runtime_error("mock calling error: type mismatch for method '" + std::string(method->name()) + "'");
    }
}


template<typename TClass>
template<typename TResult, typename ... TArgs>
void Mock<TClass>::setBehavior(std::type_info const *method, std::function<TResult(TArgs...)> const &&f) {
//    printf("set behavior: %s\t[%p]\n", method->name(), static_cast<const void*>(method));
    m_map_name_behavior[method] = f;
}


template<typename TClass>
template<typename TResult, typename ... TArgs>
void Mock<TClass>::setBehavior(std::type_info const *method, std::function<TResult(TArgs...) const> const &&f) {
//    printf("set behavior const: %s\t[%p]\n", method->name(), static_cast<const void*>(method));
    m_map_name_behavior[method] = f;
}


template<typename TClass>
template<typename TResult, typename ... TArgs>
Mock<TClass>::Behavior<TResult(TClass::*)(TArgs...)>::Behavior(Mock &mock, std::type_info const *method)
:
    m_mock  (mock),
    m_method(method)
{
//    printf("behavior constructor      : %s\t[%p]\n", method->name(), static_cast<const void*>(method));
}


template<typename TClass>
template<typename TResult, typename ... TArgs>
template<typename TLambda>
typename Mock<TClass>::template Behavior<TResult(TClass::*)(TArgs...)>
&Mock<TClass>::Behavior<TResult(TClass::*)(TArgs...)>::operator = (TLambda &&l)
{
    m_mock.setBehavior<TResult, TArgs...>(m_method, std::function<TResult(TArgs...)>(std::forward<TLambda>(l)));
    return *this;
}


template<typename TClass>
template<typename TResult, typename ... TArgs>
Mock<TClass>::Behavior<TResult(TClass::*)(TArgs...) const>::Behavior(Mock &mock, std::type_info const *method)
:
    m_mock  (mock),
    m_method(method)
{
//    printf("behavior constructor const: %s\t[%p]\n", method->name(), static_cast<const void*>(method));
}


template<typename TClass>
template<typename TResult, typename ... TArgs>
template<typename TLambda>
typename Mock<TClass>::template Behavior<TResult(TClass::*)(TArgs...) const>
&Mock<TClass>::Behavior<TResult(TClass::*)(TArgs...) const>::operator = (TLambda &&l)
{
    m_mock.setBehavior<TResult, TArgs...>(m_method, std::function<TResult(TArgs...)>(std::forward<TLambda>(l)));
    return *this;
}


} // testing
} // iridium


#define DEFINE_MOCK_METHOD_2(TResult, methodName) \
public: \
    TResult methodName() override { \
        return this->call<TResult()>(&typeid(TResult (TOriginalClass::*)())); \
    }

#define DEFINE_MOCK_METHOD_3(TResult, methodName, A1) \
public: \
    TResult methodName(A1 a1) override { \
        return this->call<TResult(A1)>(&typeid(TResult (TOriginalClass::*)(A1)), a1); \
    }

#define DEFINE_MOCK_METHOD_4(TResult, methodName, A1, A2) \
public: \
    TResult methodName(A1 a1, A2 a2) override { \
        return this->call<TResult(A1, A2)>(&typeid(TResult (TOriginalClass::*)(A1, A2)), a1, a2); \
    }

#define DEFINE_MOCK_METHOD_5(TResult, methodName, A1, A2, A3) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3) override { \
        return this->call<TResult(A1, A2, A3)>(&typeid(TResult (TOriginalClass::*)(A1, A2, A3)), a1, a2, a3); \
    }

#define DEFINE_MOCK_METHOD_6(TResult, methodName, A1, A2, A3, A4) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3, A4 a4) override { \
        return this->call<TResult(A1, A2, A3, A4)>(&typeid(TResult (TOriginalClass::*)(A1, A2, A3, A4)), a1, a2, a3, a4); \
    }

#define DEFINE_MOCK_METHOD(...) \
    DEFINE_MACRO_CHOOSER(DEFINE_MOCK_METHOD, __VA_ARGS__)(__VA_ARGS__)

#define DEFINE_MOCK_METHOD_CONST_2(TResult, methodName) \
public: \
    TResult methodName() const override { \
        return this->call<TResult()>(&typeid(TResult (TOriginalClass::*)() const)); \
    }

#define DEFINE_MOCK_METHOD_CONST_3(TResult, methodName, A1) \
public: \
    TResult methodName(A1 a1) const override { \
        return this->call<TResult(A1)>(&typeid(TResult (TOriginalClass::*)(A1) const), a1); \
    }

#define DEFINE_MOCK_METHOD_CONST_4(TResult, methodName, A1, A2) \
public: \
    TResult methodName(A1 a1, A2 a2) const override { \
        return this->call<TResult(A1, A2)>(&typeid(TResult (TOriginalClass::*)(A1, A2) const), a1, a2); \
    }

#define DEFINE_MOCK_METHOD_CONST_5(TResult, methodName, A1, A2, A3) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3) const override { \
        return this->call<TResult(A1, A2, A3)>(&typeid(TResult (TOriginalClass::*)(A1, A2, A3) const), a1, a2, a3); \
    }

#define DEFINE_MOCK_METHOD_CONST_6(TResult, methodName, A1, A2, A3, A4) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3, A4 a4) const override { \
        return this->call<TResult(A1, A2, A3, A4)>(&typeid(TResult (TOriginalClass::*)(A1, A2, A3, A4) const), a1, a2, a3, a4); \
    }

#define DEFINE_MOCK_METHOD_CONST(...) \
    DEFINE_MACRO_CHOOSER(DEFINE_MOCK_METHOD_CONST, __VA_ARGS__)(__VA_ARGS__)

#define DEFINE_MOCK_CLASS(Interface) \
class Interface##Mock: public Interface, public ::iridium::testing::Mock<Interface>

#define DEFINE_MOCK_CONSTRUCTOR(Interface) \
public: \
template<typename ... TArgs> \
Interface##Mock(TArgs ... args): Interface(args ...) {};

#define DEFINE_MOCK_BEHAVIOR(result_type, method_name, mock_object, ...)                        \
decltype(mock_object)::Behavior<                                                                \
    decltype(static_cast<result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*) \
    (__VA_ARGS__)>(&std::remove_reference_t<decltype(mock_object)>::TOriginalClass::method_name)) \
>(                                                                                              \
    mock_object,                                                                                \
    &typeid(static_cast<result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*) \
    (__VA_ARGS__)>(&std::remove_reference_t<decltype(mock_object)>::TOriginalClass::method_name)) \
) = [&](__VA_ARGS__)

#define DEFINE_MOCK_BEHAVIOR_CONST(result_type, method_name, mock_object, ...)                  \
decltype(mock_object)::Behavior<                                                                \
    decltype(static_cast<result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*) \
    (__VA_ARGS__) const>(&std::remove_reference_t<decltype(mock_object)>::TOriginalClass::method_name)) \
>(                                                                                              \
    mock_object,                                                                                \
    &typeid(static_cast<result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*) \
    (__VA_ARGS__) const>(&std::remove_reference_t<decltype(mock_object)>::TOriginalClass::method_name)) \
) = [&](__VA_ARGS__)

#define DEFINE_MOCK_SEQUENCE(name) \
::iridium::testing::MockSequence sequence_##name(__FILE__, __LINE__, #name)


#define DEFINE_MOCK_SEQUENCE_EXPECTATION(sequence_name, mock, method) \
sequence_##sequence_name.addExpectation(mock, &method, #method, __FILE__, __LINE__)


#endif // HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2
