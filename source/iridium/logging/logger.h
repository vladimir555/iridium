// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the main `Logger` singleton class, `LogStream` and `LogStreamDummy` helper structs,
///     and logging macros (LOGT, LOGD, etc.).
///     This is the central header for using the Iridium logging facility.
/// \~russian @file
/// @brief Определяет основной класс-одиночку `Logger`, вспомогательные структуры `LogStream` и `LogStreamDummy`,
///     а также макросы логирования (LOGT, LOGD и т.д.).
///     Это центральный заголовочный файл для использования механизма логирования Iridium.

#ifndef HEADER_LOGGER_68035722_1476_4595_9F55_5338D1576CF9
#define HEADER_LOGGER_68035722_1476_4595_9F55_5338D1576CF9


#include "iridium/pattern/singleton.h"

#include "iridium/threading/synchronized.h"
#include "iridium/platform.h"

#include <mutex>
#include <list>
#include <vector>
#include <deque>
#include <array>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include "event.h"
#include "sink.h"
#include "config.h"


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
/// \~english @brief Contains classes, structures, and enumerations related to the logging framework.
/// \~russian @brief Содержит классы, структуры и перечисления, связанные с фреймворком логирования.
namespace logging {


/// \~english @brief Central logging facility for the application.
///     This class is implemented as a thread-safe singleton and manages a collection of log sinks
///     based on the provided configuration. It is responsible for dispatching log events to
///     the appropriate sinks.
/// \~russian @brief Центральный механизм логирования для приложения.
///     Этот класс реализован как потокобезопасный singleton и управляет коллекцией
///     приемников логов на основе предоставленной конфигурации. Он отвечает за отправку
///     событий журнала соответствующим приемникам.
// todo: wait for finalizing async sinks
class Logger:
    public pattern::Singleton<Logger>,
    public threading::Synchronized<std::mutex>
{
public:
    /// \~english @brief Virtual destructor. Handles cleanup of resources, if any.
    /// \~russian @brief Виртуальный деструктор. Обрабатывает освобождение ресурсов, если таковые имеются.
    virtual ~Logger();
    /// \~english @brief Sets the logger configuration.
    ///     This will reconfigure the logger, potentially clearing existing sinks and
    ///     initializing new ones based on the provided configuration object.
    /// \~russian @brief Устанавливает конфигурацию логгера.
    ///     Это переконфигурирует логгер, потенциально очищая существующие приемники и
    ///     инициализируя новые на основе предоставленного объекта конфигурации.
    /// \~english @param config The logger configuration object.
    /// \~russian @param config Объект конфигурации логгера.
    void setConfig(config::TLogger const &config);
    /// \~english @brief Retrieves the current logger configuration.
    /// \~russian @brief Получает текущую конфигурацию логгера.
    /// \~english @return The current logger configuration object.
    /// \~russian @return Текущий объект конфигурации логгера.
    config::TLogger getConfig();
    /// \~english @brief Logs a pre-formatted event.
    ///     This method is typically called by `LogStream` upon its destruction,
    ///     or can be used to log events created externally.
    /// \~russian @brief Логирует предварительно отформатированное событие.
    ///     Этот метод обычно вызывается `LogStream` при его уничтожении,
    ///     или может использоваться для логирования событий, созданных извне.
    /// \~english @param event A shared pointer to a constant log event.
    /// \~russian @param event Разделяемый указатель на константное событие журнала.
    void log(TEvent::TConstSharedPtr const &event);
    /// \~english @brief Adds a custom sink to the logger.
    ///     This allows for programmatic addition of sinks that are not defined
    ///     in the configuration file.
    /// \~russian @brief Добавляет пользовательский приемник в логгер.
    ///     Это позволяет программно добавлять приемники, которые не определены
    ///     в конфигурационном файле.
    /// \~english @param sink A shared pointer to an object implementing the ISink interface.
    /// \~russian @param sink Разделяемый указатель на объект, реализующий интерфейс ISink.
    void addCustomSink(ISink::TSharedPtr const &sink);

private:
    /// \~english @brief Friend declaration to allow `Singleton` to manage `Logger`'s lifecycle.
    /// \~russian @brief Дружественное объявление, позволяющее `Singleton` управлять жизненным циклом `Logger`.
    friend class pattern::Singleton<Logger>;
    /// \~english @brief Default constructor, private to enforce singleton pattern.
    /// \~russian @brief Конструктор по умолчанию, приватный для реализации шаблона singleton.
    Logger() = default;

    /// \~english @brief A list of shared pointers to `ISink` objects.
    ///     These sinks are responsible for the actual output of log messages (e.g., to console, file).
    /// \~russian @brief Список разделяемых указателей на объекты `ISink`.
    ///     Эти приемники отвечают за фактический вывод сообщений журнала (например, в консоль, файл).
    std::list<ISink::TSharedPtr> m_sinks;

    /// \~english @brief The current logger configuration.
    ///     This object holds settings like log levels for different sinks and formatting options.
    ///     It is a shared pointer to an `iridium::parsing::INode` which represents the parsed configuration data.
    /// \~russian @brief Текущая конфигурация логгера.
    ///     Этот объект хранит настройки, такие как уровни логирования для различных приемников и параметры форматирования.
    ///     Это разделяемый указатель на `iridium::parsing::INode`, представляющий разобранные данные конфигурации.
    parsing::INode::TSharedPtr m_config;
};

/// \~english @brief A no-operation log stream.
///     This struct is used for log levels that are currently disabled.
///     Its `operator<<` does nothing, effectively discarding any data streamed to it.
/// \~russian @brief Потоковый объект-пустышка для логирования.
///     Эта структура используется для уровней логирования, которые в данный момент отключены.
///     Ее `operator<<` ничего не делает, фактически отбрасывая любые данные, направленные в поток.
struct LogStreamDummy {
    /// \~english @brief A no-operation stream insertion operator.
    ///     Discards the value `v`.
    /// \~russian @brief Оператор потокового вывода, не выполняющий никаких действий.
    ///     Отбрасывает значение `v`.
    /// \~english @tparam TValue The type of the value being "logged" (and discarded).
    /// \~russian @tparam TValue Тип "логируемого" (и отбрасываемого) значения.
    /// \~english @param v The value to be discarded.
    /// \~russian @param v Значение, которое будет отброшено.
    /// \~english @return A const reference to the `LogStreamDummy` object itself.
    /// \~russian @return Константная ссылка на сам объект `LogStreamDummy`.
    template<typename TValue>
    LogStreamDummy const & operator << (TValue v) const;
};

/// \~english @brief Represents an active log stream for constructing a log message.
///     An object of this type is typically created by logging macros (e.g., `LOGI`).
///     It accumulates data via `operator<<` and, upon destruction, sends the complete
///     log event to the `Logger` singleton.
/// \~russian @brief Представляет активный поток для конструирования сообщения журнала.
///     Объект этого типа обычно создается макросами логирования (например, `LOGI`).
///     Он накапливает данные через `operator<<` и при уничтожении отправляет
///     завершенное событие журнала в `Logger` singleton.
struct LogStream {
    /// \~english @brief Constructs a `LogStream` for a specific logging level.
    /// \~russian @brief Конструирует `LogStream` для определенного уровня логирования.
    /// \~english @param level The logging level for this message.
    /// \~russian @param level Уровень логирования для этого сообщения.
    explicit LogStream(TEvent::TLevel const &level);
    /// \~english @brief Destructor. Submits the accumulated log event to the `Logger`.
    /// \~russian @brief Деструктор. Отправляет накопленное событие журнала в `Logger`.
    ~LogStream();

