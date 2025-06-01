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
#include <memory>

#include "iridium/pattern/non_copyable.h"
#include "iridium/pattern/non_movable.h"


/// \~english @file
/// @brief Provides a C++ mocking framework for unit testing.
///     This framework allows creating mock objects that simulate the behavior of real objects,
///     enabling isolated testing of components. It supports defining expectations for method calls,
///     specifying return values or actions, and verifying call sequences.
///     The primary user-facing macros for defining mock behavior and expectations are typically
///     `DEFINE_MOCK_CLASS`, `DEFINE_MOCK_METHOD` (and its const versions), `DEFINE_MOCK_BEHAVIOR`,
///     and sequence-related macros like `DEFINE_MOCK_SEQUENCE` and `DEFINE_MOCK_SEQUENCE_EXPECTATION`.
///     The syntax mentioned in the original file comments (e.g., `ON_CALL`, `EXPECT_CALL`)
///     would be built upon these foundational macros.
/// \~russian @file
/// @brief Предоставляет C++ фреймворк для создания мок-объектов при модульном тестировании.
///     Этот фреймворк позволяет создавать мок-объекты, имитирующие поведение реальных объектов,
///     что обеспечивает изолированное тестирование компонентов. Он поддерживает определение ожиданий
///     для вызовов методов, указание возвращаемых значений или действий, а также проверку последовательностей вызовов.
///     Основные макросы, видимые пользователю, для определения поведения моков и ожиданий, это обычно
///     `DEFINE_MOCK_CLASS`, `DEFINE_MOCK_METHOD` (и его const-версии), `DEFINE_MOCK_BEHAVIOR`,
///     а также макросы, связанные с последовательностями, такие как `DEFINE_MOCK_SEQUENCE` и `DEFINE_MOCK_SEQUENCE_EXPECTATION`.
///     Синтаксис, упомянутый в оригинальных комментариях файла (например, `ON_CALL`, `EXPECT_CALL`),
///     строился бы на основе этих фундаментальных макросов.

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
#include <memory>

#include "iridium/pattern/non_copyable.h"
#include "iridium/pattern/non_movable.h"


namespace iridium {
/// \~english @brief Provides classes, interfaces, and macros for unit testing and mocking.
/// \~russian @brief Предоставляет классы, интерфейсы и макросы для модульного тестирования и создания мок-объектов.
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

/// \~english @brief Forward declaration for the `MockSequence` class template.
/// \~russian @brief Предварительное объявление для шаблонного класса `MockSequence`.
template<typename TClassMock>
class MockSequence;

/// \~english @brief Base class for creating mock objects.
///     To create a mock for an interface or class `TClass`, inherit from `Mock<TClass>`.
///     This class provides the machinery for intercepting method calls and defining mock behaviors.
/// \~russian @brief Базовый класс для создания мок-объектов.
///     Для создания мока для интерфейса или класса `TClass`, наследуйте от `Mock<TClass>`.
///     Этот класс предоставляет механизм для перехвата вызовов методов и определения поведения моков.
/// \~english @tparam TClass The interface or class to be mocked.
/// \~russian @tparam TClass Интерфейс или класс, для которого создается мок.
template<typename TClass>
class Mock {
public:
    /// \~english @brief Type alias for the original class being mocked.
    /// \~russian @brief Псевдоним типа для исходного класса, который мокируется.
    using TOriginalClass = TClass;

    /// \~english @brief Default constructor. Registers the mock object instance.
    /// \~russian @brief Конструктор по умолчанию. Регистрирует экземпляр мок-объекта.
    Mock();
    /// \~english @brief Virtual default destructor.
    /// \~russian @brief Виртуальный деструктор по умолчанию.
    virtual ~Mock() = default;

    /// \~english @brief Static factory method to create instances.
    ///     If mocking is active (`m_is_mocked` is true), it attempts to return a pre-registered mock object.
    ///     Otherwise, it creates a new instance of `TClass` using `std::make_shared`.
    /// \~russian @brief Статический фабричный метод для создания экземпляров.
    ///     Если мокирование активно (`m_is_mocked` истинно), пытается вернуть предварительно зарегистрированный мок-объект.
    ///     В противном случае создает новый экземпляр `TClass` с помощью `std::make_shared`.
    /// \~english @tparam TArgs Variadic template arguments for the constructor of `TClass` when creating a real object.
    /// \~russian @tparam TArgs Вариативные шаблонные аргументы для конструктора `TClass` при создании реального объекта.
    /// \~english @param args Arguments to forward to the constructor of `TClass`.
    /// \~russian @param args Аргументы для передачи в конструктор `TClass`.
    /// \~english @return A shared pointer to `TClass` (either a mock or a real instance).
    /// \~russian @return Разделяемый указатель на `TClass` (либо мок, либо реальный экземпляр).
    /// \~english @exception std::runtime_error if `m_is_mocked` is true but no mock objects are registered.
    /// \~russian @exception std::runtime_error если `m_is_mocked` истинно, но мок-объекты не зарегистрированы.
    template<typename ... TArgs>
    static ::std::shared_ptr<TClass> create(TArgs && ... args);

    /// \~english @brief Represents an expected behavior for a mocked method.
    ///     Instances of this class are typically created and configured using the
    ///     `DEFINE_MOCK_BEHAVIOR` or `DEFINE_MOCK_BEHAVIOR_CONST` macros.
    ///     The `operator=` is used to assign a lambda function that defines the action of the mocked method.
    /// \~russian @brief Представляет ожидаемое поведение для мокированного метода.
    ///     Экземпляры этого класса обычно создаются и настраиваются с использованием макросов
    ///     `DEFINE_MOCK_BEHAVIOR` или `DEFINE_MOCK_BEHAVIOR_CONST`.
    ///     Оператор `=` используется для присвоения лямбда-функции, определяющей действие мокированного метода.
    /// \~english @tparam TResult The return type of the mocked method.
    /// \~russian @tparam TResult Тип возвращаемого значения мокированного метода.
    /// \~english @tparam TArgs Variadic template arguments representing the parameter types of the mocked method.
    /// \~russian @tparam TArgs Вариативные шаблонные аргументы, представляющие типы параметров мокированного метода.
    template<typename TResult, typename ... TArgs>
    class Behavior;

