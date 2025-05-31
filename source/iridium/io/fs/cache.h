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


namespace iridium {
namespace io {
/// \~english @brief Namespace for file system related I/O functionalities.
/// \~russian @brief Пространство имен для функциональности ввода-вывода, связанной с файловой системой.
namespace fs {


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

    /// \~english @brief Virtual destructor for the interface.
    ///     Ensures proper cleanup of derived cache implementations.
    /// \~russian @brief Виртуальный деструктор для интерфейса.
    ///     Обеспечивает корректную очистку производных реализаций кэша.
    virtual ~ICache() = default;

    /// \~english @brief Pushes a string into the cache and returns its assigned ID.
    ///     If the string already exists in the cache, an existing ID might be returned,
    ///     or a new ID might be generated depending on the cache implementation's policy (e.g., interning).
    /// \~russian @brief Помещает строку в кэш и возвращает присвоенный ей идентификатор.
    ///     Если строка уже существует в кэше, может быть возвращен существующий идентификатор
    ///     или сгенерирован новый, в зависимости от политики реализации кэша (например, интернирование).
    /// \~english @param line The string to be added to the cache.
    /// \~russian @param line Строка для добавления в кэш.
    /// \~english @return A `size_t` ID uniquely identifying the string within the cache.
    /// \~russian @return Идентификатор `size_t`, уникально идентифицирующий строку в кэше.
    virtual size_t push(std::string const &line) = 0;

    /// \~english @brief Retrieves a string from the cache by its ID.
    /// \~russian @brief Извлекает строку из кэша по ее идентификатору.
    /// \~english @param id The ID of the string to retrieve.
    /// \~russian @param id Идентификатор извлекаемой строки.
    /// \~english @return The string associated with the given ID.
    /// \~english @throws std::out_of_range or similar exception if the ID is not found.
    /// \~russian @return Строка, связанная с данным идентификатором.
    /// \~russian @throws std::out_of_range или подобное исключение, если идентификатор не найден.
    virtual std::string get(size_t const &id) const = 0;

    /// \~english @brief Removes a string from the cache by its ID.
    ///     If the ID does not exist, the operation may do nothing or throw an exception,
    ///     depending on the implementation.
    /// \~russian @brief Удаляет строку из кэша по ее идентификатору.
    ///     Если идентификатор не существует, операция может ничего не делать или выбросить исключение,
    ///     в зависимости от реализации.
    /// \~english @param id The ID of the string to remove.
    /// \~russian @param id Идентификатор удаляемой строки.
    virtual void remove(size_t const &id) = 0;

    /// \~english @brief Removes all entries from the cache.
    /// \~russian @brief Удаляет все записи из кэша.
    virtual void clean() = 0;

    /// \~english @brief Gets the current number of items stored in the cache.
    /// \~russian @brief Получает текущее количество элементов, хранящихся в кэше.
    /// \~english @return The number of cached strings.
    /// \~russian @return Количество кэшированных строк.
    virtual size_t size() const = 0;

    /// \~english @brief Checks if a string with the given ID exists in the cache.
    ///     Note: The method name "checkExistense" contains a typo and should likely be "checkExistence".
    /// \~russian @brief Проверяет, существует ли строка с данным идентификатором в кэше.
    ///     Примечание: Имя метода "checkExistense" содержит опечатку и, вероятно, должно быть "checkExistence".
    /// \~english @param id The ID to check for.
    /// \~russian @param id Идентификатор для проверки.
    /// \~english @return `true` if an entry with the specified ID exists, `false` otherwise.
    /// \~russian @return `true`, если запись с указанным идентификатором существует, иначе `false`.
    virtual bool checkExistense(size_t const &id) const = 0;

    /// \~english @brief Flushes any pending changes in the cache to a persistent store, if applicable.
    ///     For caches that are backed by a file or database, this would ensure all in-memory
    ///     modifications are written out. For in-memory-only caches, this might be a no-op.
    /// \~russian @brief Сбрасывает все ожидающие изменения в кэше в постоянное хранилище, если применимо.
    ///     Для кэшей, которые поддерживаются файлом или базой данных, это обеспечит запись
    ///     всех изменений в памяти. Для кэшей, работающих только в памяти, это может быть пустой операцией.
    virtual void flush() = 0;
};


} // fs
} // io
} // iridium


#endif // HEADER_CACHE_F566D6D8_C42D_4049_9642_C600B057A128
