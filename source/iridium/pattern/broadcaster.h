// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_BROADCASTER_843F1FB7_7A80_4BAF_8C67_457F4D84B287
#define HEADER_BROADCASTER_843F1FB7_7A80_4BAF_8C67_457F4D84B287


#include <unordered_set>
#include <functional>


namespace iridium::pattern {


// ----- interface


template<typename TReceiver>
class Broadcaster {
public:
    Broadcaster() = default;
    virtual ~Broadcaster() = default;
    void attach(typename TReceiver::TSharedPtr const &receiver);
    void detach(typename TReceiver::TSharedPtr const &receiver);
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