    /// \~english @brief Appends a C-style string (const char*) to the log message.
    /// \~russian @brief Добавляет строку в стиле C (const char*) к сообщению журнала.
    /// \~english @param s The C-style string to append. Null strings are logged as "(null)".
    /// \~russian @param s Строка в стиле C для добавления. Нулевые строки логируются как "(null)".
    /// \~english @return A const reference to the `LogStream` object itself, allowing for chained `<<` operations.
    /// \~russian @return Константная ссылка на сам объект `LogStream`, позволяющая выполнять цепочечные операции `<<`.
    LogStream const & operator << (char const * const s) const;

    /// \~english @brief Appends a C-style string (char*) to the log message.
    /// \~russian @brief Добавляет строку в стиле C (char*) к сообщению журнала.
    /// \~english @param s The C-style string to append. Null strings are logged as "(null)".
    /// \~russian @param s Строка в стиле C для добавления. Нулевые строки логируются как "(null)".
    /// \~english @return A const reference to the `LogStream` object itself, allowing for chained `<<` operations.
    /// \~russian @return Константная ссылка на сам объект `LogStream`, позволяющая выполнять цепочечные операции `<<`.
    LogStream const & operator << (char       *       s) const;

    /// \~english @brief Appends a generic value to the log message, converted to a string.
    ///     This is a template operator that handles various data types by attempting to convert them to `std::string` via `iridium::convertion::convert`.
    /// \~russian @brief Добавляет значение общего типа к сообщению журнала, преобразованное в строку.
    ///     Это шаблонный оператор, который обрабатывает различные типы данных, пытаясь преобразовать их в `std::string` через `iridium::convertion::convert`.
    /// \~english @tparam TValue The type of the value.
    /// \~russian @tparam TValue Тип значения.
    /// \~english @param v The value to append.
    /// \~russian @param v Значение для добавления.
    /// \~english @return A const reference to the `LogStream` object itself.
    /// \~russian @return Константная ссылка на сам объект `LogStream`.
    template<typename TValue>
    LogStream const & operator << (TValue const &v) const;
    /// \~english @brief Appends a value pointed to by a pointer-to-pointer to the log message.
    ///     Logs "nullptr" if the outer pointer is null.
    /// \~russian @brief Добавляет значение, на которое указывает указатель на указатель, к сообщению журнала.
    ///     Логирует "nullptr", если внешний указатель нулевой.
    /// \~english @tparam TValue The type of the pointed-to value.
    /// \~russian @tparam TValue Тип значения, на которое указывает указатель.
    /// \~english @param v The pointer-to-pointer.
    /// \~russian @param v Указатель на указатель.
    /// \~english @return A const reference to the `LogStream` object itself.
    /// \~russian @return Константная ссылка на сам объект `LogStream`.
    template<typename TValue>
    LogStream const & operator << (TValue * const * v) const;
    /// \~english @brief Appends an atomic value to the log message, converted to its underlying type then to a string.
    /// \~russian @brief Добавляет атомарное значение к сообщению журнала, преобразованное к его базовому типу, а затем в строку.
    /// \~english @tparam TValue The underlying type of the atomic value.
    /// \~russian @tparam TValue Базовый тип атомарного значения.
    /// \~english @param v The atomic value to append.
    /// \~russian @param v Атомарное значение для добавления.
    /// \~english @return A const reference to the `LogStream` object itself.
    /// \~russian @return Константная ссылка на сам объект `LogStream`.
    template<typename TValue>
    LogStream const & operator << (std::atomic<TValue> const &v) const;
    /// \~english @brief Appends the value pointed to by a shared_ptr to the log message.
    ///     Logs "nullptr" if the shared_ptr is null.
    /// \~russian @brief Добавляет значение, на которое указывает shared_ptr, к сообщению журнала.
    ///     Логирует "nullptr", если shared_ptr нулевой.
    /// \~english @tparam TValue The type of the pointed-to value.
    /// \~russian @tparam TValue Тип значения, на которое указывает указатель.
    /// \~english @param v The shared_ptr.
    /// \~russian @param v Разделяемый указатель (shared_ptr).
    /// \~english @return A const reference to the `LogStream` object itself.
    /// \~russian @return Константная ссылка на сам объект `LogStream`.
    template<typename TValue>
    LogStream const & operator << (std::shared_ptr<TValue> const &v) const;
    /// \~english @brief Appends the content of a `std::list` to the log message, formatted as "[ item1, item2, ... ]".
    /// \~russian @brief Добавляет содержимое `std::list` к сообщению журнала, отформатированное как "[ item1, item2, ... ]".
    /// \~english @tparam TValue The type of elements in the list.
    /// \~russian @tparam TValue Тип элементов в списке.
    /// \~english @param values The list to append.
    /// \~russian @param values Список для добавления.
    /// \~english @return A const reference to the `LogStream` object itself.
    /// \~russian @return Константная ссылка на сам объект `LogStream`.
    template<typename TValue>
    LogStream const & operator << (std::list<TValue> const &values) const;
    /// \~english @brief Appends the content of a `std::vector` to the log message, formatted as "[ item1, item2, ... ]".
    /// \~russian @brief Добавляет содержимое `std::vector` к сообщению журнала, отформатированное как "[ item1, item2, ... ]".
    /// \~english @tparam TValue The type of elements in the vector.
    /// \~russian @tparam TValue Тип элементов в векторе.
    /// \~english @param values The vector to append.
    /// \~russian @param values Вектор для добавления.
    /// \~english @return A const reference to the `LogStream` object itself.
    /// \~russian @return Константная ссылка на сам объект `LogStream`.
    template<typename TValue>
    LogStream const & operator << (std::vector<TValue> const &values) const;
    /// \~english @brief Appends the content of a `std::deque` to the log message, formatted as "[ item1, item2, ... ]".
    /// \~russian @brief Добавляет содержимое `std::deque` к сообщению журнала, отформатированное как "[ item1, item2, ... ]".
    /// \~english @tparam TValue The type of elements in the deque.
    /// \~russian @tparam TValue Тип элементов в двусторонней очереди.
    /// \~english @param values The deque to append.
    /// \~russian @param values Двусторонняя очередь для добавления.
    /// \~english @return A const reference to the `LogStream` object itself.
    /// \~russian @return Константная ссылка на сам объект `LogStream`.
    template<typename TValue>
    LogStream const & operator << (std::deque<TValue> const &values) const;
    /// \~english @brief Appends the content of a `std::array` to the log message, formatted as "[ item1, item2, ... ]".
    /// \~russian @brief Добавляет содержимое `std::array` к сообщению журнала, отформатированное как "[ item1, item2, ... ]".
    /// \~english @tparam TValue The type of elements in the array.
    /// \~russian @tparam TValue Тип элементов в массиве.
    /// \~english @tparam size The size of the array.
    /// \~russian @tparam size Размер массива.
    /// \~english @param values The array to append.
    /// \~russian @param values Массив для добавления.
    /// \~english @return A const reference to the `LogStream` object itself.
    /// \~russian @return Константная ссылка на сам объект `LogStream`.
    template<typename TValue, std::size_t size>
    LogStream const & operator << (std::array<TValue, size> const &values) const;
    /// \~english @brief Appends the content of a `std::set` to the log message, formatted as "[ item1, item2, ... ]".
    /// \~russian @brief Добавляет содержимое `std::set` к сообщению журнала, отформатированное как "[ item1, item2, ... ]".
    /// \~english @tparam TValue The type of elements in the set.
    /// \~russian @tparam TValue Тип элементов в множестве.
    /// \~english @param values The set to append.
    /// \~russian @param values Множество для добавления.
    /// \~english @return A const reference to the `LogStream` object itself.
    /// \~russian @return Константная ссылка на сам объект `LogStream`.
    template<typename TValue>
    LogStream const & operator << (std::set<TValue> const &values) const;


//    template<typename TContainer>
//    typename std::enable_if<
//        std::is_same_v<TContainer, std::vector          <typename TContainer::value_type>> ||
//        std::is_same_v<TContainer, std::list            <typename TContainer::value_type>> ||
//        std::is_same_v<TContainer, std::set             <typename TContainer::value_type>> ||
//        std::is_same_v<TContainer, std::unordered_set   <typename TContainer::value_type>>,
//        LogStream const &
//    >::type operator << (TContainer const &values) const {
//        *this << "[ ";
//        bool is_first = true;
//        for (auto const &value: values)
//            *this << (is_first ? "" : ", ") << value, is_first = false;
//        *this << "]";
//        return *this;
//    }


