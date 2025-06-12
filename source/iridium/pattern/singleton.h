// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_SINGLETON_33ED8344_7855_4F54_A108_7FF9BC7D71B4
#define HEADER_SINGLETON_33ED8344_7855_4F54_A108_7FF9BC7D71B4


#include "non_copyable.h"
#include "non_movable.h"

#include <memory>
#include <list>


namespace iridium {
namespace pattern {

/// \~english @brief A class template for implementing the Singleton design pattern.
///     This pattern ensures that only one instance of `TClass` can exist throughout the application.
///     The singleton instance is created on its first access via the `instance()` method.
///     Initialization is thread-safe in C++11 and later due to the guarantees for static local variable initialization.
///     The class inherits from `NonCopyable` and `NonMovable` to prevent copying and moving of the singleton manager itself,
///     though the managed `TClass` instance's copy/move semantics are independent.
///     To use, `TClass` should typically make `Singleton<TClass>` a friend or have a public default constructor.
/// \~russian @brief Шаблон класса для реализации шаблона проектирования "Одиночка" (Singleton).
///     Этот шаблон гарантирует, что в приложении может существовать только один экземпляр `TClass`.
///     Экземпляр-одиночка создается при первом доступе через метод `instance()`.
///     Инициализация является потокобезопасной в C++11 и более поздних версиях благодаря гарантиям инициализации статических локальных переменных.
///     Класс наследует от `NonCopyable` и `NonMovable` для предотвращения копирования и перемещения самого менеджера одиночки,
///     хотя семантика копирования/перемещения управляемого экземпляра `TClass` независима.
///     Для использования `TClass` обычно должен объявить `Singleton<TClass>` другом или иметь публичный конструктор по умолчанию.
/// \~english @tparam TClass The class for which the singleton instance is to be managed.
///     `TClass` must have a default constructor that is accessible to this `Singleton<TClass>` template
///     (e.g., it can be public, or `Singleton<TClass>` can be declared as a friend of `TClass`).
/// \~russian @tparam TClass Класс, для которого будет управляться экземпляр-одиночка.
///     `TClass` должен иметь конструктор по умолчанию, доступный этому шаблону `Singleton<TClass>`
///     (например, он может быть публичным, или `Singleton<TClass>` может быть объявлен другом `TClass`).
template<typename TClass>
class Singleton:
    public NonCopyable,
    public NonMovable
{
public:
    /// \~english @brief Virtual default destructor.
    ///     Ensures proper cleanup if `TClass` has a virtual destructor and `Singleton` is managed polymorphically (though rare for singletons).
    /// \~russian @brief Виртуальный деструктор по умолчанию.
    ///     Обеспечивает правильное удаление, если `TClass` имеет виртуальный деструктор и `Singleton` управляется полиморфно (хотя это редко для одиночек).
    virtual ~Singleton() = default;
    /// \~english @brief Provides access to the singleton instance of `TClass`.
    ///     The instance is created on the first call to this method.
    /// \~russian @brief Предоставляет доступ к экземпляру-одиночке `TClass`.
    ///     Экземпляр создается при первом вызове этого метода.
    /// \~english @return A reference to the single instance of `TClass`.
    /// \~russian @return Ссылка на единственный экземпляр `TClass`.
    static TClass &instance();
};

/// \~english @details The instance is managed by a static `std::shared_ptr`. This provides lazy initialization
///     (creation on first use) and ensures that the instance is properly destroyed when the program terminates.
///     The check `if (!instance)` ensures that the `new TClass()` is called only once.
///     In C++11 and later, the initialization of static local variables is thread-safe.
/// \~russian @details Экземпляр управляется статическим `std::shared_ptr`. Это обеспечивает ленивую инициализацию
///     (создание при первом использовании) и гарантирует, что экземпляр будет правильно уничтожен при завершении программы.
///     Проверка `if (!instance)` гарантирует, что `new TClass()` вызывается только один раз.
///     В C++11 и более поздних версиях инициализация статических локальных переменных является потокобезопасной.
template<typename TClass>
TClass &Singleton<TClass>::instance() {
    static std::shared_ptr<TClass> instance;
    
    if (!instance)
        instance = std::shared_ptr<TClass>(new TClass());

    return *instance; // ----->
}


} // pattern
} // iridium


#endif // HEADER_SINGLETON_33ED8344_7855_4F54_A108_7FF9BC7D71B4