    /// \~english @brief Specialization of `Behavior` for non-const member functions.
    /// \~russian @brief Специализация `Behavior` для неконстантных функций-членов.
    template<typename TResult, typename ... TArgs>
    class Behavior<TResult(TClass::*)(TArgs...)>:
        public pattern::NonCopyable, public pattern::NonMovable
    {
    public:
        /// \~english @brief Constructs a `Behavior` object.
        /// \~russian @brief Конструирует объект `Behavior`.
        /// \~english @param mock The `Mock` instance this behavior is associated with.
        /// \~russian @param mock Экземпляр `Mock`, с которым связано это поведение.
        /// \~english @param method A pointer to the `std::type_info` of the method signature this behavior is for.
        /// \~russian @param method Указатель на `std::type_info` сигнатуры метода, для которого предназначено это поведение.
        explicit Behavior(Mock &mock, std::type_info const *method);

        /// \~english @brief Assigns a lambda function as the action for this behavior.
        /// \~russian @brief Присваивает лямбда-функцию в качестве действия для этого поведения.
        /// \~english @tparam TLambda The type of the lambda function.
        /// \~russian @tparam TLambda Тип лямбда-функции.
        /// \~english @param l The lambda function defining the behavior.
        /// \~russian @param l Лямбда-функция, определяющая поведение.
        /// \~english @return A reference to this `Behavior` object.
        /// \~russian @return Ссылка на этот объект `Behavior`.
        template<typename TLambda>
        Behavior &operator=(TLambda &&l);

    private:
        /// \~english @brief Reference to the parent `Mock` object.
        /// \~russian @brief Ссылка на родительский объект `Mock`.
        Mock &m_mock;
        /// \~english @brief Type information for the mocked method signature.
        /// \~russian @brief Информация о типе для сигнатуры мокированного метода.
        std::type_info const *m_method;
    };

    /// \~english @brief Specialization of `Behavior` for const member functions.
    /// \~russian @brief Специализация `Behavior` для константных функций-членов.
    template<typename TResult, typename ... TArgs>
    class Behavior<TResult(TClass::*)(TArgs...) const>:
        public pattern::NonCopyable, public pattern::NonMovable
    {
    public:
        /// \~english @brief Constructs a `Behavior` object for a const method.
        /// \~russian @brief Конструирует объект `Behavior` для константного метода.
        /// \~english @param mock The `Mock` instance this behavior is associated with.
        /// \~russian @param mock Экземпляр `Mock`, с которым связано это поведение.
        /// \~english @param method A pointer to the `std::type_info` of the const method signature this behavior is for.
        /// \~russian @param method Указатель на `std::type_info` сигнатуры константного метода, для которого предназначено это поведение.
        explicit Behavior(Mock &mock, std::type_info const *method);

        /// \~english @brief Assigns a lambda function as the action for this const method behavior.
        /// \~russian @brief Присваивает лямбда-функцию в качестве действия для этого поведения константного метода.
        /// \~english @tparam TLambda The type of the lambda function.
        /// \~russian @tparam TLambda Тип лямбда-функции.
        /// \~english @param l The lambda function defining the behavior.
        /// \~russian @param l Лямбда-функция, определяющая поведение.
        /// \~english @return A reference to this `Behavior` object.
        /// \~russian @return Ссылка на этот объект `Behavior`.
        template<typename TLambda>
        Behavior &operator=(TLambda &&l);

    private:
        /// \~english @brief Reference to the parent `Mock` object.
        /// \~russian @brief Ссылка на родительский объект `Mock`.
        Mock &m_mock;
        /// \~english @brief Type information for the mocked const method signature.
        /// \~russian @brief Информация о типе для сигнатуры мокированного константного метода.
        std::type_info const *m_method;
    };

protected:
    /// \~english @brief Called by `DEFINE_MOCK_METHOD` macros to invoke the registered behavior for a non-const method.
    /// \~russian @brief Вызывается макросами `DEFINE_MOCK_METHOD` для вызова зарегистрированного поведения для неконстантного метода.
    /// \~english @tparam TResult The return type of the method.
    /// \~russian @tparam TResult Тип возвращаемого значения метода.
    /// \~english @tparam TArgs Variadic types of the method arguments.
    /// \~russian @tparam TArgs Вариативные типы аргументов метода.
    /// \~english @param args Arguments passed to the mocked method.
    /// \~russian @param args Аргументы, переданные в мокированный метод.
    /// \~english @return The result from the registered behavior.
    /// \~russian @return Результат зарегистрированного поведения.
    /// \~english @exception std::runtime_error if no behavior is registered for the method or if type mismatch occurs.
    /// \~russian @exception std::runtime_error если для метода не зарегистрировано поведение или происходит несоответствие типов.
    template<typename TResult, typename ... TArgs>
    auto call(TArgs && ... args);
    /// \~english @brief Called by `DEFINE_MOCK_METHOD_CONST` macros to invoke the registered behavior for a const method.
    /// \~russian @brief Вызывается макросами `DEFINE_MOCK_METHOD_CONST` для вызова зарегистрированного поведения для константного метода.
    /// \~english @tparam TResult The return type of the method.
    /// \~russian @tparam TResult Тип возвращаемого значения метода.
    /// \~english @tparam TArgs Variadic types of the method arguments.
    /// \~russian @tparam TArgs Вариативные типы аргументов метода.
    /// \~english @param args Arguments passed to the mocked method.
    /// \~russian @param args Аргументы, переданные в мокированный метод.
    /// \~english @return The result from the registered behavior.
    /// \~russian @return Результат зарегистрированного поведения.
    /// \~english @exception std::runtime_error if no behavior is registered for the method or if type mismatch occurs.
    /// \~russian @exception std::runtime_error если для метода не зарегистрировано поведение или происходит несоответствие типов.
    template<typename TResult, typename ... TArgs>
    auto call(TArgs && ... args) const;

private:
    /// \~english @brief Sets the behavior (lambda) for a non-const mocked method. Used internally by `Behavior::operator=`.
    /// \~russian @brief Устанавливает поведение (лямбду) для неконстантного мокированного метода. Используется внутри `Behavior::operator=`.
    /// \~english @tparam TResult Return type of the method.
    /// \~russian @tparam TResult Тип возвращаемого значения метода.
    /// \~english @tparam TArgs Argument types of the method.
    /// \~russian @tparam TArgs Типы аргументов метода.
    /// \~english @param method Type info of the method signature.
    /// \~russian @param method Информация о типе сигнатуры метода.
    /// \~english @param f The lambda function defining the behavior.
    /// \~russian @param f Лямбда-функция, определяющая поведение.
    template<typename TResult, typename ... TArgs>
    void setBehavior(std::type_info const *method, std::function<TResult(TArgs...)> const &&f);

