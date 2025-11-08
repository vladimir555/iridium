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


namespace iridium {
namespace parsing {


// todo: Depth-First Search, (DFS) and (Breadth-First Search, BFS) methods maybe parallel
/// \~english @brief The INodeType class defines a generic interface for a node in a tree-like data structure.
/// \~russian @brief Класс INodeType определяет общий интерфейс для узла в древовидной структуре данных.
template<typename TValue>
class INodeType : public pattern::IPrototype<INodeType<TValue>>
{
public:
    DEFINE_INTERFACE(INodeType)
    /// \~english @brief A list of shared pointers to nodes.
    /// \~russian @brief Список разделяемых указателей на узлы.
    typedef std::list<typename INodeType::TSharedPtr> TNodesSharedPtr;
    /// \~english @brief A list of raw pointers to nodes.
    /// \~russian @brief Список "сырых" указателей на узлы.
    typedef std::list<INodeType<TValue>*> TNodesPtr;
    /// \~english @brief Iterator for a list of shared pointers to nodes.
    /// \~russian @brief Итератор для списка разделяемых указателей на узлы.
    typedef typename TNodesSharedPtr::iterator iterator;
    /// \~english @brief Constant iterator for a list of shared pointers to nodes.
    /// \~russian @brief Константный итератор для списка разделяемых указателей на узлы.
    typedef typename TNodesSharedPtr::const_iterator const_iterator;
    /// \~english @brief Constant reference to a shared pointer to a node.
    /// \~russian @brief Константная ссылка на разделяемый указатель на узел.
    typedef typename TNodesSharedPtr::const_reference const_reference;
    /// \~english @brief Gets the parent nodes of the current node.
    /// \~russian @brief Возвращает родительские узлы текущего узла.
    virtual typename INodeType::TNodesPtr getParents() const = 0;
    /// \~english @brief Gets the name of the node.
    /// \~russian @brief Возвращает имя узла.
    virtual std::string getName() const = 0;
    /// \~english @brief Gets the value of the node.
    /// \~russian @brief Возвращает значение узла.
    virtual TValue getValue() const = 0;
    /// \~english @brief Sets the value of the node.
    /// \~russian @brief Устанавливает значение узла.
    virtual void setValue(TValue const &value) = 0;
    /// \~english @brief Gets the type of the node.
    /// \~russian @brief Возвращает тип узла.
    virtual std::string getType() const = 0;
    /// \~english @brief Slices the node structure based on a path.
    /// \~russian @brief "Срезает" структуру узла на основе пути.
    virtual typename INodeType::TNodesSharedPtr slice(std::string const &path) const = 0;
    ///// todo:
    //virtual typename INodeType::TNodesConst findChilds(std::string const &path) const = 0;
    /// \~english @brief Returns an iterator to the beginning of the child nodes.
    /// \~russian @brief Возвращает итератор на начало дочерних узлов.
    virtual iterator begin() = 0;
    /// \~english @brief Returns an iterator to the end of the child nodes.
    /// \~russian @brief Возвращает итератор на конец дочерних узлов.
    virtual iterator end() = 0;
    /// \~english @brief Returns a constant iterator to the beginning of the child nodes.
    /// \~russian @brief Возвращает константный итератор на начало дочерних узлов.
    virtual const_iterator begin() const = 0;
    /// \~english @brief Returns a constant iterator to the end of the child nodes.
    /// \~russian @brief Возвращает константный итератор на конец дочерних узлов.
    virtual const_iterator end() const = 0;
    /// \~english @brief Gets the number of child nodes.
    /// \~russian @brief Возвращает количество дочерних узлов.
    virtual size_t size() const = 0;
    /// \~english @brief Checks if the node has child nodes.
    /// \~russian @brief Проверяет, есть ли у узла дочерние узлы.
    virtual bool hasChilds() const = 0;

    /// \~english @brief Gets a child node by name.
    /// \~russian @brief Возвращает дочерний узел по имени.
    virtual typename INodeType<TValue>::TSharedPtr getChild(std::string const &name) = 0;
    /// \~english @brief Gets a constant child node by name.
    /// \~russian @brief Возвращает константный дочерний узел по имени.
    virtual typename INodeType<TValue>::TConstSharedPtr getChild(std::string const &name) const = 0;
    /// \~english @brief Gets all child nodes with a specific name.
    /// \~russian @brief Возвращает все дочерние узлы с указанным именем.
    virtual TNodesSharedPtr getChilds(std::string const &name) const = 0;
    /// \~english @brief Adds a child node.
    /// \~russian @brief Добавляет дочерний узел.
    virtual typename INodeType::TSharedPtr addChild(typename INodeType::TSharedPtr const &child_node) = 0;
    /// \~english @brief Adds a child node with a specific name.
    /// \~russian @brief Добавляет дочерний узел с указанным именем.
    virtual typename INodeType::TSharedPtr addChild(std::string const &name) = 0;
    /// \~english @brief Adds a child node with a specific name and value.
    /// \~russian @brief Добавляет дочерний узел с указанным именем и значением.
    virtual typename INodeType::TSharedPtr addChild(std::string const &name, TValue const &value) = 0;
    /// \~english @brief Deletes all child nodes with a specific name.
    /// \~russian @brief Удаляет все дочерние узлы с указанным именем.
    virtual void delChilds(std::string const &name) = 0;
};


} // parsing
} // iridium


#endif // HEADER_NODE_96BF1757_0A3E_46F0_94CB_A7AFC4877775
