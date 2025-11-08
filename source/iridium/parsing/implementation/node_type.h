// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NODE_0A80E93C_07DA_494B_8C4D_8D0AF4B134A5
#define  HEADER_NODE_0A80E93C_07DA_494B_8C4D_8D0AF4B134A5


#include <string>
#include <list>
#include <unordered_set>
#include <algorithm>

#include "iridium/smart_ptr.h"
#include "iridium/parsing/node_type.h"


namespace iridium {
namespace parsing {
namespace implementation {


// ----- interface


template<typename TValue>
class CNodeType : virtual public INodeType<TValue> {
public:
    DEFINE_CREATE(CNodeType)
    /// \~english @brief Constructor with name and value.
    /// \~russian @brief Конструктор с именем и значением.
    explicit CNodeType(std::string const &name, TValue const &value);
    /// \~english @brief Constructor with name, value, and type.
    /// \~russian @brief Конструктор с именем, значением и типом.
    explicit CNodeType(std::string const &name, TValue const &value, std::string const &type);
    /// \~english @brief Constructor with name only.
    /// \~russian @brief Конструктор только с именем.
    explicit CNodeType(std::string const &name);
    /// \~english @brief Virtual destructor.
    /// \~russian @brief Виртуальный деструктор.
    virtual ~CNodeType();
    /// \~english @brief Gets the parent nodes.
    /// \~russian @brief Возвращает родительские узлы.
    virtual typename INodeType<TValue>::TNodesPtr getParents() const override;
    /// \~english @brief Gets the node name.
    /// \~russian @brief Возвращает имя узла.
    std::string getName() const override final;
    /// \~english @brief Gets the node value.
    /// \~russian @brief Возвращает значение узла.
    TValue      getValue() const override;
    /// \~english @brief Sets the node value.
    /// \~russian @brief Устанавливает значение узла.
    void        setValue(TValue const &value) override;
    /// \~english @brief Gets the node type.
    /// \~russian @brief Возвращает тип узла.
    std::string getType() const override;
    /// \~english @brief Slices the node tree based on a path.
    /// \~russian @brief "Срезает" дерево узлов на основе пути.
    typename INodeType<TValue>::TNodesSharedPtr slice(std::string const &path) const override;
    /// \~english @brief Returns an iterator to the beginning of the child nodes.
    /// \~russian @brief Возвращает итератор на начало дочерних узлов.
    typename INodeType<TValue>::iterator        begin() override;
    /// \~english @brief Returns an iterator to the end of the child nodes.
    /// \~russian @brief Возвращает итератор на конец дочерних узлов.
    typename INodeType<TValue>::iterator        end() override;
    /// \~english @brief Returns a constant iterator to the beginning of the child nodes.
    /// \~russian @brief Возвращает константный итератор на начало дочерних узлов.
    typename INodeType<TValue>::const_iterator  begin() const override;
    /// \~english @brief Returns a constant iterator to the end of the child nodes.
    /// \~russian @brief Возвращает константный итератор на конец дочерних узлов.
    typename INodeType<TValue>::const_iterator  end() const override;
    /// \~english @brief Gets the number of child nodes.
    /// \~russian @brief Возвращает количество дочерних узлов.
    size_t size() const override;
    /// \~english @brief Checks if the node has children.
    /// \~russian @brief Проверяет, есть ли у узла дочерние узлы.
    bool hasChilds() const override;
    /// \~english @brief Gets a child node by name.
    /// \~russian @brief Возвращает дочерний узел по имени.
    typename INodeType<TValue>::TSharedPtr      getChild(std::string const &name) override;
    /// \~english @brief Gets a constant child node by name.
    /// \~russian @brief Возвращает константный дочерний узел по имени.
    typename INodeType<TValue>::TConstSharedPtr getChild(std::string const &name) const override;
    /// \~english @brief Gets all child nodes with a specific name.
    /// \~russian @brief Возвращает все дочерние узлы с указанным именем.
    typename INodeType<TValue>::TNodesSharedPtr getChilds(std::string const &name) const override;
    /// \~english @brief Adds a child node.
    /// \~russian @brief Добавляет дочерний узел.
    typename INodeType<TValue>::TSharedPtr      addChild(typename INodeType<TValue>::TSharedPtr const &child_node) override;
    /// \~english @brief Adds a child node with a specific name.
    /// \~russian @brief Добавляет дочерний узел с указанным именем.
    typename INodeType<TValue>::TSharedPtr      addChild(std::string const &name) override;
    /// \~english @brief Adds a child node with a specific name and value.
    /// \~russian @brief Добавляет дочерний узел с указанным именем и значением.
    typename INodeType<TValue>::TSharedPtr      addChild(std::string const &name, TValue const &value) override;
    /// \~english @brief Deletes all child nodes with a specific name.
    /// \~russian @brief Удаляет все дочерние узлы с указанным именем.
    void                                        delChilds(std::string const &name) override;
    /// \~english @brief Clones the node and its children.
    /// \~russian @brief Клонирует узел и его дочерние узлы.
    typename INodeType<TValue>::TSharedPtr      clone() const override;

private:
    /// \~english @brief A set of strings for optimizing memory usage.
    /// \~russian @brief Множество строк для оптимизации использования памяти.
    typedef std::unordered_set<std::string> TStrings;
    /// \~english @brief Private constructor for internal use.
    /// \~russian @brief Приватный конструктор для внутреннего использования.
    explicit CNodeType(std::string const &name, TValue const &value, std::shared_ptr<TStrings> const &strings);
    /// \~english @brief Private constructor for internal use.
    /// \~russian @brief Приватный конструктор для внутреннего использования.
    explicit CNodeType(std::string const &name, std::shared_ptr<TStrings> const &strings);
    /// \~english @brief Interns a string to save memory.
    /// \~russian @brief Интернирует строку для экономии памяти.
    std::string const *makeName(std::string const &name);
    /// \~english @brief Shared pointer to the string interning table.
    /// \~russian @brief Разделяемый указатель на таблицу интернирования строк.
    std::shared_ptr<TStrings> m_strings;
    /// \~english @brief Pointer to the interned name of the node.
    /// \~russian @brief Указатель на интернированное имя узла.
    std::string const * const m_name = nullptr;
    /// \~english @brief Pointer to the interned type of the node.
    /// \~russian @brief Указатель на интернированный тип узла.
    std::string const * const m_type = nullptr;
    /// \~english @brief The value of the node.
    /// \~russian @brief Значение узла.
    TValue m_value;
    /// \~english @brief List of child nodes.
    /// \~russian @brief Список дочерних узлов.
    typename INodeType<TValue>::TNodesSharedPtr m_nodes;
    /// \~english @brief List of parent nodes.
    /// \~russian @brief Список родительских узлов.
    typename INodeType<TValue>::TNodesPtr       m_parent_nodes;
};


// ----- implementation


template<typename TValue>
std::string const * CNodeType<TValue>::makeName(std::string const &name) {
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
CNodeType<TValue>::CNodeType(std::string const &name, TValue const &value, std::string const &type)
:
    m_strings   (std::make_shared<TStrings>()),
    m_name      (makeName(name)),
    m_type      (makeName(type)),
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
CNodeType<TValue>::~CNodeType() {
    for (auto const &node: m_nodes) {
        auto &parents = std::dynamic_pointer_cast< CNodeType<TValue> >(node)->m_parent_nodes;
        parents.erase(
            std::remove_if(
                parents.begin(),
                parents.end(),
                    [&](auto const &node) {
                       return node == this;
                    }
            )
        );
    }
}


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
typename INodeType<TValue>::TNodesPtr CNodeType<TValue>::getParents() const {
    return m_parent_nodes;
}


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
std::string CNodeType<TValue>::getType() const {
    if (m_type)
        return *m_type; // ----->
    else
        return {}; // ----->
}


template<typename TValue>
typename INodeType<TValue>::TNodesSharedPtr CNodeType<TValue>::slice(std::string const &path) const {
    static std::string const DEFAULT_PATH_DELIMITER("/");

    // todo: depth overflow test
    typename INodeType<TValue>::TNodesSharedPtr nodes;

    if (path.empty())
        return nodes; // ----->

    if (path == "/")
        return m_nodes; // ----->

    std::string child_path = path.substr(1, path.find(DEFAULT_PATH_DELIMITER, 1) - 1);
    std::string next_path;

    if ((child_path.size() + DEFAULT_PATH_DELIMITER.size()) <= path.size())
        next_path = path.substr(child_path.size() + DEFAULT_PATH_DELIMITER.size());

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
typename INodeType<TValue>::TNodesSharedPtr CNodeType<TValue>::getChilds(std::string const &name) const {
    typename INodeType<TValue>::TNodesSharedPtr nodes;

    for (auto const &node: m_nodes)
        if (node->getName() == name)
            nodes.push_back(node);

    return nodes; // ----->
}


template<typename TValue>
typename INodeType<TValue>::TSharedPtr CNodeType<TValue>::addChild(typename INodeType<TValue>::TSharedPtr const &child_node) {
    if (child_node) {
        std::dynamic_pointer_cast< CNodeType<TValue> >(child_node)->m_parent_nodes.push_back(this);
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
void CNodeType<TValue>::delChilds(std::string const &name) {
    m_nodes.remove_if(
        [&] (auto const &node) {
            return node->getName() == name;
        }
    );
}


template<typename TValue>
typename INodeType<TValue>::TSharedPtr CNodeType<TValue>::clone() const {
    auto node = CNodeType<TValue>::create(*m_name, m_value);

    for (auto const &i: m_nodes)
        node->addChild(i->clone());

    node->m_parent_nodes.clear();
    return node; // ----->
}


} // implementation
} // parser
} // iridium


#endif // HEADER_NODE_0A80E93C_07DA_494B_8C4D_8D0AF4B134A5