    /// \~english @brief Sets the behavior (lambda) for a const mocked method. Used internally by `Behavior::operator=`.
    /// \~russian @brief Устанавливает поведение (лямбду) для константного мокированного метода. Используется внутри `Behavior::operator=`.
    /// \~english @tparam TResult Return type of the method.
    /// \~russian @tparam TResult Тип возвращаемого значения метода.
    /// \~english @tparam TArgs Argument types of the method.
    /// \~russian @tparam TArgs Типы аргументов метода.
    /// \~english @param method Type info of the method signature.
    /// \~russian @param method Информация о типе сигнатуры метода.
    /// \~english @param f The lambda function defining the behavior.
    /// \~russian @param f Лямбда-функция, определяющая поведение.
    template<typename TResult, typename ... TArgs>
    void setBehavior(std::type_info const *method, std::function<TResult(TArgs...) const> const &&f);

    /// \~english @brief Map storing behaviors for mocked methods, keyed by method signature type info. `std::any` holds the `std::function`.
    /// \~russian @brief Карта, хранящая поведение для мокированных методов, с ключом по информации о типе сигнатуры метода. `std::any` содержит `std::function`.
    mutable std::unordered_map<std::type_info const *, std::any>   m_map_name_behavior;
    /// \~english @brief Reverse map from a behavior's internal ID (if any, currently seems unused based on structure) to method signature type info.
    /// \~russian @brief Обратная карта от внутреннего идентификатора поведения (если есть, в настоящее время, похоже, не используется) к информации о типе сигнатуры метода.
    mutable std::unordered_map<void *, std::type_info const *>     m_map_behavior_name; // Potentially for more complex behavior management or reflection.

    /// \~english @brief Static list of all created mock objects of this type. Used by `create()` when `m_is_mocked` is true.
    /// \~russian @brief Статический список всех созданных мок-объектов этого типа. Используется `create()` когда `m_is_mocked` истинно.
    static std::list<Mock<TClass> *>    m_mock_objects;
    /// \~english @brief Static flag indicating if mocking is active. If true, `create()` returns registered mocks; otherwise, it creates real objects.
    /// \~russian @brief Статический флаг, указывающий, активно ли мокирование. Если true, `create()` возвращает зарегистрированные моки; иначе создает реальные объекты.
    static bool                         m_is_mocked;
};

/// \~english @brief Manages a sequence of expected method calls on a mock object.
///     This class allows defining a strict order in which methods are expected to be called,
///     along with specific arguments for those calls.
/// \~russian @brief Управляет последовательностью ожидаемых вызовов методов на мок-объекте.
///     Этот класс позволяет определить строгий порядок, в котором ожидаются вызовы методов,
///     а также конкретные аргументы для этих вызовов.
/// \~english @tparam TClassMock The type of the mock class (e.g., `MyInterfaceMock`) for which the sequence is defined.
/// \~russian @tparam TClassMock Тип мок-класса (например, `MyInterfaceMock`), для которого определяется последовательность.
template<typename TClassMock>
class MockSequence {
public:
    /// \~english @brief Constructs a `MockSequence`.
    /// \~russian @brief Конструирует `MockSequence`.
    /// \~english @param mock A reference to the mock object on which the sequence of calls is expected.
    /// \~russian @param mock Ссылка на мок-объект, на котором ожидается последовательность вызовов.
    /// \~english @param file_line A string representing the file and line where the sequence is defined, for error reporting.
    /// \~russian @param file_line Строка, представляющая файл и строку, где определена последовательность, для сообщений об ошибках.
    MockSequence(TClassMock &mock, std::string const &file_line)
    :
        m_mock      (mock),
        m_file_line (file_line),
        m_call_count(0)
    {}

    /// \~english @brief Adds an expectation for a non-const method call to the sequence.
    /// \~russian @brief Добавляет ожидание вызова неконстантного метода в последовательность.
    /// \~english @tparam TResult Return type of the expected method.
    /// \~russian @tparam TResult Тип возвращаемого значения ожидаемого метода.
    /// \~english @tparam TMethodArgs Parameter types of the expected method.
    /// \~russian @tparam TMethodArgs Типы параметров ожидаемого метода.
    /// \~english @tparam TCallArgs Types of arguments provided for matching the expectation.
    /// \~russian @tparam TCallArgs Типы аргументов, предоставленных для сопоставления с ожиданием.
    /// \~english @param file_line File and line where this expectation is defined.
    /// \~russian @param file_line Файл и строка, где определено это ожидание.
    /// \~english @param call_count_max Maximum expected call count (currently seems unused in `step`).
    /// \~russian @param call_count_max Максимальное ожидаемое количество вызовов (в настоящее время, похоже, не используется в `step`).
    /// \~english @param call_count_min Minimum expected call count (currently seems unused in `step`).
    /// \~russian @param call_count_min Минимальное ожидаемое количество вызовов (в настоящее время, похоже, не используется в `step`).
    /// \~english @param method Pointer to the member function that is expected to be called.
    /// \~russian @param method Указатель на функцию-член, вызов которой ожидается.
    /// \~english @param args Arguments that are expected to be passed to the method.
    /// \~russian @param args Аргументы, которые, как ожидается, будут переданы методу.
    /// \~english @return Reference to this `MockSequence` object for chaining.
    /// \~russian @return Ссылка на этот объект `MockSequence` для цепочки вызовов.
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