    /// \~english @brief Appends the content of a `std::map` to the log message, formatted as "[ key1: value1, key2: value2, ... ]".
    /// \~russian @brief Добавляет содержимое `std::map` к сообщению журнала, отформатированное как "[ ключ1: значение1, ключ2: значение2, ... ]".
    /// \~english @tparam TKey The type of keys in the map.
    /// \~russian @tparam TKey Тип ключей в отображении.
    /// \~english @tparam TValue The type of values in the map.
    /// \~russian @tparam TValue Тип значений в отображении.
    /// \~english @param values The map to append.
    /// \~russian @param values Отображение для добавления.
    /// \~english @return A const reference to the `LogStream` object itself.
    /// \~russian @return Константная ссылка на сам объект `LogStream`.
    template<typename TKey, typename TValue>
    LogStream const & operator << (std::map<TKey, TValue> const &values) const;
    /// \~english @brief Appends the content of a `std::unordered_set` to the log message, formatted as "[ item1, item2, ... ]".
    /// \~russian @brief Добавляет содержимое `std::unordered_set` к сообщению журнала, отформатированное как "[ item1, item2, ... ]".
    /// \~english @tparam TValue The type of elements in the unordered_set.
    /// \~russian @tparam TValue Тип элементов в неупорядоченном множестве.
    /// \~english @param values The unordered_set to append.
    /// \~russian @param values Неупорядоченное множество для добавления.
    /// \~english @return A const reference to the `LogStream` object itself.
    /// \~russian @return Константная ссылка на сам объект `LogStream`.
    template<typename TValue>
    LogStream const & operator << (std::unordered_set<TValue> const &values) const;
    /// \~english @brief Appends the content of a `std::unordered_map` to the log message, formatted as "[ key1: value1, key2: value2, ... ]".
    /// \~russian @brief Добавляет содержимое `std::unordered_map` к сообщению журнала, отформатированное как "[ ключ1: значение1, ключ2: значение2, ... ]".
    /// \~english @tparam TKey The type of keys in the unordered_map.
    /// \~russian @tparam TKey Тип ключей в неупорядоченном отображении.
    /// \~english @tparam TValue The type of values in the unordered_map.
    /// \~russian @tparam TValue Тип значений в неупорядоченном отображении.
    /// \~english @param values The unordered_map to append.
    /// \~russian @param values Неупорядоченное отображение для добавления.
    /// \~english @return A const reference to the `LogStream` object itself.
    /// \~russian @return Константная ссылка на сам объект `LogStream`.
    template<typename TKey, typename TValue>
    LogStream const & operator << (std::unordered_map<TKey, TValue> const &values) const;

private:
    /// \~english @brief Mutable shared pointer to the log event being constructed.
    ///     `mutable` allows modification of the pointed-to `TEvent` object (e.g., its `line` member)
    ///     within const member functions of `LogStream`. This allows `operator<<` to be const while still modifying the event's content.
    /// \~russian @brief Изменяемый разделяемый указатель на конструируемое событие журнала.
    ///     `mutable` позволяет изменять объект `TEvent`, на который указывает указатель (например, его член `line`),
    ///     внутри константных функций-членов `LogStream`. Это позволяет `operator<<` быть константным, при этом изменяя содержимое события.
    mutable TEvent::TSharedPtr m_event;
};


template<typename TValue>
LogStreamDummy const &LogStreamDummy::operator << (TValue) const {
    return *this;
}


template<typename TValue>
LogStream const &LogStream::operator << (TValue const &v) const {
    m_event->line += convertion::convert<std::string>(v);
    return std::move(*this); // ----->
}


template<typename TValue>
LogStream const &LogStream::operator << (std::atomic<TValue> const &v) const {
    m_event->line += convertion::convert<std::string>(static_cast<TValue>(v));
    return std::move(*this); // ----->
}


template<typename TValue>
LogStream const &LogStream::operator << (TValue * const * v) const {
    m_event->line += v ? convertion::convert<std::string>(*v) : "nullptr";
    return std::move(*this); // ----->
}


template<typename TValue>
LogStream const &LogStream::operator << (std::shared_ptr<TValue> const &v) const {
    m_event->line += v ? convertion::convert<std::string>(*v) : "nullptr";
    return std::move(*this); // ----->
}


template<typename TValue>
LogStream const & LogStream::operator << (std::list<TValue> const &values) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &value: values)
        *this << (is_first ? "" : ", ") << value, is_first = false;
    *this << " ]";
    return *this;
}


