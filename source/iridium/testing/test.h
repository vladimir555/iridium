// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_TEST_2AF68F5B_6537_45C6_876E_EC1DB2BBAA67
#define HEADER_TEST_2AF68F5B_6537_45C6_876E_EC1DB2BBAA67


#include <string>
#include "iridium/parsing/node_type.h"


namespace iridium::testing {


/// \~english @brief Interface for a unit test case.
/// \~russian @brief Интерфейс для случая модульного теста.
class IUnitTestCase {
public:
    DEFINE_INTERFACE(IUnitTestCase)
    /// \~english @brief Runs the test case.
    /// \~russian @brief Запускает тестовый случай.
    virtual void run() = 0;
    /// \~english @brief Gets the line number where the test is defined.
    /// \~russian @brief Возвращает номер строки, на которой определен тест.
    virtual size_t getLine() const = 0;
};


} // namespace iridium::testing


#endif // HEADER_TEST_2AF68F5B_6537_45C6_876E_EC1DB2BBAA67
