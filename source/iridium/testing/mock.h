#ifndef HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2
#define HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2


#include "iridium/macros/va_args.h"
#include "iridium/items.h"

#include <typeinfo>
#include <functional>
#include <any>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <memory>
#include <limits>
#include <cstdio>
#include <tuple>
#include <utility>
#include <type_traits>

#include "iridium/pattern/non_copyable.h"
#include "iridium/pattern/non_movable.h"


namespace iridium {
namespace testing {

// Forward declarations
template<typename TClassMock> class MockSequence;
template<typename TClassMock, typename MethodReturnType, typename... MethodArgs> class ExpectationBuilder;

namespace internal {
    struct ActiveExpectationInfo {
        enum class ActionType { NONE, RETURN, THROW, INVOKE };
        ActionType action_type = ActionType::NONE;
        std::any return_value;
        std::function<void()> throw_lambda;
        std::any invoke_lambda;
        bool is_action_will_once = true;
        int* action_invocation_count_ptr = nullptr;

        void clear() {
            action_type = ActionType::NONE;
            return_value.reset();
            throw_lambda = nullptr;
            invoke_lambda.reset();
            is_action_will_once = true;
            action_invocation_count_ptr = nullptr;
        }
    };
    inline ActiveExpectationInfo g_active_sequence_action_info;
}


template<typename TClass>
class Mock {
public:
    using TOriginalClass = TClass;
    Mock();
    virtual ~Mock() = default;

    template<typename ... TArgs>
    static ::std::shared_ptr<TClass> create(TArgs && ... args);

    template<typename TResult, typename ... TArgs> class Behavior;
    template<typename TResult, typename ... TArgs>
    class Behavior<TResult(TClass::*)(TArgs...)>: public pattern::NonCopyable, public pattern::NonMovable {
    public:
        explicit Behavior(Mock &mock, std::type_info const *method);
        template<typename TLambda> Behavior &operator=(TLambda &&l);
    private:
        Mock &m_mock;
        std::type_info const *m_method;
    };
    template<typename TResult, typename ... TArgs>
    class Behavior<TResult(TClass::*)(TArgs...) const>: public pattern::NonCopyable, public pattern::NonMovable {
    public:
        explicit Behavior(Mock &mock, std::type_info const *method);
        template<typename TLambda> Behavior &operator=(TLambda &&l);
    private:
        Mock &m_mock;
        std::type_info const *m_method;
    };

protected:
    template<typename TResult, typename ... TArgs>
    auto call(TArgs && ... args);
    template<typename TResult, typename ... TArgs>
    auto call(TArgs && ... args) const;

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


template<typename R>
struct ReturnAction {
    R value;
    explicit ReturnAction(R val) : value(std::move(val)) {}
};

template<>
struct ReturnAction<void> {
    explicit ReturnAction() = default;
};

template<typename R>
auto Return(R&& val) -> std::enable_if_t<!std::is_void_v<std::decay_t<R>>, ReturnAction<std::decay_t<R>>> {
    return ReturnAction<std::decay_t<R>>(std::forward<R>(val));
}

template<typename R>
auto Return(const R& val) -> std::enable_if_t<!std::is_void_v<std::decay_t<R>>, ReturnAction<std::decay_t<R>>> {
    return ReturnAction<std::decay_t<R>>(val);
}
// Optional: For explicit Return() for void methods, if desired for syntax consistency.
// inline ReturnAction<void> Return() { return ReturnAction<void>(); }


struct ThrowAction {
    std::function<void()> throw_lambda;
    template<typename E>
    explicit ThrowAction(E exception_instance)
        : throw_lambda([exc = std::move(exception_instance)]() { throw exc; }) {}
};

template<typename E>
ThrowAction Throw(E&& exception_instance) {
    return ThrowAction(std::forward<E>(exception_instance));
}

template<typename Lambda>
struct InvokeActionWrapper {
    Lambda lambda;
    explicit InvokeActionWrapper(Lambda l) : lambda(std::move(l)) {}
};

template<typename Lambda>
InvokeActionWrapper<Lambda> Invoke(Lambda&& l) {
    return InvokeActionWrapper<Lambda>(std::forward<Lambda>(l));
}


template<typename... Args>
struct ArgsAreMatcher {
    std::tuple<std::decay_t<Args>...> expected_args;
    explicit ArgsAreMatcher(Args&&... args) : expected_args(std::forward<Args>(args)...) {}
};

template<typename... Args>
ArgsAreMatcher<Args...> ArgsAre(Args&&... args) {
    return ArgsAreMatcher<Args...>(std::forward<Args>(args)...);
}

struct Times {
    int count;
    enum class Type { EXACTLY, AT_LEAST, AT_MOST, ANY, NEVER } type;
    explicit Times(int n) : count(n), type(Type::EXACTLY) {}
    static Times Exactly(int n) { return Times(n); }
    static Times AtLeast(int n) { Times t(n); t.type = Type::AT_LEAST; return t; }
    static Times AtMost(int n) { Times t(n); t.type = Type::AT_MOST; return t; }
    static Times AnyNumber() { Times t(0); t.type = Type::ANY; return t; }
    static Times Never() { Times t(0); t.type = Type::NEVER; return t; }
};

template<typename TClassMock, typename MethodReturnType, typename... MethodArgs>
class ExpectationBuilder {
    MockSequence<TClassMock>& m_sequence;
    typename MockSequence<TClassMock>::TExpectation& m_expectation;

public:
    ExpectationBuilder(MockSequence<TClassMock>& seq, typename MockSequence<TClassMock>::TExpectation& exp)
        : m_sequence(seq), m_expectation(exp) {}