template<typename TValue>
LogStream const &LogStream::operator << (std::vector<TValue> const &values) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &value: values)
        *this << (is_first ? "" : ", ") << value, is_first = false;
    *this << " ]";
    return *this;
}


template<typename TValue>
LogStream const &LogStream::operator << (std::deque<TValue> const &values) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &value: values)
        *this << (is_first ? "" : ", ") << value, is_first = false;
    *this << " ]";
    return *this;
}


template<typename TValue, std::size_t size>
LogStream const &LogStream::operator << (std::array<TValue, size> const &values) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &value: values)
        *this << (is_first ? "" : ", ") << value, is_first = false;
    *this << " ]";
    return *this;
}


template<typename TValue>
LogStream const &LogStream::operator << (std::set<TValue> const &values) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &value: values)
        *this << (is_first ? "" : ", ") << value, is_first = false;
    *this << " ]";
    return *this;
}


template<typename TValue>
LogStream const &LogStream::operator << (std::unordered_set<TValue> const &values) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &value: values)
        *this << (is_first ? "" : ", ") << value, is_first = false;
    *this << " ]";
    return *this;
}


template<typename TKey, typename TValue>
LogStream const &LogStream::operator << (std::map<TKey, TValue> const &m) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &item: m)
        *this << (is_first ? "" : ", ") << item.first << ": " << item.second << " ", is_first = false;;
    *this << " ]";
    return *this;
}


