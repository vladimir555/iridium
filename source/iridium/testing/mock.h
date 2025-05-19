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


//class MockSequence {
//public:
//    MockSequence(const char *file, const char *line, std::string const &name)
//    :
//        m_origin(std::string(file) + ":" + line),
//        m_name  (name)
//    {}
//
//    template<typename TMock, typename TClass, typename TResult, typename... TArgs>
//    void step(TMock const * const mock, TResult(TClass::*method)(TArgs...)) {
//        if (m_expectation_index >= m_expectations.size()) {
//            throw std::runtime_error(
//                "mock sequence '" + m_name + "' at " + m_origin +
//                " error: too many calls, unexpected call to '" + mock->getName(method) + "'"
//            );
//        }
//
//        auto const &expectation = m_expectations[m_expectation_index];
//
//        if (expectation.mock != mock || expectation.method_ptr != getMethodPointer(method)) {
//            throw std::runtime_error(
//                "mock sequence '" + m_name + "' at " + m_origin +
//                " error: expected call to '" + expectation.method_name +
//                "' at " + expectation.file_line +
//                ", but got call to '" + mock->getName(method) + "'"
//            );
//        }
//
//        ++m_expectation_index;
//    }
//
//    template<typename TMock, typename TClass, typename TResult, typename... TArgs>
//    void addExpectation(TMock* mock, TResult(TClass::*method)(TArgs...), const char* method_name, const char* file, const char* line) {
//        m_expectations.push_back(
//            Expectation{
//                reinterpret_cast<void*>(mock),
//                getMethodPointer(method),
//                method_name,
//                std::string(file) + line
//            }
//        );
//    }
//
//private:
//    size_t m_expectation_index = 0;
//    std::string m_origin;
//    std::string m_name;
//
//    struct Expectation {
//        void* mock;
//        void* method_ptr;
//        std::string method_name;
//        std::string file_line;
//    };
//
//    std::vector<Expectation> m_expectations;
//
//    // Приводим любой метод к типу void*
//    template<typename TClass, typename TResult, typename... TArgs>
//    void* getMethodPointer(TResult(TClass::*method)(TArgs...)) const {
//        union {
//            TResult(TClass::*m)(TArgs...);
//            void* ptr;
//        } u = { method };
//        return u.ptr;
//    }
//};


template<typename TClass>
class Mock {
public:
    Mock();
    virtual ~Mock() = default;

    template<typename ... TArgs>
    static ::std::shared_ptr<TClass> create(TArgs && ... args);

    using TOriginalClass = TClass;

    friend class Behavior;

    template<typename TResult, typename ... TArgs>
    class Behavior;

    template<typename TResult, typename ... TArgs>
    class Behavior<TResult(TOriginalClass::*)(TArgs...)> {
    public:
        Behavior(Mock &mock, std::type_info const *method);

        template<typename TLambda>
        Behavior &operator=(TLambda &&l);

    private:
        Mock &m_mock;
        std::type_info const *m_method;
    };

public:
    template<typename TSignature, typename ... TArgs>
    auto call(std::type_info const *method, TArgs && ... args) const;

private:
    template<typename TResult, typename ... TArgs>
    void setBehavior(std::type_info const *method, std::function<TResult(TArgs...)> const &&f);

    mutable std::unordered_map<std::type_info const *, std::any>   m_map_name_behavior;
    mutable std::unordered_map<void *, std::type_info const *>     m_map_behavior_name;

    static std::list<Mock<TClass> *>    m_mock_objects;
    static bool                         m_is_mocked;
};


template<typename TClass>
std::list<Mock<TClass> *> Mock<TClass>::m_mock_objects;


template<typename TClass>
bool Mock<TClass>::m_is_mocked = false;


// implementation


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
    m_map_name_behavior[method] = f;
}


template<typename TClass>
template<typename TResult, typename ... TArgs>
Mock<TClass>::Behavior<TResult(TClass::*)(TArgs...)>::Behavior(Mock &mock, std::type_info const *method)
:
    m_mock  (mock),
    m_method(method)
{}


template<typename TClass>
template<typename TResult, typename ... TArgs>
template<typename TLambda>
typename Mock<TClass>::template Behavior<TResult(TClass::*)(TArgs...)> &Mock<TClass>::Behavior<TResult(TClass::*)(TArgs...)>::operator = (TLambda &&l) {
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
        return this->call<TResult()>(&typeid(TResult (TOriginalClass::*)())); \
    }

#define DEFINE_MOCK_METHOD_CONST_3(TResult, methodName, A1) \
public: \
    TResult methodName(A1 a1) const override { \
        return this->call<TResult(A1)>(&typeid(TResult (TOriginalClass::*)(A1)), a1); \
    }

#define DEFINE_MOCK_METHOD_CONST_4(TResult, methodName, A1, A2) \
public: \
    TResult methodName(A1 a1, A2 a2) const override { \
        return this->call<TResult(A1, A2)>(&typeid(TResult (TOriginalClass::*)(A1, A2)), a1, a2); \
    }

#define DEFINE_MOCK_METHOD_CONST_5(TResult, methodName, A1, A2, A3) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3) const override { \
        return this->call<TResult(A1, A2, A3)>(&typeid(TResult (TOriginalClass::*)(A1, A2, A3)), a1, a2, a3); \
    }

#define DEFINE_MOCK_METHOD_CONST_6(TResult, methodName, A1, A2, A3, A4) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3, A4 a4) const override { \
        return this->call<TResult(A1, A2, A3, A4)>(&typeid(TResult (TOriginalClass::*)(A1, A2, A3, A4)), a1, a2, a3, a4); \
    }

#define DEFINE_MOCK_METHOD_CONST(...) \
    DEFINE_MACRO_CHOOSER(DEFINE_MOCK_METHOD_CONST, __VA_ARGS__)(__VA_ARGS__)

#define DEFINE_MOCK_CLASS(Interface) \
class Interface##Mock: public Interface, public ::iridium::testing::Mock<Interface>

#define DEFINE_MOCK_CONSTRUCTOR(Interface) \
public: \
template<typename ... TArgs> \
Interface##Mock(TArgs ... args): Interface(args ...) {};

#define DEFINE_MOCK_BEHAVIOR(result_type, method_name, mock_object, ...) \
decltype(mock_object)::Behavior< decltype(static_cast< \
        result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*)(__VA_ARGS__) \
        >(&std::remove_reference_t<decltype(mock_object)>::TOriginalClass::method_name))> ( \
        mock_object, &typeid(decltype(static_cast< \
        result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*)(__VA_ARGS__) \
        >(&std::remove_reference_t<decltype(mock_object)>::TOriginalClass::method_name))) \
    ) = [&](__VA_ARGS__)

#define DEFINE_MOCK_SEQUENCE(name) \
::iridium::testing::MockSequence sequence_##name(__FILE__, __LINE__, #name)

#define DEFINE_MOCK_SEQUENCE_EXPECTATION(sequence_name, mock, method) \
sequence_##sequence_name.addExpectation(mock, &method, #method, __FILE__, __LINE__)


#endif // HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2
