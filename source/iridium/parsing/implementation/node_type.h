// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NODE_0A80E93C_07DA_494B_8C4D_8D0AF4B134A5
#define  HEADER_NODE_0A80E93C_07DA_494B_8C4D_8D0AF4B134A5


#include <string>
#include <list>
#include <unordered_set>

#include "iridium/smart_ptr.h"
#include "iridium/parsing/node_type.h"


namespace iridium {
namespace parsing {
namespace implementation {


// ----- interface

/// \~english @brief Concrete implementation of the `INodeType` interface.
///     This class template provides a full implementation for a node in a tree-like structure.
///     It manages the node's name (using string interning for efficiency), its primary value of type `TValue`,
///     and a list of child nodes. It is designed to be cloneable via the `IPrototype` interface.
/// \~russian @brief Конкретная реализация интерфейса `INodeType`.
///     Этот шаблон класса предоставляет полную реализацию для узла в древовидной структуре.
///     Он управляет именем узла (используя интернирование строк для эффективности), его основным значением типа `TValue`
///     и списком дочерних узлов. Класс спроектирован для поддержки клонирования через интерфейс `IPrototype`.
/// \~english @tparam TValue The type of the primary value stored in the node.
/// \~russian @tparam TValue Тип основного значения, хранящегося в узле.
template<typename TValue>
class CNodeType : public INodeType<TValue> {
public:
    /// \~english @brief Defines standard implementation helpers for `CNodeType`, such as smart pointer typedefs and a static `create` method.
    /// \~russian @brief Определяет стандартные вспомогательные средства реализации для `CNodeType`, такие как typedef-ы умных указателей и статический метод `create`.
    DEFINE_IMPLEMENTATION(CNodeType)
    /// \~english @brief Constructs a node with a given name and value. Initializes a new string interning pool.
    /// \~russian @brief Конструирует узел с заданным именем и значением. Инициализирует новый пул интернирования строк.
    /// \~english @param name The name of the node.
    /// \~russian @param name Имя узла.
    /// \~english @param value The primary value of the node.
    /// \~russian @param value Основное значение узла.
    explicit CNodeType(std::string const &name, TValue const &value);
    /// \~english @brief Constructs a node with a given name and a default-constructed value. Initializes a new string interning pool.
    /// \~russian @brief Конструирует узел с заданным именем и значением, созданным по умолчанию. Инициализирует новый пул интернирования строк.
    /// \~english @param name The name of the node.
    /// \~russian @param name Имя узла.
    explicit CNodeType(std::string const &name);

    /// \~english @brief Gets the name of this node. Returns the interned string.
    /// \~russian @brief Получает имя этого узла. Возвращает интернированную строку.
    /// \~english @return The name of the node.
    /// \~russian @return Имя узла.
    std::string getName() const override final;
    /// \~english @brief Gets the primary value of this node.
    /// \~russian @brief Получает основное значение этого узла.
    /// \~english @return The value of the node.
    /// \~russian @return Значение узла.
    TValue      getValue() const override;
    /// \~english @brief Sets the primary value of this node.
    /// \~russian @brief Устанавливает основное значение этого узла.
    /// \~english @param value The new value to set.
    /// \~russian @param value Новое значение для установки.
    void        setValue(TValue const &value) override;
    /// \~english @brief Retrieves child nodes matching a given path. Implements path traversal.
    /// \~russian @brief Получает дочерние узлы, соответствующие заданному пути. Реализует обход по пути.
    /// \~english @param path The path string to locate child nodes.
    /// \~russian @param path Строка пути для поиска дочерних узлов.
    /// \~english @return A list of shared pointers to matching child nodes.
    /// \~russian @return Список разделяемых указателей на соответствующие дочерние узлы.
    typename INodeType<TValue>::TNodes          slice(std::string const &path) override;

    /// \~english @brief Returns an iterator to the beginning of the child node list.
    /// \~russian @brief Возвращает итератор на начало списка дочерних узлов.
    /// \~english @return An iterator to the first child.
    /// \~russian @return Итератор на первый дочерний узел.
    typename INodeType<TValue>::iterator        begin() override;
    /// \~english @brief Returns an iterator to the end of the child node list.
    /// \~russian @brief Возвращает итератор на конец списка дочерних узлов.
    /// \~english @return An iterator referring to the past-the-end element.
    /// \~russian @return Итератор, указывающий на элемент после последнего.
    typename INodeType<TValue>::iterator        end() override;
    /// \~english @brief Returns a const iterator to the beginning of the child node list.
    /// \~russian @brief Возвращает константный итератор на начало списка дочерних узлов.
    /// \~english @return A const iterator to the first child.
    /// \~russian @return Константный итератор на первый дочерний узел.
    typename INodeType<TValue>::const_iterator  begin() const override;
    /// \~english @brief Returns a const iterator to the end of the child node list.
    /// \~russian @brief Возвращает константный итератор на конец списка дочерних узлов.
    /// \~english @return A const iterator referring to the past-the-end element.
    /// \~russian @return Константный итератор, указывающий на элемент после последнего.
    typename INodeType<TValue>::const_iterator  end() const override;

