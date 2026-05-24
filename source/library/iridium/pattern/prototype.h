// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROTOTYPE_248BDC92_79E1_458D_85F8_A8E49C19564B
#define HEADER_PROTOTYPE_248BDC92_79E1_458D_85F8_A8E49C19564B


#include "iridium/smart_ptr.h"


namespace iridium::pattern {


/// \~english @brief Interface for the Prototype design pattern.
/// \~english @details Classes inheriting from this interface can be cloned to create new instances.
/// \~russian @brief Интерфейс для паттерна проектирования "Прототип".
/// \~russian @details Классы, наследующие от этого интерфейса, могут быть клонированы для создания новых экземпляров.
template<typename TSubject>
class IPrototype {
public:
    /// \~english @brief Defines standard smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.) for `IPrototype<TSubject>`.
    /// \~russian @brief Определяет стандартные typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.) для `IPrototype<TSubject>`.
    DEFINE_SMART_PTR(IPrototype<TSubject>)
    virtual ~IPrototype() = default;

    /// \~english @brief Creates a clone of the object.
    /// \~russian @brief Создает клон объекта.
    /// \~english @return A shared pointer to the new cloned object.
    /// \~russian @return Умный указатель на новый клонированный объект.
    virtual std::shared_ptr<TSubject> clone() const = 0;
};


} // namespace iridium::pattern


#endif // HEADER_PROTOTYPE_248BDC92_79E1_458D_85F8_A8E49C19564B
