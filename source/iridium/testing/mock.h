#ifndef HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2
#define HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2


#include "mock_registry.h"


//#include "iridium/smart_ptr.h"
#include "iridium/macros/va_args.h"
#include "iridium/items.h"
#include "iridium/logging/logger.h"

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


//class MockBase {
//public:
//
//}


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


//class IMock {
//public:
//    DEFINE_INTERFACE(IMock)
//    virtual std::string getName() const = 0;
//};


//template<typename TClass>
//class MockFactory {
//public:
//    template<typename ... TArgs>
//    static std::shared_ptr<TClass> create(TArgs&& ... args) {
//        return std::make_shared<TClass>(std::forward<TArgs>(args)...);
//    }
//};


//class Mock {
//public:
//    friend class Behavior;
//    template<typename TSignature>
//    class Behavior {
//    public:
//        Behavior(Mock &mock, std::string const &name_method_arguments);
//        template<typename TLambda>
//        Behavior &operator = (TLambda const &&l) {}
//    };
//
//protected:
//    template<typename TSignature, typename ... TArgs>
//    auto call(std::string const &name, TArgs && ... args) const {}
//
//private:
//    template<typename TSignature>
//    void setBehavior(std::string const &name, std::function<TSignature> const &&f) {}
//};


template<typename TClass>
class Mock {
public:
    Mock() {
        MockRegistry<TClass>::add(dynamic_cast<TClass * const>(this));
    }
    virtual ~Mock() = default;

    friend class Behavior;
    template<typename TSignature>
    class Behavior {
    public:
        Behavior(Mock &mock, std::type_info const *method);
        template<typename TLambda>
        Behavior &operator = (TLambda const &&l);

    private:
        Mock                   &m_mock;
        std::type_info const   *m_method;
    };

public:
    template<typename TSignature, typename ... TArgs>
    auto call(std::type_info const *method, TArgs && ... args) const;

    using TClassInternal = TClass;

private:
    template<typename TSignature>
    void setBehavior(std::type_info const *method, std::function<TSignature> const &&f);

    mutable std::unordered_map<std::type_info const *, std::any>   m_map_name_behavior;
    mutable std::unordered_map<void *, std::type_info const *>     m_map_behavior_name;
//    mutable std::vector<class MockSequence *> m_sequences;
};


// implementation


template<typename TClass>
template<typename TSignature, typename ... TArgs>
auto Mock<TClass>::call(std::type_info const *method, TArgs && ... args) const {
//    LOGT << "call: " << method->name();
    auto i = m_map_name_behavior.find(method);
    if (i == m_map_name_behavior.end()) {
        throw std::runtime_error("mock calling error: unexpected call '" + std::string(method->name()) + "'");
    }

    using TMethod = std::function<TSignature>;

    try {
        const auto& method = std::any_cast<const TMethod&>(i->second);
        return method(std::forward<TArgs>(args)...);
    } catch (const std::bad_any_cast&) {
        throw std::runtime_error("mock calling error: type mismatch for method '" + std::string(method->name()) + "'");
    }
}


template<typename TClass>
template<typename TSignature>
void Mock<TClass>::setBehavior(std::type_info const *method, std::function<TSignature> const &&f) {
    m_map_name_behavior[method] = f;
}


template<typename TClass>
template<typename TSignature>
Mock<TClass>::Behavior<TSignature>::Behavior(Mock &mock, std::type_info const *method)
:
    m_mock  (mock),
    m_method(method)
{
//    LOGT << "add behavior: " << method->name();
}


template<typename TClass>
template<typename TSignature>
template<typename TLambda>
typename Mock<TClass>::template Behavior<TSignature> &Mock<TClass>::Behavior<TSignature>::operator = (TLambda const &&l) {
    m_mock.setBehavior<TSignature>(m_method, l);
    return *this;
}


} // testing
} // iridium


#define DEFINE_MOCK_METHOD_2(TResult, methodName) \
public: \
    TResult methodName() { \
        static_assert(std::is_same< \
            decltype(static_cast<TResult (TClassInternal::*)()>(&TClassInternal::methodName)), \
            TResult (TClassInternal::*)() \
        >::value, "Method does not override base class method!"); \
        return this->call<TResult()>(&typeid(TResult (TClassInternal::*)())); \
    }

#define DEFINE_MOCK_METHOD_3(TResult, methodName, A1) \
public: \
    TResult methodName(A1 a1) { \
        static_assert(std::is_same< \
            decltype(static_cast<TResult (TClassInternal::*)(A1)>(&TClassInternal::methodName)), \
            TResult (TClassInternal::*)(A1) \
        >::value, "Method does not override base class method!"); \
        return this->call<TResult(A1)>(&typeid(TResult (TClassInternal::*)(A1)), a1); \
    }

#define DEFINE_MOCK_METHOD_4(TResult, methodName, A1, A2) \
public: \
    TResult methodName(A1 a1, A2 a2) { \
        static_assert(std::is_same< \
            decltype(static_cast<TResult (TClassInternal::*)(A1, A2)>(&TClassInternal::methodName)), \
            TResult (TClassInternal::*)(A1, A2) \
        >::value, "Method does not override base class method!"); \
        return this->call<TResult(A1, A2)>(&typeid(TResult (TClassInternal::*)(A1, A2)), a1, a2); \
    }

#define DEFINE_MOCK_METHOD_5(TResult, methodName, A1, A2, A3) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3) { \
        static_assert(std::is_same< \
            decltype(static_cast<TResult (TClassInternal::*)(A1, A2, A3)>(&TClassInternal::methodName)), \
            TResult (TClassInternal::*)(A1, A2, A3) \
        >::value, "Method does not override base class method!"); \
        return this->call<TResult(A1, A2, A3)>(&typeid(TResult (TClassInternal::*)(A1, A2, A3)), a1, a2, a3); \
    }

