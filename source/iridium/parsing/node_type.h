// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NODE_96BF1757_0A3E_46F0_94CB_A7AFC4877775
#define  HEADER_NODE_96BF1757_0A3E_46F0_94CB_A7AFC4877775


#include <string>
#include <list>

#include "iridium/convertion/convert.h"
#include "iridium/pattern/prototype.h"
#include "iridium/smart_ptr.h"


namespace iridium::parsing {


// todo: Depth-First Search, DFS) and (Breadth-First Search, BFS) methods maybe parallel
/// \~english @brief Generic interface for a node in a tree-like data structure.
/// \~english @details This template class defines the common operations for a node, such as accessing its name, value, and children. It is designed to be cloneable via the IPrototype interface.
/// \~russian @brief Обобщенный интерфейс для узла в древовидной структуре данных.
/// \~russian @details Этот шаблонный класс определяет общие операции для узла, такие как доступ к его имени, значению и дочерним элементам. Он спроектирован для клонирования через интерфейс IPrototype.
template<typename TValue>
class INodeType : public pattern::IPrototype<INodeType<TValue>>
{
public:
    DEFINE_INTERFACE(INodeType)

    // list is faster then deque and vector
    typedef std::list<typename INodeType::TSharedPtr>
        TNodesSharedPtr;
    typedef std::list<INodeType<TValue> *>
        TNodesPtr;

    typedef typename TNodesSharedPtr::iterator
        iterator;
    typedef typename TNodesSharedPtr::const_iterator
        const_iterator;
    typedef typename TNodesSharedPtr::const_reference
        const_reference;
    /// \~english @brief Gets the list of parent nodes.
    /// \~russian @brief Возвращает список родительских узлов.
    virtual typename INodeType::TNodesPtr
        getParents() const = 0;

    /// \~english @brief Gets the name of the node.
    /// \~russian @brief Возвращает имя узла.
    virtual std::string
        getName() const = 0;
    /// \~english @brief Gets the value of the node.
    /// \~russian @brief Возвращает значение узла.
    virtual TValue
        getValue() const = 0;
    /// \~english @brief Sets the value of the node.
    /// \~russian @brief Устанавливает значение узла.
    virtual void
        setValue(TValue const &value) = 0;
    /// \~english @brief Gets the type of the node.
    /// \~russian @brief Возвращает тип узла.
    virtual std::string
        getType() const = 0;
    /// \~english @brief Slices the tree based on the provided path.
    /// \~russian @brief Срезает дерево на основе предоставленного пути.
    virtual typename INodeType::TNodesSharedPtr
        slice(std::string const &path) const = 0;
    /// \~english @brief Returns an iterator to the beginning of the children list.
    /// \~russian @brief Возвращает итератор на начало списка дочерних узлов.
    virtual iterator
        begin() = 0;
    /// \~english @brief Returns an iterator to the end of the children list.
    /// \~russian @brief Возвращает итератор на конец списка дочерних узлов.
    virtual iterator
        end() = 0;
    /// \~english @brief Returns a constant iterator to the beginning of the children list.
    /// \~russian @brief Возвращает константный итератор на начало списка дочерних узлов.
    virtual const_iterator
        begin() const = 0;
    /// \~english @brief Returns a constant iterator to the end of the children list.
    /// \~russian @brief Возвращает константный итератор на конец списка дочерних узлов.
    virtual const_iterator
        end() const = 0;
    /// \~english @brief Gets the number of children.
    /// \~russian @brief Возвращает количество дочерних элементов.
    virtual size_t
        size() const = 0;
    /// \~english @brief Checks if the node has children.
    /// \~russian @brief Проверяет, есть ли у узла дочерние элементы.
    virtual bool
        hasChilds() const = 0;

    /// \~english @brief Gets a child node by name.
    /// \~russian @brief Возвращает дочерний узел по имени.
    virtual typename INodeType<TValue>::TSharedPtr
        getChild(std::string const &name) = 0;
    virtual typename INodeType<TValue>::TConstSharedPtr
        getChild(std::string const &name) const = 0;
    /// \~english @brief Gets all child nodes with a specific name.
    /// \~russian @brief Возвращает все дочерние узлы с определенным именем.
    virtual TNodesSharedPtr
        getChilds(std::string const &name) const = 0;
    /// \~english @brief Adds a child node.
    /// \~russian @brief Добавляет дочерний узел.
    virtual typename INodeType::TSharedPtr
        addChild(typename INodeType::TSharedPtr const &child_node) = 0;
    virtual typename INodeType::TSharedPtr
        addChild(std::string const &name) = 0;
    virtual typename INodeType::TSharedPtr
        addChild(std::string const &name, TValue const &value) = 0;
    /// \~english @brief Deletes child nodes by name.
    /// \~russian @brief Удаляет дочерние узлы по имени.
    virtual void
        delChilds(std::string const &name) = 0;
};


} // namespace iridium::parsing


#endif // HEADER_NODE_96BF1757_0A3E_46F0_94CB_A7AFC4877775