    /// \~english @brief Adds an expectation for a const method call to the sequence.
    /// \~russian @brief Добавляет ожидание вызова константного метода в последовательность.
    /// \~english @tparam TResult Return type of the expected method.
    /// \~russian @tparam TResult Тип возвращаемого значения ожидаемого метода.
    /// \~english @tparam TMethodArgs Parameter types of the expected method.
    /// \~russian @tparam TMethodArgs Типы параметров ожидаемого метода.
    /// \~english @tparam TCallArgs Types of arguments provided for matching the expectation.
    /// \~russian @tparam TCallArgs Типы аргументов, предоставленных для сопоставления с ожиданием.
    /// \~english @param file_line File and line where this expectation is defined.
    /// \~russian @param file_line Файл и строка, где определено это ожидание.
    /// \~english @param call_count_max Maximum expected call count (currently seems unused in `step`).
    /// \~russian @param call_count_max Максимальное ожидаемое количество вызовов (в настоящее время, похоже, не используется в `step`).
    /// \~english @param call_count_min Minimum expected call count (currently seems unused in `step`).
    /// \~russian @param call_count_min Минимальное ожидаемое количество вызовов (в настоящее время, похоже, не используется в `step`).
    /// \~english @param method Pointer to the const member function that is expected to be called.
    /// \~russian @param method Указатель на константную функцию-член, вызов которой ожидается.
    /// \~english @param args Arguments that are expected to be passed to the method.
    /// \~russian @param args Аргументы, которые, как ожидается, будут переданы методу.
    /// \~english @return Reference to this `MockSequence` object for chaining.
    /// \~russian @return Ссылка на этот объект `MockSequence` для цепочки вызовов.
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
           &typeid(static_cast<TResult (TClassMock::TOriginalClass::*)(TMethodArgs...)>(method)), // Intentional: typeid of non-const version for map key?
            std::tuple<std::decay_t<TMethodArgs> ...>(std::forward<TCallArgs>(args) ...));
        m_expectations.push_back(expectation);
        return *this;
    }

    /// \~english @brief Compares stored arguments (in `std::any`) with current call arguments.
    /// \~russian @brief Сравнивает сохраненные аргументы (в `std::any`) с текущими аргументами вызова.
    /// \~english @tparam TCallArgs Types of the current call arguments.
    /// \~russian @tparam TCallArgs Типы текущих аргументов вызова.
    /// \~english @param stored_any The `std::any` object holding the expected arguments tuple.
    /// \~russian @param stored_any Объект `std::any`, содержащий кортеж ожидаемых аргументов.
    /// \~english @param args The current call arguments.
    /// \~russian @param args Текущие аргументы вызова.
    /// \~english @return True if types match and arguments are equal, false otherwise.
    /// \~russian @return True, если типы совпадают и аргументы равны, иначе false.
    template<typename ... TCallArgs>
    bool isEqual(const std::any &stored_any, TCallArgs &&... args) {
        using TTuple = std::tuple<std::decay_t<TCallArgs>...>;

        if (stored_any.type() != typeid(TTuple)) {
            return false;
        }

        const TTuple &stored_tuple = std::any_cast<const TTuple&>(stored_any);
        TTuple current_tuple(std::forward<TCallArgs>(args)...);

        return stored_tuple == current_tuple;
    }

    /// \~english @brief Throws a runtime error for sequence violations.
    /// \~russian @brief Выбрасывает исключение `std::runtime_error` при нарушениях последовательности.
    /// \~english @tparam TClass The type of the class whose method was unexpectedly called.
    /// \~russian @tparam TClass Тип класса, метод которого был неожиданно вызван.
    /// \~english @tparam TMethod The type of the method that was unexpectedly called.
    /// \~russian @tparam TMethod Тип неожиданно вызванного метода.
    /// \~english @param method Type info of the method that was called.
    /// \~russian @param method Информация о типе вызванного метода.
    /// \~english @param file_line File and line where the sequence was defined.
    /// \~russian @param file_line Файл и строка, где была определена последовательность.
    /// \~english @param error Specific error message.
    /// \~russian @param error Конкретное сообщение об ошибке.
    template<typename TClass, typename TMethod>
    void throwException(TMethod const &method, std::string const &file_line, std::string const &error) {
        throw std::runtime_error(std::string("sequence at '") + file_line + "' mock '" +
            typeid(TClass).name() + "' expectation error: unexpected call '" +
            typeid(method).name() + ", " + error);
    }

    /// \~english @brief Advances the sequence check for a method call.
    ///     Verifies if the current method call matches the next expectation in the sequence.
    ///     If matched, the expectation is removed. Throws if the sequence is violated.
    ///     Note: Call count min/max checks seem commented out in the original source.
    /// \~russian @brief Продвигает проверку последовательности для вызова метода.
    ///     Проверяет, соответствует ли текущий вызов метода следующему ожиданию в последовательности.
    ///     При совпадении ожидание удаляется. Выбрасывает исключение при нарушении последовательности.
    ///     Примечание: Проверки минимального/максимального количества вызовов, похоже, закомментированы в исходном коде.
    /// \~english @tparam TClass Type of the class being mocked.
    /// \~russian @tparam TClass Тип мокируемого класса.
    /// \~english @tparam TResult Return type of the method.
    /// \~russian @tparam TResult Тип возвращаемого значения метода.
    /// \~english @tparam TMethodArgs Parameter types of the method.
    /// \~russian @tparam TMethodArgs Типы параметров метода.
    /// \~english @tparam TCallArgs Types of the arguments in the current call.
    /// \~russian @tparam TCallArgs Типы аргументов в текущем вызове.
    /// \~english @param method Pointer to the member function that was called.
    /// \~russian @param method Указатель на функцию-член, которая была вызвана.
    /// \~english @param args Arguments passed in the current call.
    /// \~russian @param args Аргументы, переданные в текущем вызове.
    template<typename TClass, typename TResult, typename... TMethodArgs, typename... TCallArgs>
    void step(
        TResult (TClass::*method)(TMethodArgs...),
        TCallArgs && ... args)
    {
        if (m_expectations.empty())
            throwException<TClass>(method, m_file_line, "sequence is empty");

        auto expectation = m_expectations.front();
        if (expectation.arguments.has_value() && !isEqual(expectation.arguments, args ...))
            throwException<TClass>(method, expectation.file_line, "not equal arguments");

//        if (m_call_count > expectation.call_count_max)
//            throwException<TClass>(method, expectation.file_line, "calls count more then max " +
//                convertion::convert<std::string>(expectation.call_count_max));
//
//        if (m_call_count < expectation.call_count_min)
//            throwException<TClass>(method, expectation.file_line, "calls count less then min " +
//                convertion::convert<std::string>(expectation.call_count_max));

        m_call_count = 0;
        m_expectations.pop_front();
    }

