// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

/// \~english @file
/// @brief Defines the `INodeType<TValue>` template interface for generic tree nodes.
///     This interface is fundamental for representing hierarchical data structures within the Iridium parsing framework.
///     It supports operations like getting/setting name and value, accessing children, and path-based slicing.
/// \~russian @file
/// @brief Определяет шаблонный интерфейс `INodeType<TValue>` для обобщенных узлов дерева.
///     Этот интерфейс является фундаментальным для представления иерархических структур данных
///     во фреймворке разбора Iridium. Он поддерживает операции, такие как получение/установка имени и значения,
///     доступ к дочерним элементам и срезы на основе пути.

#ifndef HEADER_NODE_96BF1757_0A3E_46F0_94CB_A7AFC4877775
#define  HEADER_NODE_96BF1757_0A3E_46F0_94CB_A7AFC4877775


#include <string>
#include <list>

#include "iridium/convertion/convert.h"
#include "iridium/pattern/prototype.h"
#include "iridium/smart_ptr.h"


// No specific Doxygen for iridium namespace itself here.
namespace iridium {
/// \~english @brief Provides interfaces, utilities, and concrete classes for parsing and composing structured data.
///     This namespace often deals with representing data as a tree of nodes (e.g., `INode`, `INodeType`).
/// \~russian @brief Предоставляет интерфейсы, утилиты и конкретные классы для разбора и формирования структурированных данных.
///     Это пространство имен часто связано с представлением данных в виде дерева узлов (например, `INode`, `INodeType`).
namespace parsing {


// todo: Depth-First Search, DFS) and (Breadth-First Search, BFS) methods maybe parallel

/// \~english @brief Interface for a generic node in a tree-like structure, templated on its value type.
///     Represents elements in parsed data structures such as XML, JSON, or configuration files.
///     A node can have a name, a primary value of type `TValue`, attributes (typically managed by concrete implementations),
///     and a list of child nodes. This class template inherits from `iridium::pattern::IPrototype`
///     suggesting support for cloning.
/// \~russian @brief Интерфейс для обобщенного узла в древовидной структуре, параметризованный типом его значения.
///     Представляет элементы в разобранных структурах данных, таких как XML, JSON или конфигурационные файлы.
///     Узел может иметь имя, основное значение типа `TValue`, атрибуты (обычно управляемые конкретными реализациями)
///     и список дочерних узлов. Этот шаблон класса наследуется от `iridium::pattern::IPrototype`,
///     что предполагает поддержку клонирования.
/// \~english @tparam TValue The type of the primary value stored in the node.
/// \~russian @tparam TValue Тип основного значения, хранящегося в узле.
template<typename TValue>
class INodeType : public pattern::IPrototype<INodeType<TValue> > {
public:
    /// \~english @brief Defines common interface elements for `INodeType`, such as smart pointer typedefs (`TSharedPtr`, `TWeakPtr`, etc.).
    /// \~russian @brief Определяет общие элементы интерфейса для `INodeType`, такие как typedef-ы умных указателей (`TSharedPtr`, `TWeakPtr` и т.д.).
    DEFINE_INTERFACE(INodeType)
<<<<<<< HEAD
    /// \~english @brief Default delimiter character used for constructing and interpreting paths to navigate or access child nodes. Typically '/'.
    /// \~russian @brief Символ-разделитель по умолчанию, используемый для построения и интерпретации путей для навигации или доступа к дочерним узлам. Обычно '/'.
    static std::string const PATH_DELIMITER;

    // todo: typedef unordered_map<name, INodeType> TNodes;
    /// \~english @brief A list of shared pointers to child nodes.
    /// \~russian @brief Список разделяемых указателей на дочерние узлы.
=======
    ///
>>>>>>> 85fcd15 ( - parsing: node bugfix for musl)
    typedef std::list<typename INodeType::TSharedPtr> TNodes;
    /// \~english @brief Iterator for traversing child nodes.
    /// \~russian @brief Итератор для обхода дочерних узлов.
    typedef typename TNodes::iterator           iterator;
    /// \~english @brief Const iterator for traversing child nodes.
    /// \~russian @brief Константный итератор для обхода дочерних узлов.
    typedef typename TNodes::const_iterator     const_iterator;
    /// \~english @brief Const reference to a child node (typically a shared pointer).
    /// \~russian @brief Константная ссылка на дочерний узел (обычно разделяемый указатель).
    typedef typename TNodes::const_reference    const_reference;
<<<<<<< HEAD

