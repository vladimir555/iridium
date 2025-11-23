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
    /// \~english @brief A list of shared pointers to nodes.
    /// \~russian @brief Список умных указателей на узлы.
    typedef std::list<typename INodeType::TSharedPtr>   TNodesSharedPtr;
    /// \~english @brief A list of raw pointers to nodes.
    /// \~russian @brief Список сырых указателей на узлы.
    typedef std::list<INodeType<TValue> *>              TNodesPtr;
    /// \~english @brief Iterator for a list of nodes.
    /// \~russian @brief Итератор для списка узлов.
    typedef typename TNodesSharedPtr::iterator          iterator;
    /// \~english @brief Const iterator for a list of nodes.
    /// \~russian @brief Константный итератор для списка узлов.
    typedef typename TNodesSharedPtr::const_iterator    const_iterator;
    /// \~english @brief Const reference to a node.
    /// \~russian @brief Константная ссылка на узел.
    typedef typename TNodesSharedPtr::const_reference   const_reference;

    /// \~english @brief Gets the parent nodes of this node.
    /// \~russian @brief Возвращает родительские узлы этого узла.
    virtual typename INodeType::TNodesPtr getParents() const = 0;

    /// \~english @brief Gets the name of the node.
    /// \~russian @brief Возвращает имя узла.
    virtual std::string     getName() const = 0;
    /// \~english @brief Gets the value of the node.
    /// \~russian @brief Возвращает значение узла.
    virtual TValue          getValue() const = 0;
    /// \~english @brief Sets the value of the node.
    /// \~russian @brief Устанавливает значение узла.
    virtual void            setValue(TValue const &value) = 0;
    /// \~english @brief Gets the type of the node.
    /// \~russian @brief Возвращает тип узла.
    virtual std::string     getType() const = 0;
    /// \~english @brief Slices the node tree based on a path.
    /// \~russian @brief Выполняет срез дерева узлов по указанному пути.
    virtual typename INodeType::TNodesSharedPtr slice(std::string const &path) const = 0;
    ///// todo:
    //virtual typename INodeType::TNodesConst findChilds(std::string const &path) const = 0;
    /// \~english @brief Returns an iterator to the beginning of the children list.
    /// \~russian @brief Возвращает итератор на начало списка дочерних узлов.
    virtual iterator        begin() = 0;
    /// \~english @brief Returns an iterator to the end of the children list.
    /// \~russian @brief Возвращает итератор на конец списка дочерних узлов.
    virtual iterator        end() = 0;
    /// \~english @brief Returns a const iterator to the beginning of the children list.
    /// \~russian @brief Возвращает константный итератор на начало списка дочерних узлов.
    virtual const_iterator  begin()     const = 0;
    /// \~english @brief Returns a const iterator to the end of the children list.
    /// \~russian @brief Возвращает константный итератор на конец списка дочерних узлов.
    virtual const_iterator  end()       const = 0;
    /// \~english @brief Returns the number of child nodes.
    /// \~russian @brief Возвращает количество дочерних узлов.
    virtual size_t          size()      const = 0;
    /// \~english @brief Checks if the node has any children.
    /// \~russian @brief Проверяет, есть ли у узла дочерние узлы.
    virtual bool            hasChilds() const = 0;

    /// \~english @brief Gets the first child node with the specified name.
    /// \~russian @brief Возвращает первый дочерний узел с указанным именем.
    virtual typename INodeType<TValue>::TSharedPtr      getChild(std::string const &name) = 0;
    /// \~english @brief Gets the first child node with the specified name (const version).
    /// \~russian @brief Возвращает первый дочерний узел с указанным именем (константная версия).
    virtual typename INodeType<TValue>::TConstSharedPtr getChild(std::string const &name) const = 0;
    /// \~english @brief Gets all child nodes with the specified name.
    /// \~russian @brief Возвращает все дочерние узлы с указанным именем.
    virtual          TNodesSharedPtr                    getChilds(std::string const &name) const = 0;
    /// \~english @brief Adds a child node.
    /// \~russian @brief Добавляет дочерний узел.
    virtual typename INodeType::TSharedPtr              addChild(typename INodeType::TSharedPtr const &child_node) = 0;
    /// \~english @brief Adds a child node with the specified name.
    /// \~russian @brief Добавляет дочерний узел с указанным именем.
    virtual typename INodeType::TSharedPtr              addChild(std::string const &name) = 0;
    /// \~english @brief Adds a child node with the specified name and value.
    /// \~russian @brief Добавляет дочерний узел с указанным именем и значением.
    virtual typename INodeType::TSharedPtr              addChild(std::string const &name, TValue const &value) = 0;
    /// \~english @brief Deletes all child nodes with the specified name.
    /// \~russian @brief Удаляет все дочерние узлы с указанным именем.
    virtual void                                        delChilds(std::string const &name) = 0;
};


} // namespace iridium::parsing


#endif // HEADER_NODE_96BF1757_0A3E_46F0_94CB_A7AFC4877775