private:
    /// \~english @brief Represents a single expected method call within a sequence.
    /// \~russian @brief Представляет одно ожидаемое обращение к методу в рамках последовательности.
    struct TExpectation {
        /// \~english @brief Constructs a `TExpectation`.
        /// \~russian @brief Конструирует `TExpectation`.
        /// \~english @param file_line File and line where the expectation is defined, for error reporting.
        /// \~russian @param file_line Файл и строка, где определено ожидание, для сообщений об ошибках.
        /// \~english @param call_count_min Minimum number of times this call is expected (currently seems unused).
        /// \~russian @param call_count_min Минимальное количество ожидаемых вызовов (в настоящее время, похоже, не используется).
        /// \~english @param call_count_max Maximum number of times this call is expected (currently seems unused).
        /// \~russian @param call_count_max Максимальное количество ожидаемых вызовов (в настоящее время, похоже, не используется).
        /// \~english @param method_name Type info of the expected method's signature.
        /// \~russian @param method_name Информация о типе сигнатуры ожидаемого метода.
        /// \~english @param arguments `std::any` holding a tuple of expected arguments. Empty if arguments are not checked.
        /// \~russian @param arguments `std::any`, содержащий кортеж ожидаемых аргументов. Пусто, если аргументы не проверяются.
        TExpectation(
            std::string     const file_line,
            size_t          const &call_count_min,
            size_t          const &call_count_max,
            std::type_info  const *method_name,
            std::any        const &arguments = {})
        :
            file_line       (file_line),
            call_count_min  (call_count_min),
            call_count_max  (call_count_max),
            method_name     (method_name),
            arguments       (arguments)
        {}
        /// \~english @brief File and line where the expectation was defined.
        /// \~russian @brief Файл и строка, где было определено ожидание.
        std::string             const file_line;
        /// \~english @brief Minimum expected call count (currently not enforced by `step`).
        /// \~russian @brief Минимальное ожидаемое количество вызовов (в настоящее время не проверяется `step`).
        size_t                  const call_count_min;
        /// \~english @brief Maximum expected call count (currently not enforced by `step`).
        /// \~russian @brief Максимальное ожидаемое количество вызовов (в настоящее время не проверяется `step`).
        size_t                  const call_count_max;
        /// \~english @brief Pointer to the `std::type_info` of the expected method's signature.
        /// \~russian @brief Указатель на `std::type_info` сигнатуры ожидаемого метода.
        std::type_info  const * const method_name;
        /// \~english @brief `std::any` storing a `std::tuple` of the expected arguments for the method call.
        /// \~russian @brief `std::any`, хранящий `std::tuple` ожидаемых аргументов для вызова метода.
        std::any                const arguments;
    };

    /// \~english @brief Reference to the mock object this sequence applies to.
    /// \~russian @brief Ссылка на мок-объект, к которому применяется эта последовательность.
    TClassMock             &m_mock;
    /// \~english @brief File and line where the `DEFINE_MOCK_SEQUENCE` was invoked.
    /// \~russian @brief Файл и строка, где был вызван `DEFINE_MOCK_SEQUENCE`.
    std::string             m_file_line;
    /// \~english @brief List of pending expectations in this sequence.
    /// \~russian @brief Список ожидающих выполнения ожиданий в этой последовательности.
    std::list<TExpectation> m_expectations;
    /// \~english @brief Current call count for the current expectation (seems to be reset in `step` without full use).
    /// \~russian @brief Текущее количество вызовов для текущего ожидания (похоже, сбрасывается в `step` без полного использования).
    size_t                  m_call_count;
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
template<typename TResult, typename ... TArgs>
auto Mock<TClass>::call(TArgs && ... args) {
    auto method_type_info = &typeid(TResult (TOriginalClass::*)(TArgs ...));
    auto i = m_map_name_behavior.find(method_type_info);
    if (i == m_map_name_behavior.end())
        throw std::runtime_error("mock calling error: unexpected call '" + std::string(method_type_info->name()) + "'");

    try {
        using TMethod = std::function<TResult (TArgs ...)>;
        auto method = std::any_cast<TMethod>(i->second);
        return method(std::forward<TArgs>(args)...);
    } catch (const std::bad_any_cast&) {
        throw std::runtime_error("mock calling error: type mismatch for method '" + std::string(method_type_info->name()) + "'");
    }
}


