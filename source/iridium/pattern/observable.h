// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_OBSERVABLE_EDD92C20_D8E9_491E_B074_1B31B94F754C
#define HEADER_OBSERVABLE_EDD92C20_D8E9_491E_B074_1B31B94F754C


#include "observer.h"


namespace iridium {
namespace pattern {


/// \~english @brief Interface for the 'Observable' (or 'Subject') part of the Observer design pattern.
///     Objects implementing this interface can be observed by `IObserver` instances. When an event
///     of type `TEvent` occurs, the observable is responsible for notifying its subscribed observers.
/// \~russian @brief Интерфейс для 'Наблюдаемого объекта' (или 'Субъекта') в рамках шаблона проектирования "Наблюдатель".
///     Объекты, реализующие этот интерфейс, могут быть отслеживаемы экземплярами `IObserver`. Когда происходит
///     событие типа `TEvent`, наблюдаемый объект отвечает за уведомление своих подписанных наблюдателей.
/// \~english @tparam TEvent The type of event data that observers will be notified about. This defines the context of notifications.
/// \~russian @tparam TEvent Тип данных события, о котором будут уведомляться наблюдатели. Этот параметр определяет контекст уведомлений.
template<typename TEvent>
class IObservable {
public:
    /// \~english @brief Defines common interface elements for `IObservable`, such as smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.).
    /// \~russian @brief Определяет общие элементы интерфейса для `IObservable`, такие как typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.).
    DEFINE_INTERFACE(IObservable)
    /// \~english @brief Subscribes an observer to this observable object.
    ///     Once subscribed, the observer will receive notifications for events of type `TEvent` that occur in this observable.
    /// \~russian @brief Подписывает наблюдателя на этот наблюдаемый объект.
    ///     После подписки наблюдатель будет получать уведомления о событиях типа `TEvent`, происходящих в этом наблюдаемом объекте.
    /// \~english @param observer A shared pointer to an `IObserver<TEvent>` instance that wishes to receive updates.
    /// \~russian @param observer Разделяемый указатель на экземпляр `IObserver<TEvent>`, который желает получать обновления.
    /// \~english @exception std::logic_error May be thrown if the observer is already subscribed or if subscription is not possible for some reason.
    /// \~russian @exception std::logic_error Может быть выброшено, если наблюдатель уже подписан или если подписка невозможна по какой-либо причине.
    virtual void subscribe  (typename IObserver<TEvent>::TWeakPtr const &observer) = 0;
    /// \~english @brief Unsubscribes an observer from this observable object.
    ///     The observer will no longer receive notifications from this observable.
    /// \~russian @brief Отписывает наблюдателя от этого наблюдаемого объекта.
    ///     Наблюдатель больше не будет получать уведомления от этого наблюдаемого объекта.
    /// \~english @param observer A shared pointer to an `IObserver<TEvent>` instance to be unsubscribed.
    /// \~russian @param observer Разделяемый указатель на экземпляр `IObserver<TEvent>`, который должен быть отписан.
    /// \~english @exception std::logic_error May be thrown if the observer was not previously subscribed or cannot be found.
    /// \~russian @exception std::logic_error Может быть выброшено, если наблюдатель не был ранее подписан или не может быть найден.
    virtual void unsubscribe(typename IObserver<TEvent>::TWeakPtr const &observer) = 0;
    /// \~english @brief Notifies all subscribed observers of a new event.
    /// \~russian @brief Уведомляет всех подписанных наблюдателей о новом событии.
    virtual void notify     (TEvent const &event) = 0;
};

} // pattern
} // iridium


#endif // HEADER_OBSERVABLE_EDD92C20_D8E9_491E_B074_1B31B94F754C