    ExpectationBuilder& Times(int n) {
        if (n < 0) throw std::invalid_argument("Times() argument cannot be negative.");
        m_expectation.m_call_count_type = MockSequence<TClassMock>::TExpectation::CallCountType::EXACTLY;
        m_expectation.m_expected_min_calls = n;
        m_expectation.m_expected_max_calls = n;
        return *this;
    }

    ExpectationBuilder& Times(const struct Times& times_spec) {
        switch (times_spec.type) {
            case Times::Type::EXACTLY:
                if (times_spec.count < 0) throw std::invalid_argument("Times::Exactly count cannot be negative.");
                m_expectation.m_call_count_type = MockSequence<TClassMock>::TExpectation::CallCountType::EXACTLY;
                m_expectation.m_expected_min_calls = times_spec.count;
                m_expectation.m_expected_max_calls = times_spec.count;
                break;
            case Times::Type::AT_LEAST:
                if (times_spec.count < 0) throw std::invalid_argument("Times::AtLeast count cannot be negative.");
                if (times_spec.count == 0) {
                     m_expectation.m_call_count_type = MockSequence<TClassMock>::TExpectation::CallCountType::ANY_NUMBER;
                     m_expectation.m_expected_min_calls = 0;
                     m_expectation.m_expected_max_calls = std::numeric_limits<int>::max();
                } else {
                    m_expectation.m_call_count_type = MockSequence<TClassMock>::TExpectation::CallCountType::AT_LEAST;
                    m_expectation.m_expected_min_calls = times_spec.count;
                    m_expectation.m_expected_max_calls = std::numeric_limits<int>::max();
                }
                break;
            case Times::Type::AT_MOST:
                if (times_spec.count < 0) throw std::invalid_argument("Times::AtMost count cannot be negative.");
                m_expectation.m_call_count_type = MockSequence<TClassMock>::TExpectation::CallCountType::AT_MOST;
                m_expectation.m_expected_min_calls = 0;
                m_expectation.m_expected_max_calls = times_spec.count;
                break;
            case Times::Type::ANY:
                m_expectation.m_call_count_type = MockSequence<TClassMock>::TExpectation::CallCountType::ANY_NUMBER;
                m_expectation.m_expected_min_calls = 0;
                m_expectation.m_expected_max_calls = std::numeric_limits<int>::max();
                break;
            case Times::Type::NEVER:
                m_expectation.m_call_count_type = MockSequence<TClassMock>::TExpectation::CallCountType::NEVER;
                m_expectation.m_expected_min_calls = 0;
                m_expectation.m_expected_max_calls = 0;
                break;
        }
        return *this;
    }

    template<typename... MatcherArgs>
    ExpectationBuilder& With(ArgsAreMatcher<MatcherArgs...>&& arg_matcher) {
        static_assert(sizeof...(MatcherArgs) == sizeof...(MethodArgs), "ArgsAre: Number of arguments does not match method signature being expected.");
        m_expectation.m_expected_args_tuple = std::move(arg_matcher.expected_args);
        return *this;
    }

    template<typename ActualReturnType = MethodReturnType>
    auto WillOnce(ReturnAction<ActualReturnType>&& action) ->
        std::enable_if_t<!std::is_void_v<ActualReturnType> &&
                         std::is_same_v<ActualReturnType, MethodReturnType>, ExpectationBuilder&>
    {
        m_expectation.m_action_type = MockSequence<TClassMock>::TExpectation::ActionType::RETURN;
        m_expectation.m_return_value = std::move(action.value);
        m_expectation.m_is_action_will_once = true;
        return *this;
    }

    template<typename ActualReturnType = MethodReturnType>
    auto WillRepeatedly(ReturnAction<ActualReturnType>&& action) ->
        std::enable_if_t<!std::is_void_v<ActualReturnType> &&
                         std::is_same_v<ActualReturnType, MethodReturnType>, ExpectationBuilder&>
    {
        m_expectation.m_action_type = MockSequence<TClassMock>::TExpectation::ActionType::RETURN;
        m_expectation.m_return_value = std::move(action.value);
        m_expectation.m_is_action_will_once = false;
        return *this;
    }

    // Overloads for void methods (no ReturnAction value)
    // This allows .WillOnce() or .WillRepeatedly() without arguments for void methods.
    template<typename ActualReturnType = MethodReturnType>
    auto WillOnce() -> std::enable_if_t<std::is_void_v<ActualReturnType> &&
                                      std::is_same_v<ActualReturnType, MethodReturnType>, ExpectationBuilder&> {
        m_expectation.m_action_type = MockSequence<TClassMock>::TExpectation::ActionType::RETURN; // Still a RETURN action, but Mock::call handles void.
        m_expectation.m_return_value.reset(); // Ensure no value is stored for void return.
        m_expectation.m_is_action_will_once = true;
        return *this;
    }

