// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_BROADCASTER_843F1FB7_7A80_4BAF_8C67_457F4D84B287
#define HEADER_BROADCASTER_843F1FB7_7A80_4BAF_8C67_457F4D84B287


#include <unordered_set>
#include <functional>


namespace iridium::pattern {


// ----- interface

/// \~english @brief Implements the Broadcaster pattern, allowing messages to be sent to multiple receivers.
/// \~english @details This class manages a collection of receivers and provides a mechanism to invoke a specific member function on all of them.
/// \~russian @brief Реализует паттерн "Вещатель", позволяя отправлять сообщения нескольким получателям.
/// \~russian @details Этот класс управляет коллекцией получателей и предоставляет механизм для вызова определенной функции-члена у всех из них.
template<typename TReceiver>
class Broadcaster {
public:
    Broadcaster() = default;
    virtual ~Broadcaster() = default;

    /// \~english @brief Attaches a receiver to the broadcaster.
    /// \~russian @brief Присоединяет получателя к вещателю.
    /// \~english @param receiver The receiver to attach.
    /// \~russian @param receiver Получатель для присоединения.
    void attach(typename TReceiver::TSharedPtr const &receiver);

    /// \~english @brief Detaches a receiver from the broadcaster.
    /// \~russian @brief Отсоединяет получателя от вещателя.
    /// \~english @param receiver The receiver to detach.
    /// \~russian @param receiver Получатель для отсоединения.
    void detach(typename TReceiver::TSharedPtr const &receiver);

    /// \~english @brief Broadcasts a message by invoking a member function on all attached receivers.
    /// \~russian @brief Отправляет сообщение, вызывая функцию-член у всех присоединенных получателей.
    /// \~english @tparam TReceiver_ The receiver class type.
    /// \~russian @tparam TReceiver_ Тип класса получателя.
    /// \~english @tparam TArgs The types of the arguments for the member function.
    /// \~russian @tparam TArgs Типы аргументов для функции-члена.
    /// \~english @param f A pointer to the member function to be called.
    /// \~russian @param f Указатель на функцию-член, которую нужно вызвать.
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


} // namespace iridium::pattern


#endif // HEADER_BROADCASTER_843F1FB7_7A80_4BAF_8C67_457F4D84B287
