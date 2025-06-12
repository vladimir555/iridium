// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_CONTAINER_EE72B9D8_E36F_48F2_BF16_F9AF024EAE2A
#define HEADER_CONTAINER_EE72B9D8_E36F_48F2_BF16_F9AF024EAE2A


#include "iridium/smart_ptr.h"


namespace iridium {
namespace pattern {

/// \~english @brief A generic interface for a container that can hold items of a specific type.
///     This interface defines the basic operations for adding an item to the container
///     and deleting an item from the container. Concrete implementations will provide
///     the specific storage mechanism and item management logic.
/// \~russian @brief Обобщенный интерфейс для контейнера, который может хранить элементы определенного типа.
///     Этот интерфейс определяет базовые операции для добавления элемента в контейнер
///     и удаления элемента из контейнера. Конкретные реализации будут предоставлять
///     специфический механизм хранения и логику управления элементами.
/// \~english @tparam TItem The type of items to be stored in the container.
/// \~russian @tparam TItem Тип элементов, которые будут храниться в контейнере.
template<typename TItem>
class IContainer {
public:
    /// \~english @brief Defines common interface elements for `IContainer`, such as smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.).
    /// \~russian @brief Определяет общие элементы интерфейса для `IContainer`, такие как typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.).
    DEFINE_INTERFACE(IContainer)
    /// \~english @brief Adds an item to the container.
    /// \~russian @brief Добавляет элемент в контейнер.
    /// \~english @param item The item to be added to the container.
    /// \~russian @param item Элемент для добавления в контейнер.
    /// \~english @exception std::logic_error May be thrown if the item cannot be added (e.g., if it's a duplicate in a container that requires unique items, or if the container is full and has a fixed capacity).
    /// \~russian @exception std::logic_error Может быть выброшено, если элемент не может быть добавлен (например, если это дубликат в контейнере, требующем уникальных элементов, или если контейнер полон и имеет фиксированную емкость).
    virtual void add(TItem const &item) = 0;
    /// \~english @brief Deletes an item from the container.
    /// \~russian @brief Удаляет элемент из контейнера.
    /// \~english @param item The item to be deleted from the container.
    /// \~russian @param item Элемент для удаления из контейнера.
    /// \~english @exception std::logic_error May be thrown if the item is not found in the container or cannot be deleted.
    /// \~russian @exception std::logic_error Может быть выброшено, если элемент не найден в контейнере или не может быть удален.
    virtual void del(TItem const &item) = 0;
};


} // pattern
} // iridium


#endif // HEADER_CONTAINER_EE72B9D8_E36F_48F2_BF16_F9AF024EAE2A