    /// \~english @brief Gets the name of this node.
    /// \~russian @brief Получает имя этого узла.
    /// \~english @return The name of the node as a string.
    /// \~russian @return Имя узла в виде строки.
=======
    ///
>>>>>>> 85fcd15 ( - parsing: node bugfix for musl)
    virtual std::string     getName() const = 0;
    /// \~english @brief Gets the primary value of this node.
    /// \~russian @brief Получает основное значение этого узла.
    /// \~english @return The value of the node, of type `TValue`.
    /// \~russian @return Значение узла типа `TValue`.
    virtual TValue          getValue() const = 0;
    /// \~english @brief Sets the primary value of this node.
    /// \~russian @brief Устанавливает основное значение этого узла.
    /// \~english @param value The new value to set for the node.
    /// \~russian @param value Новое значение для установки узлу.
    virtual void            setValue(TValue const &value) = 0;
<<<<<<< HEAD

    /// \~english @brief Retrieves a list of child nodes matching a given path.
    ///     The path can specify a sequence of child names, typically delimited by `PATH_DELIMITER`.
    ///     Note: Source comments suggest this might return a full subtree and could be parallelized.
    /// \~russian @brief Получает список дочерних узлов, соответствующих заданному пути.
    ///     Путь может указывать последовательность имен дочерних узлов, обычно разделенных `PATH_DELIMITER`.
    ///     Примечание: Комментарии в исходном коде предполагают, что этот метод может возвращать полное поддерево и может быть распараллелен.
    /// \~english @param path The path string to locate child nodes.
    /// \~russian @param path Строка пути для поиска дочерних узлов.
    /// \~english @return A `TNodes` list containing shared pointers to the matching child nodes. May be empty if no nodes match.
    /// \~russian @return Список `TNodes`, содержащий разделяемые указатели на соответствующие дочерние узлы. Может быть пустым, если узлы не найдены.
    virtual typename INodeType::TNodes slice(std::string const &path) = 0;
    ///// todo:
    //virtual typename INodeType::TNodesConst findChilds(std::string const &path) const = 0;

    /// \~english @brief Returns an iterator to the beginning of the child node list.
    /// \~russian @brief Возвращает итератор на начало списка дочерних узлов.
    /// \~english @return An `iterator` pointing to the first child node.
    /// \~russian @return Итератор `iterator`, указывающий на первый дочерний узел.
=======
    ///
    virtual typename INodeType::TNodes slice(std::string const &path) = 0;
    ///// todo:
    //virtual typename INodeType::TNodesConst findChilds(std::string const &path) const = 0;
    ///
>>>>>>> 85fcd15 ( - parsing: node bugfix for musl)
    virtual iterator        begin() = 0;
    /// \~english @brief Returns an iterator to the end of the child node list.
    /// \~russian @brief Возвращает итератор на конец списка дочерних узлов.
    /// \~english @return An `iterator` pointing past the last child node.
    /// \~russian @return Итератор `iterator`, указывающий за последний дочерний узел.
    virtual iterator        end() = 0;
    /// \~english @brief Returns a const iterator to the beginning of the child node list.
    /// \~russian @brief Возвращает константный итератор на начало списка дочерних узлов.
    /// \~english @return A `const_iterator` pointing to the first child node.
    /// \~russian @return Константный итератор `const_iterator`, указывающий на первый дочерний узел.
    virtual const_iterator  begin()     const = 0;
    /// \~english @brief Returns a const iterator to the end of the child node list.
    /// \~russian @brief Возвращает константный итератор на конец списка дочерних узлов.
    /// \~english @return A `const_iterator` pointing past the last child node.
    /// \~russian @return Константный итератор `const_iterator`, указывающий за последний дочерний узел.
    virtual const_iterator  end()       const = 0;
    /// \~english @brief Gets the number of direct child nodes.
    /// \~russian @brief Получает количество прямых дочерних узлов.
    /// \~english @return The count of direct children.
    /// \~russian @return Количество прямых дочерних узлов.
    virtual size_t          size()      const = 0;
    /// \~english @brief Checks if this node has any child nodes.
    /// \~russian @brief Проверяет, есть ли у этого узла дочерние узлы.
    /// \~english @return `true` if the node has one or more children, `false` otherwise.
    /// \~russian @return `true`, если узел имеет одного или более дочерних узлов, иначе `false`.
    virtual bool            hasChilds() const = 0;

