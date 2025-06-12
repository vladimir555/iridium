// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_PROTOTYPE_248BDC92_79E1_458D_85F8_A8E49C19564B
#define HEADER_PROTOTYPE_248BDC92_79E1_458D_85F8_A8E49C19564B


#include "iridium/smart_ptr.h"


namespace iridium {
namespace pattern {

/// \~english @brief Interface for the Prototype design pattern.
///     This pattern allows for the creation of new objects by copying an existing object (the prototype),
///     without being dependent on their concrete classes. The `TSubject` template parameter
///     typically refers to the base class or interface of the objects that are being cloned,
///     ensuring that the clone method returns a pointer of that general type.
/// \~russian @brief Интерфейс для шаблона проектирования "Прототип".
///     Этот шаблон позволяет создавать новые объекты путем копирования существующего объекта (прототипа),
///     не завися от их конкретных классов. Параметр шаблона `TSubject`
///     обычно ссылается на базовый класс или интерфейс клонируемых объектов,
///     гарантируя, что метод клонирования возвращает указатель этого общего типа.
/// \~english @tparam TSubject The type of the object that will be cloned. This is often the class itself
///     that implements this interface, or a base class/interface type in a hierarchy.
/// \~russian @tparam TSubject Тип объекта, который будет клонирован. Часто это сам класс,
///     реализующий этот интерфейс, или тип базового класса/интерфейса в иерархии.
template<typename TSubject>
class IPrototype {
public:
    /// \~english @brief Defines standard smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.) for `IPrototype<TSubject>`.
    /// \~russian @brief Определяет стандартные typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.) для `IPrototype<TSubject>`.
    DEFINE_SMART_PTR(IPrototype<TSubject>)
    /// \~english @brief Virtual default destructor.
    ///     Ensures proper destruction of derived classes through a base pointer.
    /// \~russian @brief Виртуальный деструктор по умолчанию.
    ///     Обеспечивает правильное уничтожение производных классов через указатель на базовый класс.
    virtual ~IPrototype() = default;
    /// \~english @brief Creates a clone (a new instance that is a copy) of the current object.
    ///     The exact nature of the copy (deep vs. shallow) depends on the implementation in the concrete derived class.
    /// \~russian @brief Создает клон (новый экземпляр, являющийся копией) текущего объекта.
    ///     Точная природа копии (глубокая или поверхностная) зависит от реализации в конкретном производном классе.
    /// \~english @return A `std::shared_ptr<TSubject>` pointing to the newly created clone of this object.
    /// \~russian @return `std::shared_ptr<TSubject>`, указывающий на вновь созданный клон этого объекта.
    virtual std::shared_ptr<TSubject> clone() const = 0;
};


} // pattern
} // iridium


#endif // HEADER_PROTOTYPE_248BDC92_79E1_458D_85F8_A8E49C19564B