    template<typename ActualReturnType = MethodReturnType>
    auto WillRepeatedly() -> std::enable_if_t<std::is_void_v<ActualReturnType> &&
                                            std::is_same_v<ActualReturnType, MethodReturnType>, ExpectationBuilder&> {
        m_expectation.m_action_type = MockSequence<TClassMock>::TExpectation::ActionType::RETURN;
        m_expectation.m_return_value.reset();
        m_expectation.m_is_action_will_once = false;
        return *this;
    }

    template<typename E>
    ExpectationBuilder& WillOnce(ThrowAction&& action) {
        m_expectation.m_action_type = MockSequence<TClassMock>::TExpectation::ActionType::THROW;
        m_expectation.m_throw_lambda = std::move(action.throw_lambda);
        m_expectation.m_is_action_will_once = true;
        return *this;
    }

    template<typename E>
    ExpectationBuilder& WillRepeatedly(ThrowAction&& action) {
        m_expectation.m_action_type = MockSequence<TClassMock>::TExpectation::ActionType::THROW;
        m_expectation.m_throw_lambda = std::move(action.throw_lambda);
        m_expectation.m_is_action_will_once = false;
        return *this;
    }

    template<typename Lambda>
    ExpectationBuilder& WillOnce(InvokeActionWrapper<Lambda>&& action_wrapper) {
        static_assert(std::is_invocable_r_v<MethodReturnType, Lambda, MethodArgs...>,
                      "Invoke: Lambda signature is not compatible with method signature (return type or arguments).");
        using ExpectedFuncType = std::function<MethodReturnType(MethodArgs...)>;
        m_expectation.m_action_type = MockSequence<TClassMock>::TExpectation::ActionType::INVOKE;
        m_expectation.m_invoke_lambda = ExpectedFuncType(std::move(action_wrapper.lambda));
        m_expectation.m_is_action_will_once = true;
        return *this;
    }

    template<typename Lambda>
    ExpectationBuilder& WillRepeatedly(InvokeActionWrapper<Lambda>&& action_wrapper) {
        static_assert(std::is_invocable_r_v<MethodReturnType, Lambda, MethodArgs...>,
                      "Invoke: Lambda signature is not compatible with method signature (return type or arguments).");
        using ExpectedFuncType = std::function<MethodReturnType(MethodArgs...)>;
        m_expectation.m_action_type = MockSequence<TClassMock>::TExpectation::ActionType::INVOKE;
        m_expectation.m_invoke_lambda = ExpectedFuncType(std::move(action_wrapper.lambda));
        m_expectation.m_is_action_will_once = false;
        return *this;
    }
};

template<typename TClassMock>
class MockSequence {
public:
    struct TExpectation {
        enum class CallCountType { EXACTLY, AT_LEAST, AT_MOST, ANY_NUMBER, NEVER, UNSET };
        enum class ActionType { NONE, RETURN, THROW, INVOKE };

        CallCountType m_call_count_type;
        int m_expected_min_calls;
        int m_expected_max_calls;
        int m_actual_calls;
        std::string             const file_line;
        const std::type_info*   m_method_type_info;
        std::any                m_expected_args_tuple;

        ActionType m_action_type;
        bool m_is_action_will_once;
        int m_action_invocation_count;
        std::any m_return_value;
        std::function<void()> m_throw_lambda;
        std::any m_invoke_lambda;


        TExpectation(
            std::string     const file_line,
            const std::type_info* method_type_info,
            std::any        const &initial_expected_args = {})
        :
            file_line       (file_line),
            m_method_type_info(method_type_info),
            m_expected_args_tuple(initial_expected_args),
            m_call_count_type(CallCountType::UNSET),
            m_expected_min_calls(1),
            m_expected_max_calls(1),
            m_actual_calls(0),
            m_action_type(ActionType::NONE),
            m_is_action_will_once(true),
            m_action_invocation_count(0)
        {
        }
    };

    MockSequence(TClassMock &mock, std::string const &file_line)
    :
        m_mock      (mock),
        m_file_line (file_line),
        m_call_count(0)
    {}

    ~MockSequence() {
        for (const auto& exp : m_expectations) {
            if (exp.m_call_count_type == TExpectation::CallCountType::EXACTLY ||
                exp.m_call_count_type == TExpectation::CallCountType::NEVER) {
                if (exp.m_actual_calls != exp.m_expected_min_calls) {
                    fprintf(stderr, "MockSequence Error at %s: Method %s expected %d calls, but received %d.\n",
                            exp.file_line.c_str(), exp.m_method_type_info->name(), exp.m_expected_min_calls, exp.m_actual_calls);
                }
            } else if (exp.m_call_count_type == TExpectation::CallCountType::AT_LEAST) {
                if (exp.m_actual_calls < exp.m_expected_min_calls) {
                    fprintf(stderr, "MockSequence Error at %s: Method %s expected at least %d calls, but received %d.\n",
                            exp.file_line.c_str(), exp.m_method_type_info->name(), exp.m_expected_min_calls, exp.m_actual_calls);
                }
            }
        }
    }

