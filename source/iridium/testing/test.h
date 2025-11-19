// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_TEST_2AF68F5B_6537_45C6_876E_EC1DB2BBAA67
#define HEADER_TEST_2AF68F5B_6537_45C6_876E_EC1DB2BBAA67


#include <string>
#include "iridium/smart_ptr.h"


namespace iridium::testing {

/// \~english @brief Interface for a runnable test case.
///     Concrete test classes should implement this interface to define a specific test.
///     The `run()` method encapsulates the test logic, including any assertions.
///     Test runners can then execute collections of `ITest` instances.
/// \~russian @brief Интерфейс для запускаемого тестового случая.
///     Конкретные тестовые классы должны реализовывать этот интерфейс для определения конкретного теста.
///     Метод `run()` инкапсулирует логику теста, включая любые утверждения (assertions).
///     Средства запуска тестов затем могут выполнять коллекции экземпляров `ITest`.
class ITest {
public:
    /// \~english @brief Defines common interface elements for `ITest`, such as smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.).
    /// \~russian @brief Определяет общие элементы интерфейса для `ITest`, такие как typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.).
    DEFINE_INTERFACE(ITest)
    /// \~english @brief Runs the test case.
    ///     Implementations of this method should contain the actual test logic and assertions.
    ///     Any exception thrown during the execution of `run()` is typically caught by a test runner
    ///     and interpreted as a test failure.
    /// \~russian @brief Запускает тестовый случай.
    ///     Реализации этого метода должны содержать фактическую логику теста и утверждения.
    ///     Любое исключение, выброшенное во время выполнения `run()`, обычно перехватывается
    ///     средством запуска тестов и интерпретируется как провал теста.
    virtual void run() = 0;
};


} // namespace iridium::testing


#endif // HEADER_TEST_2AF68F5B_6537_45C6_876E_EC1DB2BBAA67
