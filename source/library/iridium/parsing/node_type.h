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
/// \~english @brief Interface for a generic node in a hierarchical data structure.
/// \~russian @brief Интерфейс для универсального узла в иерархической структуре данных.
template<typename TValue>
class INodeType : public pattern::IPrototype<INodeType<TValue> > {
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
    /// \~english @brief Returns a list of parent nodes.
    /// \~russian @brief Возвращает список родительских узлов.
    virtual typename INodeType::TNodesPtr
        getParents() const = 0;

    /// \~english @brief Returns the name of the node.
    /// \~russian @brief Возвращает имя узла.
    virtual std::string
        getName() const = 0;
    /// \~english @brief Returns the value associated with the node.
    /// \~russian @brief Возвращает значение, связанное с узлом.
    virtual TValue
        getValue() const = 0;
    /// \~english @brief Sets the value associated with the node.
    /// \~russian @brief Устанавливает значение, связанное с узлом.
    virtual void
        setValue(TValue const &value) = 0;
    /// \~english @brief Returns the type of the node.
    /// \~russian @brief Возвращает тип узла.
    virtual std::string
        getType() const = 0;
    /// \~english @brief Finds nodes matching a path.
    /// \~russian @brief Находит узлы, соответствующие пути.
    virtual typename INodeType::TNodesSharedPtr
        slice(std::string const &path) const = 0;
    virtual iterator
        begin() = 0;
    virtual iterator
        end() = 0;
    virtual const_iterator
        begin() const = 0;
    virtual const_iterator
        end() const = 0;
    /// \~english @brief Returns the number of child nodes.
    /// \~russian @brief Возвращает количество дочерних узлов.
    virtual size_t
        size() const = 0;
    /// \~english @brief Returns true if the node has children.
    /// \~russian @brief Возвращает true, если у узла есть дочерние элементы.
    virtual bool
        hasChilds() const = 0;

    /// \~english @brief Returns a single child node by name.
    /// \~russian @brief Возвращает один дочерний узел по имени.
    virtual typename INodeType<TValue>::TSharedPtr
        getChild(std::string const &name) = 0;
    /// \~english @brief Returns a single constant child node by name.
    /// \~russian @brief Возвращает один константный дочерний узел по имени.
    virtual typename INodeType<TValue>::TConstSharedPtr
        getChild(std::string const &name) const = 0;
    /// \~english @brief Returns all child nodes with a specific name.
    /// \~russian @brief Возвращает все дочерние узлы с определенным именем.
    virtual TNodesSharedPtr
        getChilds(std::string const &name) const = 0;
    /// \~english @brief Adds an existing node as a child.
    /// \~russian @brief Добавляет существующий узел в качестве дочернего.
    virtual typename INodeType::TSharedPtr
        addChild(typename INodeType::TSharedPtr const &child_node) = 0;
    /// \~english @brief Creates and adds a new child node by name.
    /// \~russian @brief Создает и добавляет новый дочерний узел по имени.
    virtual typename INodeType::TSharedPtr
        addChild(std::string const &name) = 0;
    /// \~english @brief Creates and adds a new child node with a name and value.
    /// \~russian @brief Создает и добавляет новый дочерний узел с именем и значением.
    virtual typename INodeType::TSharedPtr
        addChild(std::string const &name, TValue const &value) = 0;
    /// \~english @brief Deletes all child nodes with a specific name.
    /// \~russian @brief Удаляет все дочерние узлы с определенным именем.
    virtual void
        delChilds(std::string const &name) = 0;
};


} // namespace iridium::parsing


#endif // HEADER_NODE_96BF1757_0A3E_46F0_94CB_A7AFC4877775