    template<typename R, typename... MArgs>
    auto ExpectObj(R (TClassMock::* /* method_ptr_for_type_deduction */ )(MArgs...)) -> ExpectationBuilder<TClassMock, R, MArgs...> {
        R (typename TClassMock::TOriginalClass::*temp_original_method_ptr)(MArgs...) = nullptr;
        const auto* method_signature_type_info = &typeid(decltype(temp_original_method_ptr));
        m_expectations.emplace_back(m_file_line, method_signature_type_info);
        return ExpectationBuilder<TClassMock, R, MArgs...>(*this, m_expectations.back());
    }

    template<typename R, typename... MArgs>
    auto ExpectObj(R (TClassMock::* /* method_ptr_for_type_deduction */)(MArgs...) const) -> ExpectationBuilder<TClassMock, R, MArgs...> {
        R (typename TClassMock::TOriginalClass::*temp_original_const_method_ptr)(MArgs...) const = nullptr;
        const auto* method_signature_type_info = &typeid(decltype(temp_original_const_method_ptr));
        m_expectations.emplace_back(m_file_line, method_signature_type_info);
        return ExpectationBuilder<TClassMock, R, MArgs...>(*this, m_expectations.back());
    }

    template<typename ... TCallArgs>
    bool isEqual(const std::any &stored_any, TCallArgs &&... args) {
        using TTuple = std::tuple<std::decay_t<TCallArgs>...>;
        if (stored_any.type() != typeid(TTuple)) { return false; }
        const TTuple &stored_tuple = std::any_cast<const TTuple&>(stored_any);
        TTuple current_tuple(std::forward<TCallArgs>(args)...);
        return stored_tuple == current_tuple;
    }

    template<typename TMethodTypePtr>
    void throwException(TMethodTypePtr /*method_ptr*/, std::string const &expectation_file_line, std::string const &error_msg) {
        throw std::runtime_error(std::string("sequence at '") + m_file_line +
            "' mock '" + typeid(typename TClassMock::TOriginalClass).name() +
            "' expectation error at '" + expectation_file_line + "': " + error_msg);
    }

    template<typename TResult, typename... TMethodArgs, typename... TCallArgs>
    void step(
        TResult (TClassMock::*method_ptr)(TMethodArgs...),
        TCallArgs && ... args)
    {
        if (m_expectations.empty()) {
             throw std::runtime_error(std::string("sequence at '") + m_file_line + "' mock '" +
                typeid(typename TClassMock::TOriginalClass).name() + "' error: step() called on empty expectation list for method call.");
        }

        TExpectation& current_exp = m_expectations.front();

        TResult (typename TClassMock::TOriginalClass::*temp_original_method_ptr)(TMethodArgs...) = nullptr;
        const auto* called_method_type_info = &typeid(decltype(temp_original_method_ptr));

        if (*(current_exp.m_method_type_info) != *called_method_type_info) {
             throwException(
                method_ptr, current_exp.file_line,
                "unexpected method call. Expected: " + std::string(current_exp.m_method_type_info->name()) +
                ", Got: " + std::string(called_method_type_info->name()));
        }

        if (current_exp.m_expected_args_tuple.has_value()) {
            if (!isEqual(current_exp.m_expected_args_tuple, args ...)) {
                throwException(
                    method_ptr, current_exp.file_line,
                    "arguments do not match for method " + std::string(current_exp.m_method_type_info->name()));
            }
        }

        current_exp.m_actual_calls++;

        internal::g_active_sequence_action_info.clear();
        if (current_exp.m_action_type == TExpectation::ActionType::RETURN) {
            bool is_action_still_valid_for_invocation = !current_exp.m_is_action_will_once || (current_exp.m_action_invocation_count == 0);
            if (is_action_still_valid_for_invocation) {
                internal::g_active_sequence_action_info.action_type = internal::ActiveExpectationInfo::ActionType::RETURN;
                internal::g_active_sequence_action_info.return_value = current_exp.m_return_value;
                internal::g_active_sequence_action_info.is_action_will_once = current_exp.m_is_action_will_once;
                internal::g_active_sequence_action_info.action_invocation_count_ptr = &current_exp.m_action_invocation_count;
            }
        } else if (current_exp.m_action_type == TExpectation::ActionType::THROW) {
             bool is_action_still_valid_for_invocation = !current_exp.m_is_action_will_once || (current_exp.m_action_invocation_count == 0);
            if (is_action_still_valid_for_invocation) {
                internal::g_active_sequence_action_info.action_type = internal::ActiveExpectationInfo::ActionType::THROW;
                internal::g_active_sequence_action_info.throw_lambda = current_exp.m_throw_lambda;
                internal::g_active_sequence_action_info.is_action_will_once = current_exp.m_is_action_will_once;
                internal::g_active_sequence_action_info.action_invocation_count_ptr = &current_exp.m_action_invocation_count;
            }
        } else if (current_exp.m_action_type == TExpectation::ActionType::INVOKE) {
            bool is_action_still_valid_for_invocation = !current_exp.m_is_action_will_once || (current_exp.m_action_invocation_count == 0);
            if (is_action_still_valid_for_invocation) {
                internal::g_active_sequence_action_info.action_type = internal::ActiveExpectationInfo::ActionType::INVOKE;
                internal::g_active_sequence_action_info.invoke_lambda = current_exp.m_invoke_lambda;
                internal::g_active_sequence_action_info.is_action_will_once = current_exp.m_is_action_will_once;
                internal::g_active_sequence_action_info.action_invocation_count_ptr = &current_exp.m_action_invocation_count;
            }
        }


        if ((current_exp.m_call_count_type == TExpectation::CallCountType::EXACTLY ||
             current_exp.m_call_count_type == TExpectation::CallCountType::NEVER ||
             current_exp.m_call_count_type == TExpectation::CallCountType::AT_MOST) &&
            current_exp.m_actual_calls > current_exp.m_expected_max_calls) {
            throw std::runtime_error("MockSequence error at " + current_exp.file_line +
                                     ": Method " + current_exp.m_method_type_info->name() + " called " +
                                     std::to_string(current_exp.m_actual_calls) +
                                     " times, but expected max " + std::to_string(current_exp.m_expected_max_calls) + " calls.");
        }

        bool pop_expectation = false;
        if (current_exp.m_call_count_type == TExpectation::CallCountType::UNSET ||
            current_exp.m_call_count_type == TExpectation::CallCountType::EXACTLY) {
            if (current_exp.m_actual_calls >= current_exp.m_expected_max_calls) {
                pop_expectation = true;
            }
        } else if (current_exp.m_call_count_type == TExpectation::CallCountType::NEVER) {
             if (current_exp.m_actual_calls > 0) {
                 pop_expectation = true;
             }
        }

        if (pop_expectation) {
             m_expectations.pop_front();
        }

        m_call_count = 0;
    }