template<typename TKey, typename TValue>
LogStream const &LogStream::operator << (std::unordered_map<TKey, TValue> const &m) const {
    *this << "[ ";
    bool is_first = true;
    for (auto const &item: m)
        *this << (is_first ? "" : ", ") << item.first << ": " << item.second << " ", is_first = false;;
    *this << " ]";
    return *this;
}

/// \~english @brief Sets the global logger configuration. This is a free function wrapper around Logger::instance().setConfig().
/// \~russian @brief Устанавливает глобальную конфигурацию логгера. Это свободная функция-обертка вокруг Logger::instance().setConfig().
/// \~english @param config The logger configuration object.
/// \~russian @param config Объект конфигурации логгера.
void setConfig(config::TLogger const &config);

/// \~english @brief Converts a C++ function signature (e.g., from `__PRETTY_FUNCTION__`) into a more concise function name for logging.
/// \~russian @brief Преобразует сигнатуру функции C++ (например, из `__PRETTY_FUNCTION__`) в более краткое имя функции для логирования.
/// \~english @param name The full function signature string.
/// \~russian @param name Полная строка сигнатуры функции.
/// \~english @return A string containing the extracted and simplified function name.
/// \~russian @return Строка, содержащая извлеченное и упрощенное имя функции.
std::string convertFunctionNameToLogFunctionName(std::string const &name);