template<typename TClass>
template<typename TResult, typename ... TArgs>
auto Mock<TClass>::call(TArgs && ... args) const {
    auto method_type_info = &typeid(TResult (TOriginalClass::*)(TArgs ...) const);
    auto i = m_map_name_behavior.find(method_type_info);
    if (i == m_map_name_behavior.end())
        throw std::runtime_error("mock calling error: unexpected call '" + std::string(method_type_info->name()) + "'");

    try {
        using TMethod = std::function<TResult (TArgs ...)>;
        const auto& method = std::any_cast<const TMethod&>(i->second);
        return method(std::forward<TArgs>(args)...);
    } catch (const std::bad_any_cast&) {
        throw std::runtime_error("mock calling error: type mismatch for method '" + std::string(method_type_info->name()) + "'");
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


/// \~english @def DEFINE_MOCK_METHOD_2(TResult, methodName)
/// @brief Defines a mock method override with no arguments.
/// \~russian @def DEFINE_MOCK_METHOD_2(TResult, methodName)
/// @brief Определяет переопределение мок-метода без аргументов.
/// \~english @param TResult The return type of the method.
/// \~russian @param TResult Тип возвращаемого значения метода.
/// \~english @param methodName The name of the method to mock.
/// \~russian @param methodName Имя мокируемого метода.
#define DEFINE_MOCK_METHOD_2(TResult, methodName) \
public: \
    TResult methodName() override { \
        return this->call<TResult>(); \
    }

/// \~english @def DEFINE_MOCK_METHOD_3(TResult, methodName, A1)
/// @brief Defines a mock method override with one argument.
/// \~russian @def DEFINE_MOCK_METHOD_3(TResult, methodName, A1)
/// @brief Определяет переопределение мок-метода с одним аргументом.
/// \~english @param TResult The return type.
/// \~russian @param TResult Тип возвращаемого значения.
/// \~english @param methodName The method name.
/// \~russian @param methodName Имя метода.
/// \~english @param A1 Type of the first argument.
/// \~russian @param A1 Тип первого аргумента.
#define DEFINE_MOCK_METHOD_3(TResult, methodName, A1) \
public: \
    TResult methodName(A1 a1) override { \
        return this->call<TResult>(a1); \
    }

/// \~english @def DEFINE_MOCK_METHOD_4(TResult, methodName, A1, A2)
/// @brief Defines a mock method override with two arguments.
/// \~russian @def DEFINE_MOCK_METHOD_4(TResult, methodName, A1, A2)
/// @brief Определяет переопределение мок-метода с двумя аргументами.
/// \~english @param TResult Return type. @param methodName Method name. @param A1 Type of arg1. @param A2 Type of arg2.
/// \~russian @param TResult Тип возврата. @param methodName Имя метода. @param A1 Тип арг1. @param A2 Тип арг2.
#define DEFINE_MOCK_METHOD_4(TResult, methodName, A1, A2) \
public: \
    TResult methodName(A1 a1, A2 a2) override { \
        return this->call<TResult>(a1, a2); \
    }

/// \~english @def DEFINE_MOCK_METHOD_5(TResult, methodName, A1, A2, A3)
/// @brief Defines a mock method override with three arguments.
/// \~russian @def DEFINE_MOCK_METHOD_5(TResult, methodName, A1, A2, A3)
/// @brief Определяет переопределение мок-метода с тремя аргументами.
/// \~english @param TResult Return type. @param methodName Method name. @param A1 Type of arg1. @param A2 Type of arg2. @param A3 Type of arg3.
/// \~russian @param TResult Тип возврата. @param methodName Имя метода. @param A1 Тип арг1. @param A2 Тип арг2. @param A3 Тип арг3.
#define DEFINE_MOCK_METHOD_5(TResult, methodName, A1, A2, A3) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3) override { \
        return this->call<TResult>(a1, a2, a3); \
    }

/// \~english @def DEFINE_MOCK_METHOD_6(TResult, methodName, A1, A2, A3, A4)
/// @brief Defines a mock method override with four arguments.
/// \~russian @def DEFINE_MOCK_METHOD_6(TResult, methodName, A1, A2, A3, A4)
/// @brief Определяет переопределение мок-метода с четырьмя аргументами.
/// \~english @param TResult Return type. @param methodName Method name. @param A1 Type of arg1. @param A2 Type of arg2. @param A3 Type of arg3. @param A4 Type of arg4.
/// \~russian @param TResult Тип возврата. @param methodName Имя метода. @param A1 Тип арг1. @param A2 Тип арг2. @param A3 Тип арг3. @param A4 Тип арг4.
#define DEFINE_MOCK_METHOD_6(TResult, methodName, A1, A2, A3, A4) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3, A4 a4) override { \
        return this->call<TResult>(a1, a2, a3, a4); \
    }

/// \~english @def DEFINE_MOCK_METHOD(...)
/// @brief Chooser macro for defining non-const mock method overrides, dispatching to `DEFINE_MOCK_METHOD_N` based on argument count.
/// \~russian @def DEFINE_MOCK_METHOD(...)
/// @brief Макрос-переключатель для определения переопределений неконстантных мок-методов, диспетчеризующий в `DEFINE_MOCK_METHOD_N` на основе количества аргументов.
#define DEFINE_MOCK_METHOD(...) \
    DEFINE_MACRO_CHOOSER(DEFINE_MOCK_METHOD, __VA_ARGS__)(__VA_ARGS__)

/// \~english @def DEFINE_MOCK_METHOD_CONST_2(TResult, methodName)
/// @brief Defines a const mock method override with no arguments.
/// \~russian @def DEFINE_MOCK_METHOD_CONST_2(TResult, methodName)
/// @brief Определяет переопределение константного мок-метода без аргументов.
/// \~english @param TResult The return type of the method.
/// \~russian @param TResult Тип возвращаемого значения метода.
/// \~english @param methodName The name of the method to mock.
/// \~russian @param methodName Имя мокируемого метода.
#define DEFINE_MOCK_METHOD_CONST_2(TResult, methodName) \
public: \
    TResult methodName() const override { \
        return this->call<TResult>(); \
    }