#define DEFINE_MOCK_METHOD_6(TResult, methodName, A1, A2, A3, A4) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3, A4 a4) { \
        static_assert(std::is_same< \
            decltype(static_cast<TResult (TClassInternal::*)(A1, A2, A3, A4)>(&TClassInternal::methodName)), \
            TResult (TClassInternal::*)(A1, A2, A3, A4) \
        >::value, "Method does not override base class method!"); \
        return this->call<TResult(A1, A2, A3, A4)>(&typeid(TResult (TClassInternal::*)(A1, A2, A3, A4)), a1, a2, a3, a4); \
    }

#define DEFINE_MOCK_METHOD(...) \
    DEFINE_MACRO_CHOOSER(DEFINE_MOCK_METHOD, __VA_ARGS__)(__VA_ARGS__)

#define DEFINE_MOCK_METHOD_CONST_2(TResult, methodName) \
public: \
    TResult methodName() const { \
        static_assert(std::is_same< \
            decltype(static_cast<TResult (TClassInternal::*)() const>(&TClassInternal::methodName)), \
            TResult (TClassInternal::*)() const \
        >::value, "Method does not override base class method!"); \
        return this->call<TResult()>(&typeid(TResult (TClassInternal::*)())); \
    }

#define DEFINE_MOCK_METHOD_CONST_3(TResult, methodName, A1) \
public: \
    TResult methodName(A1 a1) const { \
        static_assert(std::is_same< \
            decltype(static_cast<TResult (TClassInternal::*)(A1) const>(&TClassInternal::methodName)), \
            TResult (TClassInternal::*)(A1) const \
        >::value, "Method does not override base class method!"); \
        return this->call<TResult(A1)>(&typeid(TResult (TClassInternal::*)(A1)), a1); \
    }

#define DEFINE_MOCK_METHOD_CONST_4(TResult, methodName, A1, A2) \
public: \
    TResult methodName(A1 a1, A2 a2) const override { \
        static_assert(std::is_same< \
            decltype(static_cast<TResult (TClassInternal::*)(A1, A2) const>(&TClassInternal::methodName)), \
            TResult (TClassInternal::*)(A1, A2) const \
        >::value, "Method does not override base class method!"); \
        return this->call<TResult(A1, A2)>(&typeid(TResult (TClassInternal::*)(A1, A2)), a1, a2); \
    }

#define DEFINE_MOCK_METHOD_CONST_5(TResult, methodName, A1, A2, A3) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3) const override { \
        static_assert(std::is_same< \
            decltype(static_cast<TResult (TClassInternal::*)(A1, A2, A3) const>(&TClassInternal::methodName)), \
            TResult (TClassInternal::*)(A1, A2, A3) const \
        >::value, "Method does not override base class method!"); \
        return this->call<TResult(A1, A2, A3)>(&typeid(TResult (TClassInternal::*)(A1, A2, A3)), a1, a2, a3); \
    }

#define DEFINE_MOCK_METHOD_CONST_6(TResult, methodName, A1, A2, A3, A4) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3, A4 a4) const override { \
        static_assert(std::is_same< \
            decltype(static_cast<TResult (TClassInternal::*)(A1, A2, A3, A4) const>(&TClassInternal::methodName)), \
            TResult (TClassInternal::*)(A1, A2, A3, A4) const \
        >::value, "Method does not override base class method!"); \
        return this->call<TResult(A1, A2, A3, A4)>(&typeid(TResult (TClassInternal::*)(A1, A2, A3, A4)), a1, a2, a3, a4); \
    }

#define DEFINE_MOCK_METHOD_CONST(...) \
    DEFINE_MACRO_CHOOSER(DEFINE_MOCK_METHOD_CONST, __VA_ARGS__)(__VA_ARGS__)

//#define DEFINE_MOCK_CONSTRUCTOR(Interface) \
//public: \
//template<typename ... TArgs> \
//Interface##Mock(TArgs ... args): Interface(args ...) {};

#define DEFINE_MOCK_CLASS_BEGIN(Interface) \
class Interface##Mock: public Interface, public ::iridium::testing::Mock<Interface> { \
public: \
template<typename ... TArgs> \
Interface##Mock(TArgs ... args): Interface(args ...) {};

#define DEFINE_MOCK_CLASS_END(Interface) \
}; \
template<> \
class ::iridium::testing::MockRegistry<Interface##Mock>: \
    public MockRegistryBase<Interface> { \
public: \
    template<typename ... TArgs> \
    static auto take(TArgs&& ...) { \
        return MockRegistryBase<Interface>::take<Interface##Mock>(); \
    } \
};

#define STRINGIFY(...) #__VA_ARGS__
#define DEFINE_MOCK_BEHAVIOR(TResult, mock_method, mock_object, ...) \
    decltype(mock_object)::Behavior<TResult(__VA_ARGS__)>( \
        mock_object, &typeid(TResult (decltype(mock_object)::TClassInternal::*)(__VA_ARGS__)) \
    ) = [&](__VA_ARGS__)

#define DEFINE_MOCK_SEQUENCE(name) \
::iridium::testing::MockSequence sequence_##name(__FILE__, __LINE__, #name)

#define DEFINE_MOCK_SEQUENCE_EXPECTATION(sequence_name, mock, method) \
sequence_##sequence_name.addExpectation(mock, &method, #method, __FILE__, __LINE__)


#endif // HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2