    /// \~english @brief Gets the first child node with the specified name.
    /// \~russian @brief Получает первый дочерний узел с указанным именем.
    /// \~english @param name The name of the child node to find.
    /// \~russian @param name Имя искомого дочернего узла.
    /// \~english @return A shared pointer to the child node if found; otherwise, a null or empty shared pointer.
    /// \~russian @return Разделяемый указатель на дочерний узел, если найден; в противном случае — нулевой или пустой разделяемый указатель.
    virtual typename INodeType<TValue>::TSharedPtr      getChild(std::string const &name) = 0;
    /// \~english @brief Gets the first child node with the specified name (const version).
    /// \~russian @brief Получает первый дочерний узел с указанным именем (константная версия).
    /// \~english @param name The name of the child node to find.
    /// \~russian @param name Имя искомого дочернего узла.
    /// \~english @return A const shared pointer to the child node if found; otherwise, a null or empty shared pointer.
    /// \~russian @return Константный разделяемый указатель на дочерний узел, если найден; в противном случае — нулевой или пустой разделяемый указатель.
    virtual typename INodeType<TValue>::TConstSharedPtr getChild(std::string const &name) const = 0;
    /// \~english @brief Gets all child nodes with the specified name.
    /// \~russian @brief Получает все дочерние узлы с указанным именем.
    /// \~english @param name The name of the child nodes to find.
    /// \~russian @param name Имя искомых дочерних узлов.
    /// \~english @return A `TNodes` list containing shared pointers to all child nodes with the given name. May be empty.
    /// \~russian @return Список `TNodes`, содержащий разделяемые указатели на все дочерние узлы с указанным именем. Может быть пустым.
    virtual          TNodes                             getChilds(std::string const &name) const = 0;
    /// \~english @brief Adds an existing node as a child to this node.
    /// \~russian @brief Добавляет существующий узел в качестве дочернего к этому узлу.
    /// \~english @param child_node A shared pointer to the node to be added as a child.
    /// \~russian @param child_node Разделяемый указатель на узел, который будет добавлен как дочерний.
    /// \~english @return A shared pointer to the added child node (which might be `child_node` itself or a copy depending on implementation).
    /// \~russian @return Разделяемый указатель на добавленный дочерний узел (это может быть сам `child_node` или его копия, в зависимости от реализации).
    virtual typename INodeType::TSharedPtr              addChild(typename INodeType::TSharedPtr const &child_node) = 0;
    /// \~english @brief Creates and adds a new child node with the specified name.
    /// \~russian @brief Создает и добавляет новый дочерний узел с указанным именем.
    /// \~english @param name The name for the new child node.
    /// \~russian @param name Имя для нового дочернего узла.
    /// \~english @return A shared pointer to the newly created child node.
    /// \~russian @return Разделяемый указатель на вновь созданный дочерний узел.
    virtual typename INodeType::TSharedPtr              addChild(std::string const &name) = 0;
    /// \~english @brief Creates and adds a new child node with the specified name and value.
    /// \~russian @brief Создает и добавляет новый дочерний узел с указанными именем и значением.
    /// \~english @param name The name for the new child node.
    /// \~russian @param name Имя для нового дочернего узла.
    /// \~english @param value The primary value for the new child node.
    /// \~russian @param value Основное значение для нового дочернего узла.
    /// \~english @return A shared pointer to the newly created child node.
    /// \~russian @return Разделяемый указатель на вновь созданный дочерний узел.
    virtual typename INodeType::TSharedPtr              addChild(std::string const &name, TValue const &value) = 0;
    /// \~english @brief Deletes the first child node with the specified name.
    ///     If multiple children have the same name, only the first one encountered is typically removed.
    /// \~russian @brief Удаляет первый дочерний узел с указанным именем.
    ///     Если несколько дочерних узлов имеют одинаковое имя, обычно удаляется только первый встреченный.
    /// \~english @param name The name of the child node to delete.
    /// \~russian @param name Имя удаляемого дочернего узла.
    virtual void                                        delChild(std::string const &name) = 0;
};

<<<<<<< HEAD
/// \~english @brief Definition of the default path delimiter constant.
/// \~russian @brief Определение константы разделителя пути по умолчанию.
template<typename TValue>
std::string const INodeType<TValue>::PATH_DELIMITER("/");

=======
>>>>>>> 85fcd15 ( - parsing: node bugfix for musl)

} // parsing
} // iridium


#endif // HEADER_NODE_96BF1757_0A3E_46F0_94CB_A7AFC4877775
