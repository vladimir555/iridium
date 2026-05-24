// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
#define HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08


#include "iridium/smart_ptr.h"


namespace iridium::pattern {


/// \~english @brief Interface for objects that require initialization and finalization.
/// \~russian @brief Интерфейс для объектов, требующих инициализации и финализации.
class IInitializable {
public:
    DEFINE_SMART_PTR(IInitializable)
    virtual ~IInitializable() = default;

    /// \~english @brief Initializes the object.
    /// \~russian @brief Инициализирует объект.
    virtual void initialize() = 0;
    /// \~english @brief Finalizes the object.
    /// \~russian @brief Завершает работу объекта.
    virtual void finalize() = 0;
};


} // namespace iridium::pattern


#endif // HEADER_INITIALIZABLE_298C30DD_3F15_4BD7_813C_B2CF00E06F08