    /// \~english @brief Gets the number of direct child nodes.
    /// \~russian @brief Получает количество прямых дочерних узлов.
    /// \~english @return The number of children.
    /// \~russian @return Количество дочерних узлов.
    size_t size() const override;
    /// \~english @brief Checks if this node has any child nodes.
    /// \~russian @brief Проверяет, есть ли у этого узла дочерние узлы.
    /// \~english @return True if there are child nodes, false otherwise.
    /// \~russian @return True, если есть дочерние узлы, иначе false.
    bool hasChilds() const override;

    /// \~english @brief Gets the first child node with the specified name.
    /// \~russian @brief Получает первый дочерний узел с указанным именем.
    /// \~english @param name The name of the child to retrieve.
    /// \~russian @param name Имя извлекаемого дочернего узла.
    /// \~english @return A shared pointer to the child if found, otherwise nullptr.
    /// \~russian @return Разделяемый указатель на дочерний узел, если найден, иначе nullptr.
    typename INodeType<TValue>::TSharedPtr      getChild(std::string const &name) override;
    /// \~english @brief Gets the first child node with the specified name (const version).
    /// \~russian @brief Получает первый дочерний узел с указанным именем (константная версия).
    /// \~english @param name The name of the child to retrieve.
    /// \~russian @param name Имя извлекаемого дочернего узла.
    /// \~english @return A const shared pointer to the child if found, otherwise nullptr.
    /// \~russian @return Константный разделяемый указатель на дочерний узел, если найден, иначе nullptr.
    typename INodeType<TValue>::TConstSharedPtr getChild(std::string const &name) const override;
    /// \~english @brief Gets all child nodes with the specified name.
    /// \~russian @brief Получает все дочерние узлы с указанным именем.
    /// \~english @param name The name of the children to retrieve.
    /// \~russian @param name Имя извлекаемых дочерних узлов.
    /// \~english @return A list of shared pointers to all children with the given name.
    /// \~russian @return Список разделяемых указателей на все дочерние узлы с данным именем.
    typename INodeType<TValue>::TNodes          getChilds(std::string const &name) const override;

