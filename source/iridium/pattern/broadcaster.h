// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_BROADCASTER_843F1FB7_7A80_4BAF_8C67_457F4D84B287
#define HEADER_BROADCASTER_843F1FB7_7A80_4BAF_8C67_457F4D84B287


#include <unordered_set>
#include <functional>


namespace iridium {
namespace pattern {


// ----- interface

/// \~english @brief A class template implementing the broadcaster (or subject) role in an observer-like pattern.
///     It manages a collection of receiver objects (observers) and provides a mechanism to
///     broadcast a specific method call to all registered receivers. This allows for decoupling
///     the broadcaster from its receivers.
/// \~russian @brief Шаблон класса, реализующий роль вещателя (или субъекта) в шаблоне, подобном "Наблюдателю".
///     Управляет коллекцией объектов-получателей (наблюдателей) и предоставляет механизм
///     для трансляции вызова определенного метода всем зарегистрированным получателям. Это позволяет
///     отделить вещателя от его получателей.
/// \~english @tparam TReceiver The type of the receiver objects. This type must expose a `TSharedPtr`
///     typedef (e.g., `typedef std::shared_ptr<MyReceiverType> TSharedPtr;`) for its shared pointer type,
///     which is used for managing receiver instances.
/// \~russian @tparam TReceiver Тип объектов-получателей. Этот тип должен предоставлять typedef `TSharedPtr`
///     (например, `typedef std::shared_ptr<MyReceiverType> TSharedPtr;`) для своего типа разделяемого указателя,
///     который используется для управления экземплярами получателей.
template<typename TReceiver>
class Broadcaster {
public:
    /// \~english @brief Default constructor.
    /// \~russian @brief Конструктор по умолчанию.
    Broadcaster() = default;
    /// \~english @brief Virtual default destructor.
    /// \~russian @brief Виртуальный деструктор по умолчанию.
    virtual ~Broadcaster() = default;
    /// \~english @brief Attaches a receiver to this broadcaster.
    ///     The receiver will then be notified of subsequent broadcasts.
    /// \~russian @brief Присоединяет получателя к этому вещателю.
    ///     После этого получатель будет уведомляться о последующих трансляциях.
    /// \~english @param receiver A shared pointer to the receiver object to attach.
    /// \~russian @param receiver Разделяемый указатель на объект-получатель для присоединения.
    /// \~english @exception std::logic_error if the receiver is already attached (duplicate).
    /// \~russian @exception std::logic_error если получатель уже присоединен (дубликат).
    void attach(typename TReceiver::TSharedPtr const &receiver);
    /// \~english @brief Detaches a receiver from this broadcaster.
    ///     The receiver will no longer be notified of broadcasts.
    /// \~russian @brief Отсоединяет получателя от этого вещателя.
    ///     Получатель больше не будет уведомляться о трансляциях.
    /// \~english @param receiver A shared pointer to the receiver object to detach.
    /// \~russian @param receiver Разделяемый указатель на объект-получатель для отсоединения.
    /// \~english @exception std::logic_error if the receiver was not previously attached.
    /// \~russian @exception std::logic_error если получатель не был ранее присоединен.
    void detach(typename TReceiver::TSharedPtr const &receiver);
    /// \~english @brief Broadcasts a call to a specific member function of each attached receiver.
    ///     All attached receivers that are compatible with `TReceiver_` will have their
    ///     member function `f` invoked with the provided arguments.
    /// \~russian @brief Транслирует вызов определенной функции-члена каждому присоединенному получателю.
    ///     У всех присоединенных получателей, совместимых с `TReceiver_`, будет вызвана
    ///     их функция-член `f` с предоставленными аргументами.
    /// \~english @tparam TReceiver_ The actual type of the receiver on which the member function exists.
    ///     Should be `TReceiver` or a type derived from/compatible with it.
    /// \~russian @tparam TReceiver_ Фактический тип получателя, у которого существует функция-член.
    ///     Должен быть `TReceiver` или тип, производный/совместимый с ним.
    /// \~english @tparam TArgs Variadic template parameter pack for the arguments of the member function.
    /// \~russian @tparam TArgs Вариативный пакет шаблонных параметров для аргументов функции-члена.
    /// \~english @param f A pointer to the member function to be called on each receiver.
    /// \~russian @param f Указатель на функцию-член, которая будет вызвана у каждого получателя.
    /// \~english @param args The arguments to be passed to the member function.
    /// \~russian @param args Аргументы, которые будут переданы в функцию-член.
    template<typename TReceiver_, typename ... TArgs>
    void broadcast(void(TReceiver_::*f)(TArgs const &...), TArgs const &... args);

private:
    /// \~english @brief Container holding shared pointers to the attached receiver objects.
    ///     Using `std::unordered_set` ensures uniqueness of attached receivers.
    /// \~russian @brief Контейнер, хранящий разделяемые указатели на присоединенные объекты-получатели.
    ///     Использование `std::unordered_set` обеспечивает уникальность присоединенных получателей.
    std::unordered_set<typename TReceiver::TSharedPtr> m_receivers;
};


// ----- implementation


template<typename TReceiver>
void Broadcaster<TReceiver>::attach(typename TReceiver::TSharedPtr const &receiver)
{
    if (!m_receivers.insert(receiver).second) {
        throw std::logic_error("attach duplicate receiver"); // ----->
    }
}


template<typename TReceiver>
void Broadcaster<TReceiver>::detach(typename TReceiver::TSharedPtr const &receiver)
{
    if (m_receivers.erase(receiver) == 0) {
        throw std::logic_error("detach unatached receiver"); // ----->
    }
}


template<typename TReceiver>
template<typename TReceiver_, typename... TArgs>
void Broadcaster<TReceiver>::broadcast(void (TReceiver_::*f)(TArgs const &...), TArgs const &... args)
{
    for (auto i : m_receivers) {
        (i.get()->*f)(args...);
    }
}


} // pattern
} // iridium


#endif // HEADER_BROADCASTER_843F1FB7_7A80_4BAF_8C67_457F4D84B287
