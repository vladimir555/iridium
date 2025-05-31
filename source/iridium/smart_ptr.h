// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Provides helper macros for defining standard smart pointer typedefs and static `create` factory methods for classes, with optional support for mocking.
/// \~russian @file
/// @brief Предоставляет вспомогательные макросы для определения стандартных typedef-ов умных указателей и статических фабричных методов `create` для классов, с опциональной поддержкой мокирования.

#ifndef HEADER_SMART_PTR_C1E6B022_A228_47C3_9E76_E3C64E43617E
#define HEADER_SMART_PTR_C1E6B022_A228_47C3_9E76_E3C64E43617E


#include <memory>

/// \~english @def DEFINE_SMART_PTR(TClass)
/// @brief Defines standard smart pointer typedefs for a given class `TClass`.
/// This includes `TSharedPtr`, `TWeakPtr`, `TUniquePtr`, and their constant counterparts
/// (`TConstSharedPtr`, `TConstWeakPtr`, `TConstUniquePtr`).
/// \~russian @def DEFINE_SMART_PTR(TClass)
/// @brief Определяет стандартные typedef-ы умных указателей для заданного класса `TClass`.
/// Включает `TSharedPtr`, `TWeakPtr`, `TUniquePtr` и их константные аналоги
/// (`TConstSharedPtr`, `TConstWeakPtr`, `TConstUniquePtr`).
/// \~english @param TClass The class for which to define smart pointer typedefs.
/// \~russian @param TClass Класс, для которого определяются typedef-ы умных указателей.
#define DEFINE_SMART_PTR(TClass) \
typedef ::std::shared_ptr<TClass>         TSharedPtr; \
typedef ::std::weak_ptr  <TClass>         TWeakPtr; \
typedef ::std::unique_ptr<TClass>         TUniquePtr; \
typedef ::std::shared_ptr<TClass const>   TConstSharedPtr; \
typedef ::std::weak_ptr  <TClass const>   TConstWeakPtr; \
typedef ::std::unique_ptr<TClass const>   TConstUniquePtr;


#ifdef DEFINE_MOCK_CREATE
#include "iridium/testing/mock.h"
#endif // DEFINE_MOCK_CREATE


#ifdef DEFINE_MOCK_CREATE
/// \~english @def DEFINE_CREATE(Interface)
/// @brief Defines a static `create` factory method for a class `Interface`, typically an interface type when mocking is enabled.
/// If `DEFINE_MOCK_CREATE` is defined, this macro generates a `create` method that forwards
/// the call to `iridium::testing::Mock<Interface>::create(...)`, allowing for mock object instantiation.
/// It also includes smart pointer definitions via `DEFINE_SMART_PTR(Interface)`.
/// \~russian @def DEFINE_CREATE(Interface)
/// @brief Определяет статический фабричный метод `create` для класса `Interface`, обычно являющегося типом интерфейса при включенном мокировании.
/// Если определен `DEFINE_MOCK_CREATE`, этот макрос генерирует метод `create`, который перенаправляет
/// вызов в `iridium::testing::Mock<Interface>::create(...)`, позволяя создавать мок-объекты.
/// Также включает определения умных указателей через `DEFINE_SMART_PTR(Interface)`.
/// \~english @param Interface The class or interface for which to define the `create` method and smart pointers. When `DEFINE_MOCK_CREATE` is active, this is typically the interface type being mocked.
/// \~russian @param Interface Класс или интерфейс, для которого определяется метод `create` и умные указатели. Когда активен `DEFINE_MOCK_CREATE`, это обычно тип мокируемого интерфейса.
/// \~english @tparam TArgs Variadic template arguments for the constructor of the object being created.
/// \~russian @tparam TArgs Вариативные шаблонные аргументы для конструктора создаваемого объекта.
#define DEFINE_CREATE(Interface) \
DEFINE_SMART_PTR(Interface) \
template<typename ... TArgs> \
static auto create(TArgs && ... args) { \
return ::iridium::testing::Mock<Interface>::create(std::forward<TArgs>(args) ...); \
};

#else
/// \~english @def DEFINE_CREATE(TClass)
/// @brief Defines a static `create` factory method for a class `TClass`.
/// If `DEFINE_MOCK_CREATE` is not defined, this macro generates a `create` method that uses
/// `std::make_shared<TClass>(...)` to instantiate objects.
/// It also includes smart pointer definitions via `DEFINE_SMART_PTR(TClass)`.
/// \~russian @def DEFINE_CREATE(TClass)
/// @brief Определяет статический фабричный метод `create` для класса `TClass`.
/// Если `DEFINE_MOCK_CREATE` не определен, этот макрос генерирует метод `create`, который использует
/// `std::make_shared<TClass>(...)` для создания экземпляров объектов.
/// Также включает определения умных указателей через `DEFINE_SMART_PTR(TClass)`.
/// \~english @param TClass The class for which to define the `create` method and smart pointers.
/// \~russian @param TClass Класс, для которого определяется метод `create` и умные указатели.
/// \~english @tparam TArgs Variadic template arguments for the constructor of `TClass`.
/// \~russian @tparam TArgs Вариативные шаблонные аргументы для конструктора `TClass`.
#define DEFINE_CREATE(TClass) \
DEFINE_SMART_PTR(TClass) \
template<typename ... TArgs> \
static ::std::shared_ptr<TClass> create(TArgs && ... args) { \
    return ::std::make_shared<TClass>(std::forward<TArgs>(args) ...); \
};

#endif // DEFINE_MOCK_CREATE

/// \~english @def DEFINE_INTERFACE(TClass)
/// @brief A convenience macro for defining interfaces.
/// It combines `DEFINE_SMART_PTR(TClass)` to declare standard smart pointer typedefs
/// and declares a virtual default destructor, which is essential for interfaces.
/// \~russian @def DEFINE_INTERFACE(TClass)
/// @brief Вспомогательный макрос для определения интерфейсов.
/// Объединяет `DEFINE_SMART_PTR(TClass)` для объявления стандартных typedef-ов умных указателей
/// и объявляет виртуальный деструктор по умолчанию, что необходимо для интерфейсов.
/// \~english @param TClass The interface class name.
/// \~russian @param TClass Имя класса интерфейса.
#define DEFINE_INTERFACE(TClass) \
DEFINE_SMART_PTR(TClass) \
virtual ~TClass() = default;

/// \~english @def DEFINE_IMPLEMENTATION(TClass)
/// @brief A convenience macro for defining implementation classes.
/// It combines `DEFINE_CREATE(TClass)` to declare a static `create` factory method
/// (and associated smart pointers) and declares an overridden virtual default destructor.
/// \~russian @def DEFINE_IMPLEMENTATION(TClass)
/// @brief Вспомогательный макрос для определения классов реализации.
/// Объединяет `DEFINE_CREATE(TClass)` для объявления статического фабричного метода `create`
/// (и связанных с ним умных указателей) и объявляет переопределенный виртуальный деструктор по умолчанию.
/// \~english @param TClass The implementation class name.
/// \~russian @param TClass Имя класса реализации.
#define DEFINE_IMPLEMENTATION(TClass) \
DEFINE_CREATE(TClass) \
virtual ~TClass() override = default;


#endif // HEADER_SMART_PTR_C1E6B022_A228_47C3_9E76_E3C64E43617E
