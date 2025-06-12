// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_TEST_RUNNER_34E956BA_A097_4535_8EF2_FC8842A198B7
#define HEADER_TEST_RUNNER_34E956BA_A097_4535_8EF2_FC8842A198B7


#include "iridium/parsing/node_type.h"
#include "test.h"

#include "iridium/parsing/serialization/node.h"


namespace iridium {
namespace testing {

/// \~english @brief Represents the overall result of a test execution run.
///     This structure is typically serialized to/from a file (e.g., XML, JSON) and contains
///     a list of outcomes for individual test cases.
/// \~russian @brief Представляет общий результат выполнения тестового прогона.
///     Эта структура обычно сериализуется в/из файла (например, XML, JSON) и содержит
///     список результатов для отдельных тестовых случаев.
DEFINE_ROOT_NODE_BEGIN(Result)
    /// \~english @brief Represents an individual test case's outcome within the overall test results.
    ///     Each item in this list corresponds to a single test that was executed.
    /// \~russian @brief Представляет результат отдельного тестового случая в рамках общих результатов тестирования.
    ///     Каждый элемент в этом списке соответствует одному выполненному тесту.
    DEFINE_NODE_LIST_BEGIN(Tests)
        /// \~english @brief The path or unique identifier of the test case.
        /// \~russian @brief Путь или уникальный идентификатор тестового случая.
        DEFINE_ATTRIBUTE(std::string, Path, "")
        /// \~english @brief An error message if the test failed. This string is typically empty if the test passed.
        /// \~russian @brief Сообщение об ошибке, если тест не пройден. Эта строка обычно пуста, если тест пройден.
        DEFINE_ATTRIBUTE(std::string, Error, "")
        /// \~english @brief Any standard output captured from the test case during its execution.
        /// \~russian @brief Любой стандартный вывод, захваченный от тестового случая во время его выполнения.
        DEFINE_ATTRIBUTE(std::string, Output, "")
    DEFINE_NODE_LIST_END(Tests)
DEFINE_ROOT_NODE_END()

/// \~english @brief Interface for a test runner.
///     A test runner is responsible for discovering, executing tests (which are organized
///     in a node-like structure), and reporting their results.
/// \~russian @brief Интерфейс для средства запуска тестов.
///     Средство запуска тестов отвечает за обнаружение, выполнение тестов (которые организованы
///     в древовидной структуре) и формирование отчета о результатах.
class ITestRunner {
public:
    /// \~english @brief Defines common interface elements for `ITestRunner`, such as smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.).
    /// \~russian @brief Определяет общие элементы интерфейса для `ITestRunner`, такие как typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.).
    DEFINE_INTERFACE(ITestRunner)
    /// \~english @typedef INodeTest
    /// @brief Typedef for a specialized `INodeType` that holds a raw pointer to an `ITest` instance as its value.
    ///     This is used to represent a test case or a suite of tests within a hierarchical node structure
    ///     that the test runner can traverse and execute.
    /// \~russian @typedef INodeTest
    /// @brief Псевдоним типа для специализированного `INodeType`, который хранит сырой указатель на экземпляр `ITest` в качестве своего значения.
    ///     Используется для представления тестового случая или набора тестов в иерархической структуре узлов,
    ///     которую средство запуска тестов может обходить и выполнять.
    typedef parsing::INodeType<ITest *> INodeTest;

    /// \~english @brief Runs the tests defined in or under the provided node structure.
    /// \~russian @brief Запускает тесты, определенные в предоставленной структуре узлов или под ней.
    /// \~english @param node_test A shared pointer to the root node of the test suite or a single test case structure.
    ///     The runner will typically traverse this node tree to find and execute `ITest` instances.
    /// \~russian @param node_test Разделяемый указатель на корневой узел набора тестов или структуры отдельного тестового случая.
    ///     Средство запуска обычно обходит это дерево узлов для поиска и выполнения экземпляров `ITest`.
    /// \~english @return A `TResult` object containing the outcomes of all executed tests.
    /// \~russian @return Объект `TResult`, содержащий результаты всех выполненных тестов.
    virtual TResult run(INodeTest::TSharedPtr const &node_test) = 0;
};


} // testing
} // iridium

/// \~english @brief Declares a conversion specialization allowing an `iridium::testing::ITestRunner::INodeTest` object to be converted to a `std::string`.
///     This conversion likely extracts a meaningful string representation of the test node, such as its path or name, for display or logging.
///     The actual implementation is provided elsewhere.
/// \~russian @brief Объявляет специализацию преобразования, позволяющую объекту `iridium::testing::ITestRunner::INodeTest` быть преобразованным в `std::string`.
///     Это преобразование, вероятно, извлекает значимое строковое представление тестового узла, такое как его путь или имя, для отображения или логирования.
///     Фактическая реализация предоставляется в другом месте.
DEFINE_CONVERT(std::string, iridium::testing::ITestRunner::INodeTest)


#endif // HEADER_TEST_RUNNER_34E956BA_A097_4535_8EF2_FC8842A198B7