/// \~english @def DEFINE_MOCK_METHOD_CONST_3(TResult, methodName, A1)
/// @brief Defines a const mock method override with one argument.
/// \~russian @def DEFINE_MOCK_METHOD_CONST_3(TResult, methodName, A1)
/// @brief Определяет переопределение константного мок-метода с одним аргументом.
/// \~english @param TResult Return type. @param methodName Method name. @param A1 Type of arg1.
/// \~russian @param TResult Тип возврата. @param methodName Имя метода. @param A1 Тип арг1.
#define DEFINE_MOCK_METHOD_CONST_3(TResult, methodName, A1) \
public: \
    TResult methodName(A1 a1) const override { \
        return this->call<TResult>(a1); \
    }

/// \~english @def DEFINE_MOCK_METHOD_CONST_4(TResult, methodName, A1, A2)
/// @brief Defines a const mock method override with two arguments.
/// \~russian @def DEFINE_MOCK_METHOD_CONST_4(TResult, methodName, A1, A2)
/// @brief Определяет переопределение константного мок-метода с двумя аргументами.
/// \~english @param TResult Return type. @param methodName Method name. @param A1 Type of arg1. @param A2 Type of arg2.
/// \~russian @param TResult Тип возврата. @param methodName Имя метода. @param A1 Тип арг1. @param A2 Тип арг2.
#define DEFINE_MOCK_METHOD_CONST_4(TResult, methodName, A1, A2) \
public: \
    TResult methodName(A1 a1, A2 a2) const override { \
        return this->call<TResult>(a1, a2); \
    }

/// \~english @def DEFINE_MOCK_METHOD_CONST_5(TResult, methodName, A1, A2, A3)
/// @brief Defines a const mock method override with three arguments.
/// \~russian @def DEFINE_MOCK_METHOD_CONST_5(TResult, methodName, A1, A2, A3)
/// @brief Определяет переопределение константного мок-метода с тремя аргументами.
/// \~english @param TResult Return type. @param methodName Method name. @param A1 Type of arg1. @param A2 Type of arg2. @param A3 Type of arg3.
/// \~russian @param TResult Тип возврата. @param methodName Имя метода. @param A1 Тип арг1. @param A2 Тип арг2. @param A3 Тип арг3.
#define DEFINE_MOCK_METHOD_CONST_5(TResult, methodName, A1, A2, A3) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3) const override { \
        return this->call<TResult>(a1, a2, a3); \
    }

/// \~english @def DEFINE_MOCK_METHOD_CONST_6(TResult, methodName, A1, A2, A3, A4)
/// @brief Defines a const mock method override with four arguments.
/// \~russian @def DEFINE_MOCK_METHOD_CONST_6(TResult, methodName, A1, A2, A3, A4)
/// @brief Определяет переопределение константного мок-метода с четырьмя аргументами.
/// \~english @param TResult Return type. @param methodName Method name. @param A1 Type of arg1. @param A2 Type of arg2. @param A3 Type of arg3. @param A4 Type of arg4.
/// \~russian @param TResult Тип возврата. @param methodName Имя метода. @param A1 Тип арг1. @param A2 Тип арг2. @param A3 Тип арг3. @param A4 Тип арг4.
#define DEFINE_MOCK_METHOD_CONST_6(TResult, methodName, A1, A2, A3, A4) \
public: \
    TResult methodName(A1 a1, A2 a2, A3 a3, A4 a4) const override { \
        return this->call<TResult>(a1, a2, a3, a4); \
    }

/// \~english @def DEFINE_MOCK_METHOD_CONST(...)
/// @brief Chooser macro for defining const mock method overrides, dispatching to `DEFINE_MOCK_METHOD_CONST_N` based on argument count.
/// \~russian @def DEFINE_MOCK_METHOD_CONST(...)
/// @brief Макрос-переключатель для определения переопределений константных мок-методов, диспетчеризующий в `DEFINE_MOCK_METHOD_CONST_N` на основе количества аргументов.
#define DEFINE_MOCK_METHOD_CONST(...) \
    DEFINE_MACRO_CHOOSER(DEFINE_MOCK_METHOD_CONST, __VA_ARGS__)(__VA_ARGS__)

/// \~english @def DEFINE_MOCK_CLASS(Interface)
/// @brief Defines a mock class named `Interface##Mock` that inherits from `Interface` and `iridium::testing::Mock<Interface>`.
/// \~russian @def DEFINE_MOCK_CLASS(Interface)
/// @brief Определяет мок-класс с именем `Interface##Mock`, который наследует от `Interface` и `iridium::testing::Mock<Interface>`.
/// \~english @param Interface The name of the interface or class to be mocked.
/// \~russian @param Interface Имя интерфейса или класса, для которого создается мок.
#define DEFINE_MOCK_CLASS(Interface) \
class Interface##Mock: public Interface, public ::iridium::testing::Mock<Interface>

/// \~english @def DEFINE_MOCK_CONSTRUCTOR(Interface)
/// @brief Defines a constructor for a mock class (`Interface##Mock`) that forwards its arguments to the constructor of the base `Interface`.
/// \~russian @def DEFINE_MOCK_CONSTRUCTOR(Interface)
/// @brief Определяет конструктор для мок-класса (`Interface##Mock`), который передает свои аргументы конструктору базового класса `Interface`.
/// \~english @param Interface The name of the interface or class being mocked (used to name the mock class).
/// \~russian @param Interface Имя интерфейса или класса, для которого создается мок (используется для именования мок-класса).
#define DEFINE_MOCK_CONSTRUCTOR(Interface) \
public: \
template<typename ... TArgs> \
Interface##Mock(TArgs && ... args): Interface(std::forward<TArgs>(args) ...) {};

