// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_OBSERVER_D355605F_6C18_40F5_A7C1_509CA5AA9BC6
#define HEADER_OBSERVER_D355605F_6C18_40F5_A7C1_509CA5AA9BC6


#include "iridium/smart_ptr.h"


namespace iridium {
namespace pattern {


/// \~english @brief Interface for the 'Observer' part of the Observer design pattern.
///     Objects implementing this interface can subscribe to `IObservable` instances
///     to be notified of events of type `TEvent`. When an event occurs, the observable
///     will call the `handle` method of its subscribed observers.
/// \~russian @brief Интерфейс для 'Наблюдателя' в рамках шаблона проектирования "Наблюдатель".
///     Объекты, реализующие этот интерфейс, могут подписываться на экземпляры `IObservable`
///     для получения уведомлений о событиях типа `TEvent`. Когда происходит событие, наблюдаемый объект
///     вызовет метод `handle` своих подписанных наблюдателей.
/// \~english @tparam TEvent The type of event data that this observer is designed to handle.
/// \~russian @tparam TEvent Тип данных события, для обработки которого предназначен этот наблюдатель.
template <typename TEvent>
class IObserver {
public:
    /// \~english @brief Defines common interface elements for `IObserver`, such as smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.).
    /// \~russian @brief Определяет общие элементы интерфейса для `IObserver`, такие как typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.).
    DEFINE_INTERFACE(IObserver)
    /// \~english @brief Handles an event received from an `IObservable` to which this observer is subscribed.
    ///     This method is called by the `IObservable` when a relevant event occurs.
    /// \~russian @brief Обрабатывает событие, полученное от `IObservable`, на который подписан этот наблюдатель.
    ///     Этот метод вызывается `IObservable` при возникновении соответствующего события.
    /// \~english @param event The event data of type `TEvent` that needs to be handled by the observer.
    /// \~russian @param event Данные события типа `TEvent`, которые должны быть обработаны наблюдателем.
    virtual void handleEvent(TEvent const &event) = 0;
};


} // pattern
} // iridium


#endif // HEADER_OBSERVER_D355605F_6C18_40F5_A7C1_509CA5AA9BC6
