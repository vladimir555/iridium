// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_INITIALIZER_A79AA8C7_D7C5_492D_A771_2633A8E098DA
#define HEADER_INITIALIZER_A79AA8C7_D7C5_492D_A771_2633A8E098DA


#include "iridium/pattern/initializable.h"

#include <list>
#include <stdexcept>


namespace iridium::pattern::implementation {

/// \~english @brief A concrete implementation of `IInitializable` that manages a collection of other `IInitializable` objects.
///     This class allows grouping multiple initializable components and managing their lifecycle (initialization and finalization)
///     as a single unit. It ensures that all managed objects are initialized in the order they were provided
///     and finalized in the reverse order.
/// \~russian @brief Конкретная реализация `IInitializable`, управляющая коллекцией других объектов `IInitializable`.
///     Этот класс позволяет группировать несколько инициализируемых компонентов и управлять их жизненным циклом
///     (инициализация и финализация) как единым целым. Он гарантирует, что все управляемые объекты
///     инициализируются в порядке их предоставления и финализируются в обратном порядке.
class CInitializer: public IInitializable {
public:
    /// \~english @brief Defines standard implementation helpers for `CInitializer`, such as a static `create` method for shared pointer instantiation.
    /// \~russian @brief Определяет стандартные вспомогательные средства реализации для `CInitializer`, такие как статический метод `create` для создания экземпляров через разделяемые указатели.
    DEFINE_CREATE(CInitializer)
//    CInitializer(std::list<IInitializable::TSharedPtr> const &items);
    /// \~english @brief Constructs a `CInitializer` with a variable number of `IInitializable` items.
    ///     The items are stored internally and will be initialized and finalized by this `CInitializer` instance.
    /// \~russian @brief Конструирует `CInitializer` с переменным количеством элементов `IInitializable`.
    ///     Элементы сохраняются внутри и будут инициализированы и финализированы этим экземпляром `CInitializer`.
    /// \~english @tparam TArgs A variadic template parameter pack, where each type in `TArgs` is expected to be convertible to `IInitializable::TSharedPtr`.
    /// \~russian @tparam TArgs Вариативный пакет шаблонных параметров, где каждый тип в `TArgs` должен быть преобразуем в `IInitializable::TSharedPtr`.
    /// \~english @param items A parameter pack of shared pointers to `IInitializable` objects.
    /// \~russian @param items Пакет параметров разделяемых указателей на объекты `IInitializable`.
    /// \~english @exception std::runtime_error if any of the provided `items` is a nullptr.
    /// \~russian @exception std::runtime_error если какой-либо из предоставленных `items` является nullptr.
    template<typename ... TArgs>
    CInitializer(TArgs const &... items);
    /// \~english @brief Destructor. Ensures that `finalize()` is called if the items were initialized and not yet finalized.
    ///     This helps prevent resource leaks if `finalize()` was not explicitly called.
    /// \~russian @brief Деструктор. Гарантирует вызов `finalize()`, если элементы были инициализированы и еще не финализированы.
    ///     Это помогает предотвратить утечки ресурсов, если `finalize()` не был вызван явно.
    virtual ~CInitializer();

    /// \~english @brief Initializes all managed `IInitializable` objects in the order they were added.
    ///     If any object's initialization fails (throws an exception), subsequent objects will not be initialized.
    ///     Sets an internal flag to indicate that initialization has been performed.
    /// \~russian @brief Инициализирует все управляемые объекты `IInitializable` в порядке их добавления.
    ///     Если инициализация какого-либо объекта завершается неудачей (выбрасывает исключение), последующие объекты не будут инициализированы.
    ///     Устанавливает внутренний флаг, указывающий, что инициализация была выполнена.
    /// \~english @exception std::exception Propagates exceptions thrown by the `initialize()` method of any managed item.
    /// \~russian @exception std::exception Распространяет исключения, выброшенные методом `initialize()` любого управляемого элемента.
    void initialize()   override;
    /// \~english @brief Finalizes all managed `IInitializable` objects in the reverse order they were added.
    ///     Attempts to finalize all items even if some finalizations fail (exceptions from one `finalize` call
    ///     do not prevent subsequent `finalize` calls, though the first exception might be propagated after all attempts).
    ///     Resets an internal flag to indicate that finalization has been performed.
    /// \~russian @brief Финализирует все управляемые объекты `IInitializable` в обратном порядке их добавления.
    ///     Пытается финализировать все элементы, даже если некоторые финализации завершаются неудачей (исключения из одного вызова `finalize`
    ///     не предотвращают последующие вызовы `finalize`, хотя первое исключение может быть распространено после всех попыток).
    ///     Сбрасывает внутренний флаг, указывающий, что финализация была выполнена.
    /// \~english @exception std::exception May propagate an exception if one or more `finalize()` calls fail. Typically the first one encountered.
    /// \~russian @exception std::exception Может распространять исключение, если один или несколько вызовов `finalize()` завершаются неудачей. Обычно первое встреченное.
    void finalize()     override;

private:
    /// \~english @brief Flag indicating whether the `initialize()` method has been successfully called (and `finalize()` has not yet been called).
    /// \~russian @brief Флаг, указывающий, был ли успешно вызван метод `initialize()` (и метод `finalize()` еще не был вызван).
    bool                                    m_is_initialized;
    /// \~english @brief List of `IInitializable` objects managed by this initializer instance.
    /// \~russian @brief Список объектов `IInitializable`, управляемых этим экземпляром инициализатора.
    std::list<IInitializable::TSharedPtr>   m_items;
};

/// \~english @details Initializes the `m_items` list with the provided pack of shared pointers to `IInitializable` objects.
///     It iterates through the items to ensure none are null, throwing `std::runtime_error` if a null item is found.
/// \~russian @details Инициализирует список `m_items` предоставленным пакетом разделяемых указателей на объекты `IInitializable`.
///     Итерирует по элементам, чтобы убедиться, что ни один из них не является null, выбрасывая `std::runtime_error` при обнаружении null-элемента.
template<typename ... TArgs>
CInitializer::CInitializer(TArgs const &... items)
:
    m_is_initialized    (false),
    m_items             {items ...}
{
    for (auto const &item: m_items)
        if (!item)
            throw std::runtime_error("create initializer error: nullptr item"); // ----->
}


} // iridium::pattern::implementation


#endif // HEADER_INITIALIZER_A79AA8C7_D7C5_492D_A771_2633A8E098DA