    /// \~english @brief Adds an existing node as a child.
    /// \~russian @brief Добавляет существующий узел в качестве дочернего.
    /// \~english @param child_node The node to add as a child.
    /// \~russian @param child_node Узел для добавления в качестве дочернего.
    /// \~english @return A shared pointer to the added child.
    /// \~russian @return Разделяемый указатель на добавленный дочерний узел.
    typename INodeType<TValue>::TSharedPtr      addChild(typename INodeType<TValue>::TSharedPtr const &child_node) override;
    /// \~english @brief Creates and adds a new child node with the specified name and a default-constructed value.
    ///     The new child will share the same string interning pool as this parent node.
    /// \~russian @brief Создает и добавляет новый дочерний узел с указанным именем и значением, созданным по умолчанию.
    ///     Новый дочерний узел будет использовать тот же пул интернирования строк, что и родительский узел.
    /// \~english @param name The name for the new child node.
    /// \~russian @param name Имя для нового дочернего узла.
    /// \~english @return A shared pointer to the newly created child node.
    /// \~russian @return Разделяемый указатель на вновь созданный дочерний узел.
    typename INodeType<TValue>::TSharedPtr      addChild(std::string const &name) override;
    /// \~english @brief Creates and adds a new child node with the specified name and value.
    ///     The new child will share the same string interning pool as this parent node.
    /// \~russian @brief Создает и добавляет новый дочерний узел с указанными именем и значением.
    ///     Новый дочерний узел будет использовать тот же пул интернирования строк, что и родительский узел.
    /// \~english @param name The name for the new child node.
    /// \~russian @param name Имя для нового дочернего узла.
    /// \~english @param value The value for the new child node.
    /// \~russian @param value Значение для нового дочернего узла.
    /// \~english @return A shared pointer to the newly created child node.
    /// \~russian @return Разделяемый указатель на вновь созданный дочерний узел.
    typename INodeType<TValue>::TSharedPtr      addChild(std::string const &name, TValue const &value) override;
    /// \~english @brief Deletes all child nodes with the specified name.
    /// \~russian @brief Удаляет все дочерние узлы с указанным именем.
    /// \~english @param name The name of the child node(s) to delete.
    /// \~russian @param name Имя удаляемого дочернего узла (узлов).
    void                                        delChild(std::string const &name) override;
    /// \~english @brief Creates a deep copy (clone) of this node and its entire subtree.
    ///     The cloned nodes will share the same string interning pool.
    /// \~russian @brief Создает глубокую копию (клон) этого узла и всего его поддерева.
    ///     Клонированные узлы будут использовать тот же пул интернирования строк.
    /// \~english @return A shared pointer to the new cloned node.
    /// \~russian @return Разделяемый указатель на новый клонированный узел.
    typename INodeType<TValue>::TSharedPtr      clone() const override;

private:
    /// \~english @brief Type alias for an unordered set of strings, used for interning node names to save memory.
    /// \~russian @brief Псевдоним типа для неупорядоченного множества строк, используемый для интернирования имен узлов для экономии памяти.
    typedef std::unordered_set<std::string> TStrings;
    /// \~english @brief Internal constructor used for creating nodes (often children) that share an existing string interning pool.
    /// \~russian @brief Внутренний конструктор, используемый для создания узлов (часто дочерних), которые разделяют существующий пул интернирования строк.
    /// \~english @param name The name of the node.
    /// \~russian @param name Имя узла.
    /// \~english @param value The primary value of the node.
    /// \~russian @param value Основное значение узла.
    /// \~english @param strings A shared pointer to the string interning pool.
    /// \~russian @param strings Разделяемый указатель на пул интернирования строк.
    explicit CNodeType(std::string const &name, TValue const &value, std::shared_ptr<TStrings> const &strings);
    /// \~english @brief Internal constructor for nodes with default-constructed values, sharing an existing string interning pool.
    /// \~russian @brief Внутренний конструктор для узлов со значениями, созданными по умолчанию, разделяющий существующий пул интернирования строк.
    /// \~english @param name The name of the node.
    /// \~russian @param name Имя узла.
    /// \~english @param strings A shared pointer to the string interning pool.
    /// \~russian @param strings Разделяемый указатель на пул интернирования строк.
    explicit CNodeType(std::string const &name, std::shared_ptr<TStrings> const &strings);
    /// \~english @brief Interns the given string name using the shared `m_strings` pool.
    /// \~russian @brief Интернирует данное строковое имя, используя общий пул `m_strings`.
    /// \~english @param name The string name to intern.
    /// \~russian @param name Строковое имя для интернирования.
    /// \~english @return A pointer to the interned string within the pool.
    /// \~russian @return Указатель на интернированную строку в пуле.
    std::string const *makeName(std::string const &name);
    /// \~english @brief Shared pointer to the set of interned strings. This pool is shared among a node and its descendants created via `addChild`.
    /// \~russian @brief Разделяемый указатель на множество интернированных строк. Этот пул разделяется между узлом и его потомками, созданными через `addChild`.
    std::shared_ptr<TStrings> m_strings;
    /// \~english @brief Pointer to the interned name of this node. The actual string data resides in `m_strings`.
    /// \~russian @brief Указатель на интернированное имя этого узла. Фактические строковые данные находятся в `m_strings`.
    std::string const * const m_name = nullptr;
    /// \~english @brief The primary value of this node, of type `TValue`.
    /// \~russian @brief Основное значение этого узла типа `TValue`.
    TValue m_value;
    /// \~english @brief List of child nodes of this node.
    /// \~russian @brief Список дочерних узлов этого узла.
    typename INodeType<TValue>::TNodes m_nodes;
};


// ----- implementation


template<typename TValue>
std::string const * CNodeType<TValue>::makeName(std::string const& name) {
    return &*m_strings->insert(name).first;
}


template<typename TValue>
CNodeType<TValue>::CNodeType(std::string const &name, TValue const &value)
:
    m_strings   (std::make_shared<TStrings>()),
    m_name      (makeName(name)),
    m_value     (value)
{}


template<typename TValue>
CNodeType<TValue>::CNodeType(std::string const &name)
:
    m_strings   (std::make_shared<TStrings>()),
    m_name      (makeName(name)),
    m_value     ()
{}


template<typename TValue>
CNodeType<TValue>::CNodeType(std::string const &name, TValue const &value, std::shared_ptr<TStrings> const &strings)
:
    m_strings   (strings),
    m_name      (makeName(name)),
    m_value     (value)
{}


template<typename TValue>
CNodeType<TValue>::CNodeType(std::string const &name, std::shared_ptr<TStrings> const &strings)
:
    m_strings   (strings),
    m_name      (makeName(name)),
    m_value     ()
{}


template<typename TValue>
std::string CNodeType<TValue>::getName() const {
    return *m_name; // ----->
}


template<typename TValue>
TValue CNodeType<TValue>::getValue() const {
    return m_value; // ----->
}


template<typename TValue>
void CNodeType<TValue>::setValue(TValue const &value) {
    m_value = value;
}


template<typename TValue>
typename INodeType<TValue>::TNodes CNodeType<TValue>::slice(std::string const &path) {
    // todo: depth overflow test
    typename INodeType<TValue>::TNodes nodes;

    if (path.empty())
        return nodes; // ----->

    if (path == "/")
        return m_nodes; // ----->

    std::string child_path = path.substr(1, path.find(INodeType<TValue>::PATH_DELIMITER, 1) - 1);
    std::string next_path;

    if ((child_path.size() + INodeType<TValue>::PATH_DELIMITER.size()) <= path.size())
        next_path = path.substr(child_path.size() + INodeType<TValue>::PATH_DELIMITER.size());

    if (next_path.empty()) {
        for (auto &node: m_nodes)
            if (node->getName() == child_path)
                nodes.push_back(node);
    } else {
        for (auto &node: m_nodes)
            if (node->getName() == child_path)
                nodes.splice(nodes.end(), node->slice(next_path));
    }

    return nodes; // ----->
}


template<typename TValue>
typename INodeType<TValue>::iterator CNodeType<TValue>::begin() {
    return m_nodes.begin(); // ----->
}


template<typename TValue>
typename INodeType<TValue>::iterator CNodeType<TValue>::end() {
    return m_nodes.end(); // ----->
}


template<typename TValue>
typename INodeType<TValue>::const_iterator CNodeType<TValue>::begin() const {
    return m_nodes.cbegin();
//    return reinterpret_cast<typename INodeType<TValue>::TNodes const &>(m_nodes).cbegin(); // ----->
}


template<typename TValue>
typename INodeType<TValue>::const_iterator CNodeType<TValue>::end() const {
    return m_nodes.cend();
//    return reinterpret_cast<typename INodeType<TValue>::TNodesConst const &>(m_nodes).cend(); // ----->
}


template<typename TValue>
size_t CNodeType<TValue>::size() const {
    return m_nodes.size(); // ----->
}


template<typename TValue>
bool CNodeType<TValue>::hasChilds() const {
    return !m_nodes.empty(); // ----->
}


template<typename TValue>
typename INodeType<TValue>::TSharedPtr CNodeType<TValue>::getChild(std::string const &name) {
    for (auto &i: m_nodes)
        if (i->getName() == name)
            return i; // ----->
    return nullptr; // ----->
}


template<typename TValue>
typename INodeType<TValue>::TConstSharedPtr CNodeType<TValue>::getChild(std::string const &name) const {
    for (auto const &i: m_nodes)
        if (i->getName() == name)
            return i; // ----->
    return nullptr; // ----->
}


template<typename TValue>
typename INodeType<TValue>::TNodes CNodeType<TValue>::getChilds(std::string const &name) const {
    typename INodeType<TValue>::TNodes nodes;

    for (auto const &node: m_nodes)
        if (node->getName() == name)
            nodes.push_back(node);

    return nodes; // ----->
}


template<typename TValue>
typename INodeType<TValue>::TSharedPtr CNodeType<TValue>::addChild(typename INodeType<TValue>::TSharedPtr const &child_node) {
    if (child_node) {
        // todo: merge name map
        m_nodes.push_back(child_node);
        return m_nodes.back(); // ----->
    } else
        return {}; // ----->
}


template<typename TValue>
typename INodeType<TValue>::TSharedPtr CNodeType<TValue>::addChild(std::string const &name) {
    return addChild(CNodeType<TValue>::TSharedPtr(new CNodeType<TValue>(name, m_strings))); // ----->
}


template<typename TValue>
typename INodeType<TValue>::TSharedPtr CNodeType<TValue>::addChild(std::string const &name, TValue const &value) {
    return addChild(CNodeType<TValue>::TSharedPtr(new CNodeType<TValue>(name, value, m_strings))); // ----->
}


template<typename TValue>
void CNodeType<TValue>::delChild(std::string const &name) {
    m_nodes.remove_if(
        [&] (auto const &node) {
            return node->getName() == name;
        }
    );
}


template<typename TValue>
typename INodeType<TValue>::TSharedPtr CNodeType<TValue>::clone() const {
    typename INodeType<TValue>::TSharedPtr node = CNodeType<TValue>::create(*m_name, m_value);

    for (auto const &i: m_nodes)
        node->addChild(i->clone());

    return node; // ----->
}


} // implementation
} // parser
} // iridium


#endif // HEADER_NODE_0A80E93C_07DA_494B_8C4D_8D0AF4B134A5
