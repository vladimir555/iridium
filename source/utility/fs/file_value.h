#ifndef HEADER_FILE_VALUE_46471A9C_47FB_468C_9FC2_F8CB96FB573C
#define HEADER_FILE_VALUE_46471A9C_47FB_468C_9FC2_F8CB96FB573C


#include <string>
#include <fstream>

#include "text_writer.h"
#include "implementation/fast_text_writer.h"
#include "utility/convertion/convert.h"


namespace utility {
namespace fs {


// ----- interface


template<typename TValue>
class FileValue {
public:
    FileValue(std::string const &file_name, TValue const &default_value);
    ///
    ~FileValue() = default;
    ///
    void operator = (TValue const &value);
    ///
    operator TValue() const;
private:
    ///
    std::string m_file_name;
    ///
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
        if (f) {
            std::string value;
            f >> value;
            m_value = convertion::convert<TValue>(value);
        } else {
            m_value = default_value;
        }
    } catch (std::exception const &e) {
        throw std::runtime_error("read file '" + m_file_name + "' error: " + e.what()); // ----->
    }
}


template<typename TValue>
void FileValue<TValue>::operator = (TValue const &value) {
    try {
        m_value = value;

        std::ofstream f(m_file_name);
        f << convertion::convert<std::string>(value);
    } catch (std::exception const &e) {
        throw std::runtime_error("write file '" + m_file_name + "' error: " + e.what()); // ----->
    }
}


template<typename TValue>
FileValue<TValue>::operator TValue () const {
    return m_value; // ----->
}


} // fs
} // utility


#endif // HEADER_FILE_VALUE_46471A9C_47FB_468C_9FC2_F8CB96FB573C
