// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines the `CFileCache` class, a file-backed implementation of the `ICache` interface.
/// \~russian @brief Определяет класс `CFileCache`, файловую реализацию интерфейса `ICache`.
#ifndef HEADER_FILE_CACHE_8ABCB65B_D74A_48BA_80C2_E6B63B28786F
#define HEADER_FILE_CACHE_8ABCB65B_D74A_48BA_80C2_E6B63B28786F


#include <string>
#include <vector>
#include <memory> // For std::shared_ptr

#include "iridium/smart_ptr.h"           // For DEFINE_CREATE, TSharedPtr
#include "iridium/io/fs/cache.h"         // For ICache interface
#include "iridium/io/fs/file_stream.h"   // For IFileStreamWriter


namespace iridium {
namespace io {
namespace fs {
/// \~english @brief Namespace for internal implementation details of file system functionalities.
/// \~russian @brief Пространство имен для внутренних деталей реализации функциональностей файловой системы.
namespace implementation {


/// \~english @brief A file-backed cache implementation for strings.
///     This class implements the `ICache` interface, providing a mechanism to store strings
///     in memory with the ability to load from and persist (flush) to a specified file.
///     Strings are typically accessed by an ID, which in this implementation corresponds
///     to their index in an internal vector.
/// \~russian @brief Реализация кэша строк с файловым хранилищем.
///     Этот класс реализует интерфейс `ICache`, предоставляя механизм для хранения строк
///     в памяти с возможностью загрузки из файла и сохранения (сброса) в указанный файл.
///     Доступ к строкам обычно осуществляется по идентификатору, который в этой реализации соответствует
///     их индексу во внутреннем векторе.
class CFileCache : public ICache {
public:
    /// \~english @brief Defines smart pointer typedefs (e.g., `CFileCache::TSharedPtr`) for this class.
    /// \~russian @brief Определяет typedef'ы умных указателей (например, `CFileCache::TSharedPtr`) для этого класса.
    DEFINE_CREATE(CFileCache)

    /// \~english @brief Constructs a `CFileCache` associated with a specific file.
    ///     The cache content can be loaded from this file during `initialize()` and saved back during `flush()` or `finalize()`.
    /// \~russian @brief Конструирует `CFileCache`, связанный с определенным файлом.
    ///     Содержимое кэша может быть загружено из этого файла во время `initialize()` и сохранено обратно во время `flush()` или `finalize()`.
    /// \~english @param file_name The path to the file that will serve as the backing store for this cache.
    /// \~russian @param file_name Путь к файлу, который будет служить постоянным хранилищем для этого кэша.
    explicit CFileCache(std::string const &file_name);

    /// \~english @brief Default virtual destructor.
    /// \~russian @brief Виртуальный деструктор по умолчанию.
    virtual ~CFileCache() = default;

    /// \~english @brief Initializes the cache, potentially loading its content from the associated file.
    ///     This method is called as part of the `IInitializable` interface.
    /// \~russian @brief Инициализирует кэш, потенциально загружая его содержимое из связанного файла.
    ///     Этот метод вызывается как часть интерфейса `IInitializable`.
    /// \~english @throws iridium::Exception or derived classes on errors during file reading or parsing.
    /// \~russian @throws iridium::Exception или производные классы при ошибках во время чтения или разбора файла.
    virtual void initialize() override;

    /// \~english @brief Finalizes the cache, potentially flushing any unsaved changes to the associated file.
    ///     This method is called as part of the `IInitializable` interface (typically on shutdown).
    /// \~russian @brief Финализирует кэш, потенциально сбрасывая все несохраненные изменения в связанный файл.
    ///     Этот метод вызывается как часть интерфейса `IInitializable` (обычно при завершении работы).
    /// \~english @throws iridium::Exception or derived classes on errors during file writing.
    /// \~russian @throws iridium::Exception или производные классы при ошибках во время записи файла.
    virtual void finalize() override;

    /// \~english @brief Adds a string to the cache and returns its assigned ID (index).
    ///     The string is stored internally, and subsequent calls to `flush()` will persist it to the file.
    /// \~russian @brief Добавляет строку в кэш и возвращает присвоенный ей идентификатор (индекс).
    ///     Строка сохраняется внутри, и последующие вызовы `flush()` сохранят ее в файл.
    /// \~english @param line The string to be added to the cache.
    /// \~russian @param line Строка для добавления в кэш.
    /// \~english @return The ID (index in the internal vector) of the newly added string.
    /// \~russian @return Идентификатор (индекс во внутреннем векторе) только что добавленной строки.
    virtual size_t push(std::string const &line) override;

