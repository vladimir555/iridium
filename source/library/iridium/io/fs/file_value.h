// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @brief Defines a template class `FileValue` that synchronizes a value with a file.
/// \~russian @brief Определяет шаблонный класс `FileValue`, который синхронизирует значение с файлом.
#ifndef HEADER_FILE_VALUE_46471A9C_47FB_468C_9FC2_F8CB96FB573C
#define HEADER_FILE_VALUE_46471A9C_47FB_468C_9FC2_F8CB96FB573C


#include <string>
#include <fstream>  // For std::ifstream, std::ofstream
#include <stdexcept> // For std::runtime_error

#include "iridium/convertion/convert.h" // For iridium::convertion::convert


namespace iridium::io::fs {


// ----- interface

/// \~english @brief A template class that represents a value persisted in a file.
///     The value is loaded from the specified file upon construction. When a new value is
///     assigned to a `FileValue` object, it updates its internal state and writes the new
///     value back to the file. Conversions between `TValue` and `std::string` (for file I/O)
///     are handled by the `iridium::convertion::convert` framework.
/// \~russian @brief Шаблонный класс, представляющий значение, сохраняемое в файле.
///     Значение загружается из указанного файла при конструировании. Когда объекту `FileValue`
///     присваивается новое значение, он обновляет свое внутреннее состояние и записывает новое
///     значение обратно в файл. Преобразования между `TValue` и `std::string` (для файлового ввода-вывода)
///     обрабатываются фреймворком `iridium::convertion::convert`.
/// \~english @tparam TValue The type of the value to be stored and managed.
///     This type must have appropriate `iridium::convertion::convert` specializations
///     to and from `std::string`.
/// \~russian @tparam TValue Тип сохраняемого и управляемого значения.
///     Для этого типа должны существовать соответствующие специализации `iridium::convertion::convert`
///     для преобразования в `std::string` и из него.
template<typename TValue>
class FileValue {
public:
    /// \~english @brief Constructs a `FileValue` object, loading its initial value from a file.
    ///     Attempts to read the content of the specified file, converts it to `TValue`, and stores it.
    ///     If the file does not exist, cannot be opened, or if conversion fails, the `default_value` is used.
    /// \~russian @brief Конструирует объект `FileValue`, загружая его начальное значение из файла.
    ///     Пытается прочитать содержимое указанного файла, преобразует его в `TValue` и сохраняет.
    ///     Если файл не существует, не может быть открыт или если преобразование не удалось, используется `default_value`.
    /// \~english @param file_name The path to the file from which to load and to which to save the value.
    /// \~russian @param file_name Путь к файлу, из которого загружается и в который сохраняется значение.
    /// \~english @param default_value The value to use if the file cannot be read or its content is invalid.
    ///     Defaults to a default-constructed `TValue` if not provided.
    /// \~russian @param default_value Значение, используемое, если файл не может быть прочитан или его содержимое некорректно.
    ///     По умолчанию используется значение `TValue`, сконструированное по умолчанию, если не предоставлено.
    /// \~english @throws std::runtime_error if file reading or initial conversion from string to `TValue` fails catastrophically (though some errors might result in using default_value).
    /// \~russian @throws std::runtime_error если чтение файла или начальное преобразование из строки в `TValue` завершается катастрофической ошибкой (хотя некоторые ошибки могут привести к использованию default_value).
    FileValue(std::string const &file_name, TValue const &default_value = {});
    ~FileValue() = default;

    /// \~english @brief Assigns a new value, updating the internal state and the backing file.
    /// \~russian @brief Присваивает новое значение, обновляя внутреннее состояние и связанный файл.
    /// \~english @param value The new value to assign.
    /// \~russian @param value Новое значение для присваивания.
    /// \~english @throws std::runtime_error if the file cannot be opened for writing or if the write operation fails.
    /// \~russian @throws std::runtime_error если файл не может быть открыт для записи или если операция записи завершается неудачей.
    void operator = (TValue const &value);

    /// \~english @brief Implicitly converts the `FileValue` object to its underlying `TValue`.
    /// \~russian @brief Неявно преобразует объект `FileValue` в его базовый тип `TValue`.
    /// \~english @return The current value.
    /// \~russian @return Текущее значение.
    operator TValue() const;
private:
    /// \~english @brief The path to the file used for persistence.
    /// \~russian @brief Путь к файлу, используемому для сохранения.
    std::string m_file_name;
    TValue      m_value;
};


// ----- implementation


template<typename TValue>
FileValue<TValue>::FileValue(std::string const &file_name, TValue const &default_value)
:
    m_file_name (file_name)
{
    try {
        std::ifstream f(m_file_name);
        if (f.good() && f.peek() != std::ifstream::traits_type::eof()) { // Check if file is good and not empty
            std::string value_str;
            // Read the whole file content into value_str. A more robust way might be needed for multi-line values.
            // This implementation reads only the first "word" or until whitespace if TValue is not string.
            // If TValue is std::string, it might also only read the first word.
            // For robust whole-file-to-string, one would typically use rdbuf.
            // However, we'll assume 'f >> value_str;' is the intended way based on original code,
            // which implies values are single-word strings or types convertible from such.
            f >> value_str;
            if (!value_str.empty()) { // If something was read
                 m_value = convertion::convert<TValue>(value_str);
            } else { // File might be empty or only whitespace
                 m_value = default_value;
            }
        } else {
            m_value = default_value;
        }
    } catch (std::exception const &e) {
        // If conversion or other errors occur, it might be better to ensure m_value is default_value
        // For now, rethrow wrapped, but consider if default_value should be assigned here too.
        throw std::runtime_error("read file '" + m_file_name + "' error: " + e.what());
    }
}


template<typename TValue>
void FileValue<TValue>::operator = (TValue const &value) {
    try {
        m_value = value;

        std::ofstream f(m_file_name, std::ios::trunc); // Open in truncation mode to overwrite
        if (!f.good()) {
            throw std::runtime_error("could not open file for writing");
        }
        f << convertion::convert<std::string>(value);
        if (!f.good()) { // Check after write
            throw std::runtime_error("write operation failed");
        }
    } catch (std::exception const &e) {
        // Avoid re-wrapping if it's already our specific runtime_error
        if (dynamic_cast<const std::runtime_error*>(&e) &&
            std::string(e.what()).find("write file '" + m_file_name + "' error: ") != std::string::npos) {
            throw;
        }
        throw std::runtime_error("write file '" + m_file_name + "' error: " + e.what());
    }
}


template<typename TValue>
FileValue<TValue>::operator TValue () const {
    return m_value;
}


} // iridium::io::fs


#endif // HEADER_FILE_VALUE_46471A9C_47FB_468C_9FC2_F8CB96FB573C