/// \~english @def DEFINE_MOCK_BEHAVIOR(result_type, method_name, mock_object, ...)
/// @brief Sets the behavior for a non-const mocked method.
///     This macro is used to specify what a mocked method should do when called (e.g., return a value, throw an exception, execute custom code).
///     It creates a `Mock::Behavior` object and assigns a lambda to it.
/// \~russian @def DEFINE_MOCK_BEHAVIOR(result_type, method_name, mock_object, ...)
/// @brief Устанавливает поведение для неконстантного мокированного метода.
///     Этот макрос используется для указания того, что должен делать мокированный метод при вызове (например, вернуть значение, выбросить исключение, выполнить пользовательский код).
///     Он создает объект `Mock::Behavior` и присваивает ему лямбду.
/// \~english @param result_type The return type of the mocked method.
/// \~russian @param result_type Тип возвращаемого значения мокированного метода.
/// \~english @param method_name The name of the method whose behavior is being defined.
/// \~russian @param method_name Имя метода, поведение которого определяется.
/// \~english @param mock_object The mock object instance.
/// \~russian @param mock_object Экземпляр мок-объекта.
/// \~english @param ... The signature of the method arguments (types and names), e.g., `(int x, std::string y)`.
/// \~russian @param ... Сигнатура аргументов метода (типы и имена), например, `(int x, std::string y)`.
#define DEFINE_MOCK_BEHAVIOR(result_type, method_name, mock_object, ...)                         \
::iridium::testing::Mock<std::remove_reference_t<decltype(mock_object)>::TOriginalClass>::Behavior<                  \
    decltype(static_cast<result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*) \
    (__VA_ARGS__)>(&std::remove_reference_t<decltype(mock_object)>::method_name))>               \
(                                                                                                \
    mock_object,                                                                                 \
    &typeid(static_cast<result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*) \
    (__VA_ARGS__)>(&std::remove_reference_t<decltype(mock_object)>::method_name))                \
) = [&](__VA_ARGS__)

/// \~english @def DEFINE_MOCK_BEHAVIOR_CONST(result_type, method_name, mock_object, ...)
/// @brief Sets the behavior for a const mocked method.
///     Similar to `DEFINE_MOCK_BEHAVIOR` but for const member functions.
/// \~russian @def DEFINE_MOCK_BEHAVIOR_CONST(result_type, method_name, mock_object, ...)
/// @brief Устанавливает поведение для константного мокированного метода.
///     Аналогичен `DEFINE_MOCK_BEHAVIOR`, но для константных функций-членов.
/// \~english @param result_type The return type of the mocked const method.
/// \~russian @param result_type Тип возвращаемого значения мокированного константного метода.
/// \~english @param method_name The name of the const method whose behavior is being defined.
/// \~russian @param method_name Имя константного метода, поведение которого определяется.
/// \~english @param mock_object The mock object instance.
/// \~russian @param mock_object Экземпляр мок-объекта.
/// \~english @param ... The signature of the method arguments (types and names), e.g., `(int x, std::string y)`.
/// \~russian @param ... Сигнатура аргументов метода (типы и имена), например, `(int x, std::string y)`.
#define DEFINE_MOCK_BEHAVIOR_CONST(result_type, method_name, mock_object, ...)                   \
::iridium::testing::Mock<std::remove_reference_t<decltype(mock_object)>::TOriginalClass>::Behavior<                  \
    decltype(static_cast<result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*) \
    (__VA_ARGS__) const>(&std::remove_reference_t<decltype(mock_object)>::method_name))>         \
(                                                                                                \
    mock_object,                                                                                 \
    &typeid(static_cast<result_type (std::remove_reference_t<decltype(mock_object)>::TOriginalClass::*) \
    (__VA_ARGS__) const>(&std::remove_reference_t<decltype(mock_object)>::method_name))          \
) = [&](__VA_ARGS__)

/// \~english @def DEFINE_MOCK_SEQUENCE(name)
/// @brief Defines a `MockSequence` object for verifying ordered expectations.
/// \~russian @def DEFINE_MOCK_SEQUENCE(name)
/// @brief Определяет объект `MockSequence` для проверки упорядоченных ожиданий.
/// \~english @param name The name of the sequence object to be created (e.g., `s1` creates `sequence_s1`).
/// \~russian @param name Имя создаваемого объекта последовательности (например, `s1` создает `sequence_s1`).
#define DEFINE_MOCK_SEQUENCE(name) \
::iridium::testing::MockSequence sequence_##name(__FILE__, __LINE__, #name)

/// \~english @def DEFINE_MOCK_SEQUENCE_EXPECTATION(sequence_name, mock, method)
/// @brief Adds an expectation to a `MockSequence`.
///     This macro is likely intended to be part of a chained call or used with other macros to specify arguments and call counts.
///     The current definition seems to pass `mock`, `&method`, `#method`, `__FILE__`, `__LINE__` to `addExpectation`,
///     implying it sets up a basic expectation for `method` on `mock` within `sequence_name`.
/// \~russian @def DEFINE_MOCK_SEQUENCE_EXPECTATION(sequence_name, mock, method)
/// @brief Добавляет ожидание в `MockSequence`.
///     Этот макрос, вероятно, предназначен для использования в цепочке вызовов или с другими макросами для указания аргументов и количества вызовов.
///     Текущее определение, похоже, передает `mock`, `&method`, `#method`, `__FILE__`, `__LINE__` в `addExpectation`,
///     подразумевая, что оно устанавливает базовое ожидание для `method` на `mock` в рамках `sequence_name`.
/// \~english @param sequence_name The name of the sequence (previously defined with `DEFINE_MOCK_SEQUENCE`) to add this expectation to.
/// \~russian @param sequence_name Имя последовательности (ранее определенной с помощью `DEFINE_MOCK_SEQUENCE`), в которую добавляется это ожидание.
/// \~english @param mock The mock object instance.
/// \~russian @param mock Экземпляр мок-объекта.
/// \~english @param method The method name (unqualified) that is expected to be called.
/// \~russian @param method Имя метода (неполное), вызов которого ожидается.
#define DEFINE_MOCK_SEQUENCE_EXPECTATION(sequence_name, mock, method) \
sequence_##sequence_name.addExpectation(mock, &method, #method, __FILE__, __LINE__)


#endif // HEADER_MOCK_67D176F4_9136_4225_974D_B12E2C3C7BC2