    // Overload for const member functions
    template<typename TResult, typename... TMethodArgs, typename... TCallArgs>
    void step(
        TResult (TClassMock::*method_ptr)(TMethodArgs...) const,
        TCallArgs && ... args)
    {
        if (m_expectations.empty()) {
             throw std::runtime_error(std::string("sequence at '") + m_file_line + "' mock '" +
                typeid(typename TClassMock::TOriginalClass).name() + "' error: step() called on empty expectation list for const method call.");
        }

        TExpectation& current_exp = m_expectations.front();

        TResult (typename TClassMock::TOriginalClass::*temp_original_const_method_ptr)(TMethodArgs...) const = nullptr;
        const auto* called_method_type_info = &typeid(decltype(temp_original_const_method_ptr));


        if (*(current_exp.m_method_type_info) != *called_method_type_info) {
             throwException(
                method_ptr, current_exp.file_line,
                "unexpected const method call. Expected: " + std::string(current_exp.m_method_type_info->name()) +
                ", Got: " + std::string(called_method_type_info->name()));
        }

        if (current_exp.m_expected_args_tuple.has_value()) {
            if (!isEqual(current_exp.m_expected_args_tuple, args ...)) {
                throwException(
                    method_ptr, current_exp.file_line,
                    "arguments do not match for const method " + std::string(current_exp.m_method_type_info->name()));
            }
        }

        current_exp.m_actual_calls++;

        internal::g_active_sequence_action_info.clear();
        if (current_exp.m_action_type == TExpectation::ActionType::RETURN) {
            bool is_action_still_valid_for_invocation = !current_exp.m_is_action_will_once || (current_exp.m_action_invocation_count == 0);
            if (is_action_still_valid_for_invocation) {
                internal::g_active_sequence_action_info.action_type = internal::ActiveExpectationInfo::ActionType::RETURN;
                internal::g_active_sequence_action_info.return_value = current_exp.m_return_value;
                internal::g_active_sequence_action_info.is_action_will_once = current_exp.m_is_action_will_once;
                internal::g_active_sequence_action_info.action_invocation_count_ptr = &current_exp.m_action_invocation_count;
            }
        } else if (current_exp.m_action_type == TExpectation::ActionType::THROW) {
             bool is_action_still_valid_for_invocation = !current_exp.m_is_action_will_once || (current_exp.m_action_invocation_count == 0);
            if (is_action_still_valid_for_invocation) {
                internal::g_active_sequence_action_info.action_type = internal::ActiveExpectationInfo::ActionType::THROW;
                internal::g_active_sequence_action_info.throw_lambda = current_exp.m_throw_lambda;
                internal::g_active_sequence_action_info.is_action_will_once = current_exp.m_is_action_will_once;
                internal::g_active_sequence_action_info.action_invocation_count_ptr = &current_exp.m_action_invocation_count;
            }
        } else if (current_exp.m_action_type == TExpectation::ActionType::INVOKE) {
            bool is_action_still_valid_for_invocation = !current_exp.m_is_action_will_once || (current_exp.m_action_invocation_count == 0);
            if (is_action_still_valid_for_invocation) {
                internal::g_active_sequence_action_info.action_type = internal::ActiveExpectationInfo::ActionType::INVOKE;
                internal::g_active_sequence_action_info.invoke_lambda = current_exp.m_invoke_lambda;
                internal::g_active_sequence_action_info.is_action_will_once = current_exp.m_is_action_will_once;
                internal::g_active_sequence_action_info.action_invocation_count_ptr = &current_exp.m_action_invocation_count;
            }
        }

        if ((current_exp.m_call_count_type == TExpectation::CallCountType::EXACTLY ||
             current_exp.m_call_count_type == TExpectation::CallCountType::NEVER ||
             current_exp.m_call_count_type == TExpectation::CallCountType::AT_MOST) &&
            current_exp.m_actual_calls > current_exp.m_expected_max_calls) {
            throw std::runtime_error("MockSequence error at " + current_exp.file_line +
                                     ": Method " + current_exp.m_method_type_info->name() + " called " +
                                     std::to_string(current_exp.m_actual_calls) +
                                     " times, but expected max " + std::to_string(current_exp.m_expected_max_calls) + " calls.");
        }

        bool pop_expectation = false;
        if (current_exp.m_call_count_type == TExpectation::CallCountType::UNSET ||
            current_exp.m_call_count_type == TExpectation::CallCountType::EXACTLY) {
            if (current_exp.m_actual_calls >= current_exp.m_expected_max_calls) {
                pop_expectation = true;
            }
        } else if (current_exp.m_call_count_type == TExpectation::CallCountType::NEVER) {
             if (current_exp.m_actual_calls > 0) {
                 pop_expectation = true;
             }
        }

        if (pop_expectation) {
             m_expectations.pop_front();
        }

        m_call_count = 0;
    }


private:
    TClassMock             &m_mock;
    std::string             m_file_line;
    std::list<TExpectation> m_expectations;
    size_t                  m_call_count;
};


// Implementation of Mock<TClass> methods
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
            return std::shared_ptr<TClass>(dynamic_cast<TClass *>(mock), [] (TClass *) {});
        }
    } else {
        return ::std::make_shared<TClass>(std::forward<TArgs>(args)...);
    }
}