    /// \~english @brief Retrieves a string from the cache by its ID (index).
    /// \~russian @brief Извлекает строку из кэша по ее идентификатору (индексу).
    /// \~english @param id The ID (index) of the string to retrieve.
    /// \~russian @param id Идентификатор (индекс) извлекаемой строки.
    /// \~english @return The string associated with the given ID.
    /// \~russian @return Строка, связанная с данным идентификатором.
    /// \~english @throws std::out_of_range if the ID is invalid (e.g., out of bounds for the internal vector).
    /// \~russian @throws std::out_of_range, если идентификатор недействителен (например, выходит за пределы внутреннего вектора).
    virtual std::string get(size_t const &id) const override;

    /// \~english @brief Removes a string from the cache by its ID (index).
    ///     Note: This operation can be inefficient for a vector-based cache if it involves shifting elements
    ///     or if IDs are expected to remain stable after removals. Implementations might mark entries as invalid
    ///     instead of actually removing them to preserve ID stability.
    /// \~russian @brief Удаляет строку из кэша по ее идентификатору (индексу).
    ///     Примечание: Эта операция может быть неэффективной для кэша на основе вектора, если она включает сдвиг элементов
    ///     или если ожидается, что идентификаторы останутся стабильными после удалений. Реализации могут помечать записи как недействительные
    ///     вместо фактического удаления для сохранения стабильности идентификаторов.
    /// \~english @param id The ID (index) of the string to remove.
    /// \~russian @param id Идентификатор (индекс) удаляемой строки.
    /// \~english @throws std::out_of_range if the ID is invalid.
    /// \~russian @throws std::out_of_range, если идентификатор недействителен.
    virtual void remove(size_t const &id) override;

    /// \~english @brief Removes all entries from the in-memory cache and potentially from the backing file if `flush` is implied.
    /// \~russian @brief Удаляет все записи из кэша в памяти и, возможно, из файла хранения, если подразумевается `flush`.
    virtual void clean() override;

    /// \~english @brief Gets the current number of items stored in the cache.
    /// \~russian @brief Получает текущее количество элементов, хранящихся в кэше.
    /// \~english @return The number of cached strings.
    /// \~russian @return Количество кэшированных строк.
    virtual size_t size() const override;

    /// \~english @brief Checks if a string with the given ID (index) exists and is valid in the cache.
    ///     Note: The method name "checkExistense" contains a typo and should likely be "checkExistence".
    /// \~russian @brief Проверяет, существует ли строка с данным идентификатором (индексом) и действительна ли она в кэше.
    ///     Примечание: Имя метода "checkExistense" содержит опечатку и, вероятно, должно быть "checkExistence".
    /// \~english @param id The ID (index) to check for.
    /// \~russian @param id Идентификатор (индекс) для проверки.
    /// \~english @return `true` if an entry with the specified ID exists and is considered valid, `false` otherwise.
    /// \~russian @return `true`, если запись с указанным идентификатором существует и считается действительной, иначе `false`.
    virtual bool checkExistense(size_t const &id) const override;

    /// \~english @brief Flushes the current in-memory cache content to its associated backing file.
    ///     All strings currently in the cache are written to the file specified during construction.
    /// \~russian @brief Сбрасывает текущее содержимое кэша в памяти в связанный с ним файл хранения.
    ///     Все строки, находящиеся в данный момент в кэше, записываются в файл, указанный при конструировании.
    /// \~english @throws iridium::Exception or derived classes on errors during file writing.
    /// \~russian @throws iridium::Exception или производные классы при ошибках во время записи файла.
    virtual void flush() override;

private:
    /// \~english @brief The filename (including path) of the file used as a backing store for the cache.
    /// \~russian @brief Имя файла (включая путь), используемого в качестве постоянного хранилища для кэша.
    std::string                     m_file_name;

    /// \~english @brief A shared pointer to a file stream writer, used for writing cache contents to the backing file.
    ///     This might be initialized in `initialize()` or on first use of `flush()`.
    /// \~russian @brief Умный указатель на писатель файлового потока, используемый для записи содержимого кэша в файл хранения.
    ///     Может инициализироваться в `initialize()` или при первом использовании `flush()`.
    IFileStreamWriter::TSharedPtr   m_file_writer;

    /// \~english @brief The in-memory store for the cached strings.
    ///     Each element is a shared pointer to a string, which might help in managing memory for identical strings
    ///     or if strings are passed around. The index in this vector serves as the ID for cache operations.
    /// \~russian @brief Хранилище в памяти для кэшированных строк.
    ///     Каждый элемент является умным указателем на строку, что может помочь в управлении памятью для идентичных строк
    ///     или если строки передаются по коду. Индекс в этом векторе служит идентификатором для операций кэша.
    std::vector< std::shared_ptr<std::string> > m_lines;
};


} // implementation
} // fs
} // io
} // iridium


#endif // HEADER_FILE_CACHE_8ABCB65B_D74A_48BA_80C2_E6B63B28786F