/// \~english @brief Extracts the file name from a full path for concise logging.
/// \~russian @brief Извлекает имя файла из полного пути для краткого логирования.
/// \~english @param path The full file path.
/// \~russian @param path Полный путь к файлу.
/// \~english @return The file name part of the path.
/// \~russian @return Часть пути, представляющая имя файла.
std::string extractFileNameToLog(std::string const &path);


} // logging
} // iridium

/// \~english @def __PRETTY_FUNCTION__
/// @brief A macro that expands to a string literal containing the qualified name of the current function.
///     On MSVC, `__FUNCSIG__` provides similar functionality and is used here as a fallback if `__PRETTY_FUNCTION__` is not available.
/// \~russian @def __PRETTY_FUNCTION__
/// @brief Макрос, который раскрывается в строковый литерал, содержащий полное имя текущей функции.
///     В MSVC `__FUNCSIG__` предоставляет аналогичную функциональность и используется здесь в качестве запасного варианта, если `__PRETTY_FUNCTION__` недоступен.
#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif // _MSC_VER

/// \~english @def LOGT
/// @brief Logging macro for TRACE level messages.
///     Prepends file name and line number. Disabled if `BUILD_TYPE_DEBUG` or `BUILD_FLAG_FORCE_DEBUG_LOG` is not defined.
///     Usage: `LOGT << "Trace message: " << value;`
/// \~russian @def LOGT
/// @brief Макрос логирования для сообщений уровня TRACE.
///     Добавляет имя файла и номер строки. Отключен, если не определены `BUILD_TYPE_DEBUG` или `BUILD_FLAG_FORCE_DEBUG_LOG`.
///     Использование: `LOGT << "Сообщение трассировки: " << value;`
// macros: __func__
#if defined(BUILD_TYPE_DEBUG) || defined(BUILD_FLAG_FORCE_DEBUG_LOG)
#define LOGT \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::TRACE) << \
iridium::logging::extractFileNameToLog(std::string(__FILE__) + ":" + std::to_string(__LINE__)) << " "
#else
#define LOGT \
iridium::logging::LogStreamDummy()
#endif // BUILD_TYPE_DEBUG
/// \~english @def LOGD
/// @brief Logging macro for DEBUG level messages.
///     Disabled if `BUILD_TYPE_DEBUG` or `BUILD_FLAG_FORCE_DEBUG_LOG` is not defined.
///     Usage: `LOGD << "Debug message: " << value;`
/// \~russian @def LOGD
/// @brief Макрос логирования для сообщений уровня DEBUG.
///     Отключен, если не определены `BUILD_TYPE_DEBUG` или `BUILD_FLAG_FORCE_DEBUG_LOG`.
///     Использование: `LOGD << "Отладочное сообщение: " << value;`
#if defined(BUILD_TYPE_DEBUG) || defined(BUILD_FLAG_FORCE_DEBUG_LOG)
#define LOGD \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::DEBUG)
#else
#define LOGD \
iridium::logging::LogStreamDummy()
#endif // BUILD_TYPE_DEBUG
/// \~english @def LOGI
/// @brief Logging macro for INFO level messages.
///     Usage: `LOGI << "Informational message: " << value;`
/// \~russian @def LOGI
/// @brief Макрос логирования для сообщений уровня INFO.
///     Использование: `LOGI << "Информационное сообщение: " << value;`
#define LOGI \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::INFO)
/// \~english @def LOGW
/// @brief Logging macro for WARNING level messages.
///     Usage: `LOGW << "Warning message: " << value;`
/// \~russian @def LOGW
/// @brief Макрос логирования для сообщений уровня WARNING.
///     Использование: `LOGW << "Предупреждающее сообщение: " << value;`
#define LOGW \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::WARNING)
/// \~english @def LOGE
/// @brief Logging macro for ERROR level messages.
///     Usage: `LOGE << "Error message: " << value;`
/// \~russian @def LOGE
/// @brief Макрос логирования для сообщений уровня ERROR.
///     Использование: `LOGE << "Сообщение об ошибке: " << value;`
#define LOGE \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::ERROR_)
/// \~english @def LOGF
/// @brief Logging macro for FATAL level messages.
///     Usage: `LOGF << "Fatal error message: " << value;`
/// \~russian @def LOGF
/// @brief Макрос логирования для сообщений уровня FATAL.
///     Использование: `LOGF << "Сообщение о фатальной ошибке: " << value;`
#define LOGF \
iridium::logging::LogStream(iridium::logging::TEvent::TLevel::FATAL)


#endif // HEADER_LOGGER_68035722_1476_4595_9F55_5338D1576CF9