template<typename TClass>
template<typename TResult, typename ... TArgs>
auto Mock<TClass>::call(TArgs && ... args) {
    if (internal::g_active_sequence_action_info.action_type != internal::ActiveExpectationInfo::ActionType::NONE) {
        auto& current_action_info = internal::g_active_sequence_action_info;
        auto action_type_copy = current_action_info.action_type;

        if (action_type_copy == internal::ActiveExpectationInfo::ActionType::RETURN) {
            std::any return_val_copy = current_action_info.return_value;
            int* count_ptr_copy = current_action_info.action_invocation_count_ptr;
            bool is_once_copy = current_action_info.is_action_will_once;

            if (count_ptr_copy) { (*count_ptr_copy)++; }
            if(is_once_copy) { current_action_info.clear(); }

            if constexpr (!std::is_void_v<TResult>) {
                if (!return_val_copy.has_value() && !std::is_default_constructible_v<TResult>) { // Check if value was set for non-void
                     internal::g_active_sequence_action_info.clear(); // Ensure clear on error
                     throw std::runtime_error(std::string("Mock::call: Sequence RETURN action for non-void method '") + typeid(TResult (TOriginalClass::*)(TArgs ...)).name() + "' but no value was provided via Return(value).");
                }
                try { return std::any_cast<TResult>(return_val_copy); }
                catch (const std::bad_any_cast& e) {
                     internal::g_active_sequence_action_info.clear();
                     throw std::runtime_error(std::string("Mock::call: Sequence RETURN action: Bad any_cast. Expected to return ") + typeid(TResult).name() + ". what(): " + e.what());
                }
            } else { return; }
        } else if (action_type_copy == internal::ActiveExpectationInfo::ActionType::THROW) {
            std::function<void()> throw_lambda_copy = current_action_info.throw_lambda;
            int* count_ptr_copy = current_action_info.action_invocation_count_ptr;
            bool is_once_copy = current_action_info.is_action_will_once;

            if (count_ptr_copy) { (*count_ptr_copy)++; }
            if(is_once_copy) { current_action_info.clear(); }

            if (throw_lambda_copy) { throw_lambda_copy(); }
            else { throw std::runtime_error("Misconfigured THROW action in mock sequence (null lambda).");}
            if constexpr (!std::is_void_v<TResult>) { throw std::runtime_error("THROW action was executed, this line should be unreachable."); }
            else { return; }
        } else if (action_type_copy == internal::ActiveExpectationInfo::ActionType::INVOKE) {
            if (current_action_info.invoke_lambda.has_value()) {
                using ExpectedFuncType = std::function<TResult(TArgs...)>;
                std::any lambda_copy_any = current_action_info.invoke_lambda;
                int* count_ptr_copy = current_action_info.action_invocation_count_ptr;
                bool is_once_copy = current_action_info.is_action_will_once;

                if (count_ptr_copy) { (*(count_ptr_copy))++; }
                if(is_once_copy) { internal::g_active_sequence_action_info.clear(); }

                try {
                    ExpectedFuncType func_to_call = std::any_cast<ExpectedFuncType>(lambda_copy_any);
                    if constexpr (!std::is_void_v<TResult>) { return func_to_call(std::forward<TArgs>(args)...); }
                    else { func_to_call(std::forward<TArgs>(args)...); return; }
                } catch (const std::bad_any_cast& e) {
                    internal::g_active_sequence_action_info.clear();
                    throw std::runtime_error(std::string("Mock::call: Sequence INVOKE action: Bad any_cast for lambda. Expected std::function<") + typeid(TResult).name() + "(...)>. what(): " + e.what());
                }
            } else {
                internal::g_active_sequence_action_info.clear();
                throw std::runtime_error("Mock::call: Sequence INVOKE action specified but no lambda stored.");
            }
        }
    }

    auto method_type_info = &typeid(TResult (TOriginalClass::*)(TArgs ...));
    auto i = m_map_name_behavior.find(method_type_info);
    if (i == m_map_name_behavior.end())
        throw std::runtime_error("mock calling error: unexpected call or no behavior defined for '" + std::string(method_type_info->name()) + "' (no sequence action active or applicable)");
    try {
        using TMethod = std::function<TResult (TArgs ...)>;
        auto method_fn = std::any_cast<TMethod>(i->second);
        return method_fn(std::forward<TArgs>(args)...);
    } catch (const std::bad_any_cast&) {
        throw std::runtime_error("mock calling error: type mismatch for behavior of method '" + std::string(method_type_info->name()) + "'");
    }
}

