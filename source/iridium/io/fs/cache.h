// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines the interface for a generic string cache.
/// \~russian @brief Определяет интерфейс для обобщенного кэша строк.
#ifndef HEADER_CACHE_F566D6D8_C42D_4049_9642_C600B057A128
#define HEADER_CACHE_F566D6D8_C42D_4049_9642_C600B057A128


#include "iridium/smart_ptr.h"           // For DEFINE_SMART_PTR and TSharedPtr
#include "iridium/pattern/initializable.h" // For IInitializable

#include <string>
#include <list>   // Not directly used in interface, but common for collections
#include <cstddef> // For size_t


namespace iridium::io::fs {


/// \~english @brief Interface for a cache that stores strings and retrieves them using numeric IDs.
///     This interface defines operations for adding strings to the cache (which returns an ID),
///     retrieving strings by ID, removing entries, clearing the cache, and checking its state.
///     It inherits from `IInitializable`, suggesting that cache instances may require an initialization phase.
///     Given its location in the `fs` namespace, it might be intended for caching file lines, paths, or other file-related string data.
/// \~russian @brief Интерфейс для кэша, который хранит строки и извлекает их с использованием числовых идентификаторов.
///     Этот интерфейс определяет операции для добавления строк в кэш (что возвращает идентификатор),
///     извлечения строк по идентификатору, удаления записей, очистки кэша и проверки его состояния.
///     Наследуется от `IInitializable`, что предполагает, что экземпляры кэша могут требовать фазы инициализации.
///     Учитывая его расположение в пространстве имен `fs`, он может быть предназначен для кэширования строк файлов, путей или других строковых данных, связанных с файлами.
class ICache: public pattern::IInitializable {
public:
    /// \~english @brief Defines smart pointer typedefs (e.g., `ICache::TSharedPtr`, `ICache::TUniquePtr`) for this interface.
    /// \~russian @brief Определяет typedef'ы умных указателей (например, `ICache::TSharedPtr`, `ICache::TUniquePtr`) для этого интерфейса.
    DEFINE_SMART_PTR(ICache)
    virtual ~ICache() = default;

    virtual size_t push(std::string const &line) = 0;
    virtual std::string get(size_t const &id) const = 0;
    virtual void remove(size_t const &id) = 0;
    virtual void clean() = 0;
    virtual size_t size() const = 0;
    virtual bool checkExistense(size_t const &id) const = 0;
    virtual void flush() = 0;
};


} // iridium::io::fs


#endif // HEADER_CACHE_F566D6D8_C42D_4049_9642_C600B057A128