template<typename TClass>
template<typename TResult, typename ... TArgs>
auto Mock<TClass>::call(TArgs && ... args) const {
     if (internal::g_active_sequence_action_info.action_type != internal::ActiveExpectationInfo::ActionType::NONE) {
        auto& current_action_info = internal::g_active_sequence_action_info;
        auto action_type_copy = current_action_info.action_type;

        if (action_type_copy == internal::ActiveExpectationInfo::ActionType::RETURN) {
            std::any return_val_copy = current_action_info.return_value;
            int* count_ptr_copy = current_action_info.action_invocation_count_ptr;
            bool is_once_copy = current_action_info.is_action_will_once;
            if (count_ptr_copy) { (*count_ptr_copy)++; }
            if(is_once_copy) { current_action_info.clear(); }
            if constexpr (!std::is_void_v<TResult>) {
                 if (!return_val_copy.has_value() && !std::is_default_constructible_v<TResult>) {
                     internal::g_active_sequence_action_info.clear();
                     throw std::runtime_error(std::string("Mock::call const: Sequence RETURN action for non-void method '") + typeid(TResult (TOriginalClass::*)(TArgs ...) const).name() + "' but no value was provided via Return(value).");
                 }
                try { return std::any_cast<TResult>(return_val_copy); }
                catch (const std::bad_any_cast& e) { internal::g_active_sequence_action_info.clear(); throw std::runtime_error(std::string("Mock::call const: Sequence RETURN action: Bad any_cast. Expected ") + typeid(TResult).name() + ". what(): " + e.what()); }
            } else { return; }
        } else if (action_type_copy == internal::ActiveExpectationInfo::ActionType::THROW) {
            std::function<void()> throw_lambda_copy = current_action_info.throw_lambda;
            int* count_ptr_copy = current_action_info.action_invocation_count_ptr;
            bool is_once_copy = current_action_info.is_action_will_once;
            if (count_ptr_copy) { (*count_ptr_copy)++; }
            if(is_once_copy) { current_action_info.clear(); }
            if (throw_lambda_copy) { throw_lambda_copy(); }
            else { throw std::runtime_error("Misconfigured THROW action in mock sequence (null lambda)."); }
            if constexpr (!std::is_void_v<TResult>) { throw std::runtime_error("THROW action was executed, this line should be unreachable."); }
            else { return; }
        } else if (action_type_copy == internal::ActiveExpectationInfo::ActionType::INVOKE) {
             if (current_action_info.invoke_lambda.has_value()) {
                using ExpectedFuncType = std::function<TResult(TArgs...)>;
                std::any lambda_copy_any = current_action_info.invoke_lambda;
                int* count_ptr_copy = current_action_info.action_invocation_count_ptr;
                bool is_once_copy = current_action_info.is_action_will_once;
                if (count_ptr_copy) { (*(count_ptr_copy))++; }
                if(is_once_copy) { internal::g_active_sequence_action_info.clear(); }
                try {
                    ExpectedFuncType func_to_call = std::any_cast<ExpectedFuncType>(lambda_copy_any);
                    if constexpr (!std::is_void_v<TResult>) { return func_to_call(std::forward<TArgs>(args)...); }
                    else { func_to_call(std::forward<TArgs>(args)...); return; }
                } catch (const std::bad_any_cast& e) {
                    internal::g_active_sequence_action_info.clear();
                    throw std::runtime_error(std::string("Mock::call const: Sequence INVOKE action: Bad any_cast for lambda. Expected std::function<") + typeid(TResult).name() + "(...)>. what(): " + e.what());
                }
            } else {
                internal::g_active_sequence_action_info.clear();
                throw std::runtime_error("Mock::call const: Sequence INVOKE action specified but no lambda stored.");
            }
        }
    }

    auto method_type_info = &typeid(TResult (TOriginalClass::*)(TArgs ...) const);
    auto i = m_map_name_behavior.find(method_type_info);
    if (i == m_map_name_behavior.end())
        throw std::runtime_error("mock calling error: unexpected call or no behavior defined for const method '" + std::string(method_type_info->name()) + "' (no sequence action active or applicable)");
    try {
        using TMethod = std::function<TResult (TArgs ...)>;
        const auto& method_fn = std::any_cast<const TMethod&>(i->second);
        return method_fn(std::forward<TArgs>(args)...);
    } catch (const std::bad_any_cast&) {
        throw std::runtime_error("mock calling error: type mismatch for behavior of const method '" + std::string(method_type_info->name()) + "'");
    }
}

template<typename TClass>
template<typename TResult, typename ... TArgs>
void Mock<TClass>::setBehavior(std::type_info const *method, std::function<TResult(TArgs...)> const &&f) {
    m_map_name_behavior[method] = f;
}

template<typename TClass>
template<typename TResult, typename ... TArgs>
void Mock<TClass>::setBehavior(std::type_info const *method, std::function<TResult(TArgs...) const> const &&f) {
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
{}

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
        return this->call<TResult>(); \
    }

#define DEFINE_MOCK_METHOD_3(TResult, methodName, A1) \
public: \
    TResult methodName(A1 a1) override { \
        return this->call<TResult>(a1); \
    }

#define DEFINE_MOCK_METHOD_4(TResult, methodName, A1, A2) \
public: \
    TResult methodName(A1 a1, A2 a2) override { \
        return this->call<TResult>(a1, a2); \
    }

#define DEFINE_MOCK_METHOD_5(TResult, methodName, A1, A2, A3) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3) override { \
        return this->call<TResult>(a1, a2, a3); \
    }

#define DEFINE_MOCK_METHOD_6(TResult, methodName, A1, A2, A3, A4) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3, A4 a4) override { \
        return this->call<TResult>(a1, a2, a3, a4); \
    }

#define DEFINE_MOCK_METHOD(...) \
    DEFINE_MACRO_CHOOSER(DEFINE_MOCK_METHOD, __VA_ARGS__)(__VA_ARGS__)

#define DEFINE_MOCK_METHOD_CONST_2(TResult, methodName) \
public: \
    TResult methodName() const override { \
        return this->call<TResult>(); \
    }

#define DEFINE_MOCK_METHOD_CONST_3(TResult, methodName, A1) \
public: \
    TResult methodName(A1 a1) const override { \
        return this->call<TResult>(a1); \
    }

#define DEFINE_MOCK_METHOD_CONST_4(TResult, methodName, A1, A2) \
public: \
    TResult methodName(A1 a1, A2 a2) const override { \
        return this->call<TResult>(a1, a2); \
    }

#define DEFINE_MOCK_METHOD_CONST_5(TResult, methodName, A1, A2, A3) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3) const override { \
        return this->call<TResult>(a1, a2, a3); \
    }

#define DEFINE_MOCK_METHOD_CONST_6(TResult, methodName, A1, A2, A3, A4) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3, A4 a4) const override { \
        return this->call<TResult>(a1, a2, a3, a4); \
    }

#define DEFINE_MOCK_METHOD_CONST(...) \
    DEFINE_MACRO_CHOOSER(DEFINE_MOCK_METHOD_CONST, __VA_ARGS__)(__VA_ARGS__)

#define DEFINE_MOCK_CLASS(Interface) \
class Interface##Mock: public Interface, public ::iridium::testing::Mock<Interface>

#define DEFINE_MOCK_CONSTRUCTOR(Interface) \
public: \
template<typename ... TArgs> \
Interface##Mock(TArgs && ... args): Interface(std::forward<TArgs>(args) ...) {};

#define DEFINE_MOCK_BEHAVIOR(result_type, method_name, mock_object, ...)                         \
::iridium::testing::Mock<std::remove_reference_t<decltype(mock_object)>::TOriginalClass>::Behavior<                  \
    decltype(static_cast<result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*) \
    (__VA_ARGS__)>(&std::remove_reference_t<decltype(mock_object)>::method_name))>               \
(                                                                                                \
    mock_object,                                                                                 \
    &typeid(static_cast<result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*) \
    (__VA_ARGS__)>(&std::remove_reference_t<decltype(mock_object)>::method_name))                \
) = [&](__VA_ARGS__)

#define DEFINE_MOCK_BEHAVIOR_CONST(result_type, method_name, mock_object, ...)                   \
::iridium::testing::Mock<std::remove_reference_t<decltype(mock_object)>::TOriginalClass>::Behavior<                  \
    decltype(static_cast<result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*) \
    (__VA_ARGS__) const>(&std::remove_reference_t<decltype(mock_object)>::method_name))>         \
(                                                                                                \
    mock_object,                                                                                 \
    &typeid(static_cast<result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*) \
    (__VA_ARGS__) const>(&std::remove_reference_t<decltype(mock_object)>::method_name))          \
) = [&](__VA_ARGS__)

#define DEFINE_MOCK_SEQUENCE(name) \
::iridium::testing::MockSequence sequence_##name(__FILE__, __LINE__, #name)

// This macro is now DEPRECATED in favor of the fluent ExpectObj API.
// It will not compile correctly without the old addExpectation signatures.
// #define DEFINE_MOCK_SEQUENCE_EXPECTATION(sequence_name, mock, method) \
// sequence_##sequence_name.addExpectation(mock, &method, #method, __FILE__, __LINE__)


#endif // HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2
