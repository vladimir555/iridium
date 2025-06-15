// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NODE_94200784_8C23_4200_B54C_65736B455736
#define HEADER_NODE_94200784_8C23_4200_B54C_65736B455736


#include <string>
#include <stdexcept>

#include "iridium/parsing/node.h"
#include "iridium/parsing/implementation/node.h"
#include "iridium/macros/va_args.h"


namespace iridium {
namespace parsing {
/// \~english @brief Provides classes, macros, and utilities for defining serializable/deserializable data structures.
///     This namespace focuses on a node-based representation where data structures are defined
///     as a hierarchy of nodes, each potentially having attributes, values, and child nodes or lists of nodes.
///     The system uses `NodeView` classes as wrappers around an underlying `INode` tree to provide
///     typed access and modification, and employs a suite of macros (`DEFINE_ROOT_NODE_BEGIN`, `DEFINE_ATTRIBUTE`, etc.)
///     to declaratively define these structures.
/// \~russian @brief Предоставляет классы, макросы и утилиты для определения сериализуемых/десериализуемых структур данных.
///     Это пространство имен фокусируется на представлении данных на основе узлов, где структуры данных определяются
///     как иерархия узлов, каждый из которых потенциально может иметь атрибуты, значения и дочерние узлы или списки узлов.
///     Система использует классы `NodeView` в качестве оберток над базовым деревом `INode` для обеспечения
///     типизированного доступа и модификации, а также применяет набор макросов (`DEFINE_ROOT_NODE_BEGIN`, `DEFINE_ATTRIBUTE` и т.д.)
///     для декларативного определения этих структур.
namespace serialization {


// ----- interface


// todo: copy constructor; rm TType; fix List property name

/// \~english @brief Provides a typed view or wrapper around an underlying `INode` object.
///     This class template facilitates typed access (get/set) to the value of a node
///     and manages its relationship within a larger node structure (via path and parent).
///     It's a fundamental building block for attributes and simple value-holding nodes
///     defined using the serialization macros.
/// \~russian @brief Предоставляет типизированное представление или обертку над базовым объектом `INode`.
///     Этот шаблон класса облегчает типизированный доступ (получение/установка) к значению узла
///     и управляет его связью в рамках более крупной структуры узлов (через путь и родителя).
///     Является фундаментальным строительным блоком для атрибутов и простых узлов, содержащих значения,
///     определяемых с помощью макросов сериализации.
/// \~english @tparam TValue The expected type of the node's value. Conversions will be attempted.
/// \~russian @tparam TValue Ожидаемый тип значения узла. Будут предприняты попытки преобразования.
template<typename TValue>
class NodeView {
public:
    /// \~english @brief Default destructor.
    /// \~russian @brief Деструктор по умолчанию.
   ~NodeView() = default;
    /// \~english @brief Gets the value of the node, converting it to `TValue`.
    ///     Note: A `todo` in the source suggests lazy conversion with caching could be implemented.
    /// \~russian @brief Получает значение узла, преобразуя его к типу `TValue`.
    ///     Примечание: `todo` в исходном коде предполагает возможную реализацию ленивого преобразования с кэшированием.
    /// \~english @return The node's value as `TValue`.
    /// \~russian @return Значение узла как `TValue`.
    /// \~english @exception std::runtime_error if the node is not found or value conversion fails.
    /// \~russian @exception std::runtime_error если узел не найден или происходит сбой преобразования значения.
    TValue get() const;
    /// \~english @brief Sets the value of the node, converting from `TValue` to string for storage.
    /// \~russian @brief Устанавливает значение узла, преобразуя из `TValue` в строку для хранения.
    /// \~english @param value The value to set.
    /// \~russian @param value Устанавливаемое значение.
    void set(TValue const &value);
    /// \~english @brief Checks if the node's current value is the default value (if one was provided during construction).
    /// \~russian @brief Проверяет, является ли текущее значение узла значением по умолчанию (если оно было предоставлено при конструировании).
    /// \~english @return True if the value is the default, false otherwise.
    /// \~russian @return True, если значение является значением по умолчанию, иначе false.
    bool isDefault() const;
    /// \~english @brief Conversion operator to get the node's value. Equivalent to calling `get()`.
    /// \~russian @brief Оператор преобразования для получения значения узла. Эквивалентен вызову `get()`.
    /// \~english @return The node's value as `TValue`.
    /// \~russian @return Значение узла как `TValue`.
    operator TValue() const;
    /// \~english @brief Assignment operator to set the node's value. Equivalent to calling `set()`.
    /// \~russian @brief Оператор присваивания для установки значения узла. Эквивалентен вызову `set()`.
    /// \~english @param value The value to assign.
    /// \~russian @param value Присваиваемое значение.
    /// \~english @return Reference to this `NodeView` object.
    /// \~russian @return Ссылка на этот объект `NodeView`.
    NodeView &operator= (TValue const &value);
    /// \~english @brief Equality comparison operator. Compares the node's value with the given value.
    /// \~russian @brief Оператор сравнения на равенство. Сравнивает значение узла с заданным значением.
    /// \~english @param value The value to compare against.
    /// \~russian @param value Значение для сравнения.
    /// \~english @return True if the node's value is equal to `value`, false otherwise.
    /// \~russian @return True, если значение узла равно `value`, иначе false.
    bool operator== (TValue const &value) const;
    /// \~english @brief Gets the underlying `INode` shared pointer.
    /// \~russian @brief Получает базовый разделяемый указатель `INode`.
    /// \~english @return A shared pointer to the `INode` this view wraps.
    /// \~russian @return Разделяемый указатель на `INode`, который оборачивает это представление.
    INode::TSharedPtr getNode() const;

protected:
    /// \~english @brief Constructor for `NodeView` representing an attribute within a list item.
    ///     Used by `DEFINE_ATTRIBUTE_LIST`.
    /// \~russian @brief Конструктор для `NodeView`, представляющего атрибут элемента списка.
    ///     Используется `DEFINE_ATTRIBUTE_LIST`.
    /// \~english @param node The `INode` of the parent list item.
    /// \~russian @param node `INode` родительского элемента списка.
    /// \~english @param name The name of the attribute.
    /// \~russian @param name Имя атрибута.
    /// \~english @param path The base path to the parent list item.
    /// \~russian @param path Базовый путь к родительскому элементу списка.
    NodeView(
        INode::TSharedPtr       const &node,
        std::string             const &name,
        std::string             const &path);

    /// \~english @brief Constructor for a root `NodeView` (e.g., for an attribute list defined at the root).
    /// \~russian @brief Конструктор для корневого `NodeView` (например, для списка атрибутов, определенного в корне).
    /// \~english @param name The name of the root node.
    /// \~russian @param name Имя корневого узла.
    NodeView(std::string const &name);
    /// \~english @brief Constructor for an attribute `NodeView` with a default value, associated with a parent node.
    /// \~russian @brief Конструктор для атрибута `NodeView` со значением по умолчанию, связанного с родительским узлом.
    /// \~english @param parent The parent `NodeView<void>` object.
    /// \~russian @param parent Родительский объект `NodeView<void>`.
    /// \~english @param name The name of the attribute.
    /// \~russian @param name Имя атрибута.
    /// \~english @param default_value The default value for this attribute if not found in the underlying `INode`.
    /// \~russian @param default_value Значение по умолчанию для этого атрибута, если он не найден в базовом `INode`.
    NodeView(NodeView<void> const &parent, std::string const &name, TValue const &default_value);
    /// \~english @brief Constructor for a mandatory attribute `NodeView` (no default value), associated with a parent node.
    /// \~russian @brief Конструктор для обязательного атрибута `NodeView` (без значения по умолчанию), связанного с родительским узлом.
    /// \~english @param parent The parent `NodeView<void>` object.
    /// \~russian @param parent Родительский объект `NodeView<void>`.
    /// \~english @param name The name of the attribute.
    /// \~russian @param name Имя атрибута.
    NodeView(NodeView<void> const &parent, std::string const &name);

    /// \~english @brief Shared pointer to the underlying generic `INode` that this view wraps.
    /// \~russian @brief Разделяемый указатель на базовый обобщенный `INode`, который это представление оборачивает.
    INode::TSharedPtr       m_node;
    /// \~english @brief The full path to this node/attribute from the root of the structure.
    /// \~russian @brief Полный путь к этому узлу/атрибуту от корня структуры.
    std::string             m_path;
    /// \~english @brief Flag indicating if the current value is a default value (set if the node was missing and a default was provided).
    /// \~russian @brief Флаг, указывающий, является ли текущее значение значением по умолчанию (устанавливается, если узел отсутствовал и было предоставлено значение по умолчанию).
    bool                    m_is_default = false;
};

/// \~english @brief Specialization of `NodeView` for typeless nodes (`void` value type).
///     This is typically used as a base class for structured nodes defined by `DEFINE_NODE_BEGIN` or `DEFINE_ROOT_NODE_BEGIN`,
///     or to represent a parent node whose specific type is not needed for path construction.
///     It primarily manages the `INode` pointer and its path.
/// \~russian @brief Специализация `NodeView` для безузловых узлов (тип значения `void`).
///     Обычно используется как базовый класс для структурированных узлов, определенных `DEFINE_NODE_BEGIN` или `DEFINE_ROOT_NODE_BEGIN`,
///     или для представления родительского узла, конкретный тип которого не нужен для построения пути.
///     В основном управляет указателем `INode` и его путем.
template<>
class NodeView<void> {
public:
    /// \~english @brief Constructor for a root node view, intended primarily for deserialization where the node is yet to be created/parsed.
    /// \~russian @brief Конструктор для представления корневого узла, предназначенный в основном для десериализации, когда узел еще не создан/разобран.
    /// \~english @param name The expected name of the root node.
    /// \~russian @param name Ожидаемое имя корневого узла.
    NodeView(std::string const &name);
    /// \~english @brief Constructor for a root node view, initialized with an existing `INode`. Used for serialization or further operations.
    /// \~russian @brief Конструктор для представления корневого узла, инициализированный существующим `INode`. Используется для сериализации или дальнейших операций.
    /// \~english @param node Shared pointer to the existing root `INode`.
    /// \~russian @param node Разделяемый указатель на существующий корневой `INode`.
    /// \~english @param name The name of the root node (should match `node->getName()`).
    /// \~russian @param name Имя корневого узла (должно совпадать с `node->getName()`).
    NodeView(
         INode::TSharedPtr  const &node,
         std::string        const &name);
    /// \~english @brief Default destructor.
    /// \~russian @brief Деструктор по умолчанию.
   ~NodeView() = default;
    /// \~english @brief Gets the underlying `INode` shared pointer.
    /// \~russian @brief Получает базовый разделяемый указатель `INode`.
    /// \~english @return A shared pointer to the `INode` this view wraps.
    /// \~russian @return Разделяемый указатель на `INode`, который оборачивает это представление.
    INode::TSharedPtr       getNode() const;

    /// \~english @brief Shared pointer to the underlying generic `INode`.
    /// \~russian @brief Разделяемый указатель на базовый обобщенный `INode`.
    INode::TSharedPtr       m_node;
    /// \~english @brief The full path to this node from the root of the structure.
    /// \~russian @brief Полный путь к этому узлу от корня структуры.
    std::string             m_path;

protected:
    /// \~english @brief Constructor for a `NodeView<void>` representing an item in a list of complex nodes.
    ///     Used by `DEFINE_NODE_LIST_BEGIN/END`.
    /// \~russian @brief Конструктор для `NodeView<void>`, представляющего элемент в списке сложных узлов.
    ///     Используется `DEFINE_NODE_LIST_BEGIN/END`.
    /// \~english @param node The `INode` of this specific list item.
    /// \~russian @param node `INode` этого конкретного элемента списка.
    /// \~english @param name The expected name of this list item node.
    /// \~russian @param name Ожидаемое имя этого узла элемента списка.
    /// \~english @param path The base path to this list item (often the parent's path).
    /// \~russian @param path Базовый путь к этому элементу списка (часто путь родителя).
    NodeView(
        INode::TSharedPtr       const &node,
        std::string             const &name,
        std::string             const &path);
    /// \~english @brief Constructor for a nested `NodeView<void>` (a structured child node) associated with a parent.
    ///     Used by `DEFINE_NODE_BEGIN/END`.
    /// \~russian @brief Конструктор для вложенного `NodeView<void>` (структурированного дочернего узла), связанного с родителем.
    ///     Используется `DEFINE_NODE_BEGIN/END`.
    /// \~english @param parent The parent `NodeView<void>` object.
    /// \~russian @param parent Родительский объект `NodeView<void>`.
    /// \~english @param name The name of this nested node.
    /// \~russian @param name Имя этого вложенного узла.
    NodeView(NodeView<void> const &parent, std::string const &name);
};

/// \~english @brief Represents a list of `NodeView` objects of a specific type `TNodeView`.
///     This class provides an interface to iterate over and modify a list of complex child nodes
///     or a list of attributes within a data structure defined by the serialization macros.
/// \~russian @brief Представляет список объектов `NodeView` определенного типа `TNodeView`.
///     Этот класс предоставляет интерфейс для итерации и изменения списка сложных дочерних узлов
///     или списка атрибутов в структуре данных, определенной макросами сериализации.
/// \~english @tparam TNodeView The type of `NodeView` that this list contains. This is typically a struct generated by `DEFINE_NODE_LIST_BEGIN` or `DEFINE_ATTRIBUTE_LIST`.
/// \~russian @tparam TNodeView Тип `NodeView`, который содержит этот список. Обычно это структура, сгенерированная `DEFINE_NODE_LIST_BEGIN` или `DEFINE_ATTRIBUTE_LIST`.
template<typename TNodeView>
class NodeViewList {
public:
    /// \~english @brief Default destructor.
    /// \~russian @brief Деструктор по умолчанию.
   ~NodeViewList() = default;
    /// \~english @brief Typedef for the underlying list of `TNodeView` objects.
    /// \~russian @brief Typedef для базового списка объектов `TNodeView`.
    typedef typename std::list<TNodeView> TNodes;
    /// \~english @brief Iterator for traversing the list of `TNodeView` objects.
    /// \~russian @brief Итератор для обхода списка объектов `TNodeView`.
    typedef typename std::list<TNodeView>::iterator iterator;
    /// \~english @brief Const iterator for traversing the list of `TNodeView` objects.
    /// \~russian @brief Константный итератор для обхода списка объектов `TNodeView`.
    typedef typename std::list<TNodeView>::const_iterator const_iterator;

    /// \~english @brief Returns an iterator to the beginning of the list.
    /// \~russian @brief Возвращает итератор на начало списка.
    /// \~english @return An iterator to the first element.
    /// \~russian @return Итератор на первый элемент.
    iterator begin();
    /// \~english @brief Returns an iterator to the end of the list.
    /// \~russian @brief Возвращает итератор на конец списка.
    /// \~english @return An iterator referring to the past-the-end element.
    /// \~russian @return Итератор, указывающий на элемент после последнего.
    iterator end();
    /// \~english @brief Returns a const iterator to the beginning of the list.
    /// \~russian @brief Возвращает константный итератор на начало списка.
    /// \~english @return A const iterator to the first element.
    /// \~russian @return Константный итератор на первый элемент.
    const_iterator begin() const;
    /// \~english @brief Returns a const iterator to the end of the list.
    /// \~russian @brief Возвращает константный итератор на конец списка.
    /// \~english @return A const iterator referring to the past-the-end element.
    /// \~russian @return Константный итератор, указывающий на элемент после последнего.
    const_iterator end() const;
    /// \~english @brief Gets the number of items in the list.
    /// \~russian @brief Получает количество элементов в списке.
    /// \~english @return The number of `TNodeView` objects in the list.
    /// \~russian @return Количество объектов `TNodeView` в списке.
    size_t size() const;
    /// \~english @brief Adds a pre-constructed `TNodeView` object to the list and to the parent's underlying `INode`.
    /// \~russian @brief Добавляет предварительно сконструированный объект `TNodeView` в список и в базовый `INode` родителя.
    /// \~english @param node The `TNodeView` object to add.
    /// \~russian @param node Объект `TNodeView` для добавления.
    void add(TNodeView const &node);
    /// \~english @brief Adds a new item to the list by its value (for lists of simple attributes).
    ///     Creates a new `TNodeView`, sets its value, and adds it.
    /// \~russian @brief Добавляет новый элемент в список по его значению (для списков простых атрибутов).
    ///     Создает новый `TNodeView`, устанавливает его значение и добавляет его.
    /// \~english @tparam TValue The type of the value to add, typically compatible with `TNodeView`'s value type.
    /// \~russian @tparam TValue Тип добавляемого значения, обычно совместимый с типом значения `TNodeView`.
    /// \~english @param value The value to add to the list.
    /// \~russian @param value Значение для добавления в список.
    template<typename TValue>
    void add(TValue const &node);

protected:
    /// \~english @brief Constructor for `NodeViewList`. Initializes by finding all child nodes of `parent` that match `name`.
    /// \~russian @brief Конструктор для `NodeViewList`. Инициализируется путем поиска всех дочерних узлов `parent`, соответствующих `name`.
    /// \~english @param parent The parent `NodeView<void>` which contains this list.
    /// \~russian @param parent Родительский `NodeView<void>`, который содержит этот список.
    /// \~english @param name The name of the child nodes that constitute this list.
    /// \~russian @param name Имя дочерних узлов, составляющих этот список.
    NodeViewList(NodeView<void> const &parent, std::string const &name);

private:
    /// \~english @brief Const reference to the parent `NodeView<void>` that owns this list.
    /// \~russian @brief Константная ссылка на родительский `NodeView<void>`, которому принадлежит этот список.
    NodeView<void> const &m_parent;
    /// \~english @brief The internal list of `TNodeView` objects.
    /// \~russian @brief Внутренний список объектов `TNodeView`.
    std::list<TNodeView> m_nodes;
};

/// \~english @brief A wrapper for a lazily-initialized shared pointer to a `TNodeView`.
///     This is used for optional or recursive node definitions within data structures
///     defined by the serialization macros (e.g., `DEFINE_NODE_PTR`).
///     The actual `TNodeView` is typically created only when `get()` is called and the underlying node exists.
/// \~russian @brief Обертка для лениво инициализируемого разделяемого указателя на `TNodeView`.
///     Используется для опциональных или рекурсивных определений узлов в структурах данных,
///     определенных макросами сериализации (например, `DEFINE_NODE_PTR`).
///     Фактический `TNodeView` обычно создается только при вызове `get()` и если существует базовый узел.
/// \~english @tparam TNodeView The type of `NodeView` this pointer wrapper manages.
/// \~russian @tparam TNodeView Тип `NodeView`, которым управляет эта обертка указателя.
template<typename TNodeView>
class NodeViewPtr {
public:
    /// \~english @brief Constructs the `NodeViewPtr`. Attempts to initialize the internal `TNodeView` if the corresponding node exists under `parent`.
    /// \~russian @brief Конструирует `NodeViewPtr`. Пытается инициализировать внутренний `TNodeView`, если соответствующий узел существует в `parent`.
    /// \~english @param parent The parent `NodeView<void>` in which to look for the node.
    /// \~russian @param parent Родительский `NodeView<void>`, в котором производится поиск узла.
    NodeViewPtr(NodeView<void> const &parent);
    /// \~english @brief Default destructor.
    /// \~russian @brief Деструктор по умолчанию.
   ~NodeViewPtr() = default;
    /// \~english @brief Gets the shared pointer to the `TNodeView`.
    ///     If the `TNodeView` was not yet created (e.g., because the underlying node didn't exist at construction), this will return a null pointer.
    /// \~russian @brief Получает разделяемый указатель на `TNodeView`.
    ///     Если `TNodeView` еще не был создан (например, потому что базовый узел не существовал при конструировании), вернет нулевой указатель.
    /// \~english @return A shared pointer to the `TNodeView`, or nullptr if not available/initialized.
    /// \~russian @return Разделяемый указатель на `TNodeView` или nullptr, если он недоступен/не инициализирован.
    std::shared_ptr<TNodeView> get() const;

private:
    /// \~english @brief The lazily-initialized shared pointer to the actual `TNodeView`.
    /// \~russian @brief Лениво инициализируемый разделяемый указатель на фактический `TNodeView`.
    typename std::shared_ptr<TNodeView> m_node_view;
};


// ----- implementation


// ----- Node


template<typename TValue>
NodeView<TValue>::NodeView(
    INode::TSharedPtr       const &node,
    std::string             const &name,
    std::string             const &path)
{
    m_path = path + "/" + name;

    if (node && node->getName() == name)
        m_node   = node;
    else
        throw std::runtime_error("node '" + m_path + "' not found"); // ----->
}


template<typename TValue>
NodeView<TValue>::NodeView(std::string const &name) {
    m_path = "/" + name;
    m_node = implementation::CNode::create(name);
}


template<typename TValue>
INode::TSharedPtr NodeView<TValue>::getNode() const {
    return m_node; // ----->
}


template<typename TValue>
NodeView<TValue>::NodeView(NodeView<void> const &parent, std::string const &name, TValue const &default_value) {
    m_path = parent.m_path + "/" + name;

    if (parent.m_node)
        m_node          = parent.m_node->getChild(name);
    else
        m_node          = nullptr;

    if(!m_node) {
        m_node          = parent.m_node->addChild(name, convertion::convert<std::string>(default_value));
        m_is_default    = true;
    }
}


template<typename TValue>
NodeView<TValue>::NodeView(NodeView<void> const &parent, std::string const &name) {
    m_path = parent.m_path + "/" + name;

    if (parent.m_node)
        m_node  = parent.m_node->getChild(name);
    else
        m_node  = nullptr; // pvs warn

    if (!m_node) {
        throw std::runtime_error("node '" + m_path + "' not found"); // ----->
    } 
}


template<typename TValue>
TValue NodeView<TValue>::get() const {
    // todo: lazy convertion with cached value
    if (m_node)
        return convertion::convert<TValue>(m_node->getValue());
    else
        throw std::runtime_error("value " + m_path + " not serialized"); // ----->
}


template<typename TValue>
void NodeView<TValue>::set(TValue const &value) {
    if (m_node)
        m_node->setValue(convertion::convert<std::string>(value));

    m_is_default = false;
}


template<typename TValue>
bool NodeView<TValue>::isDefault() const {
    return m_is_default;
}


template<typename TValue>
NodeView<TValue>::operator TValue() const {
    return get(); // ----->
}


template<typename TValue>
NodeView<TValue> &NodeView<TValue>::operator= (TValue const &value) {
    set(value);
    return *this; // ----->
}


template<typename TValue>
bool NodeView<TValue>::operator== (TValue const &value) const {
    return get() == value; // ----->
}
// -----


// ----- NodeViewList


template<typename TNodeView>
NodeViewList<TNodeView>::NodeViewList(NodeView<void> const &parent, std::string const &name)
:
    m_parent(parent)
{
    if (parent.m_node) {
        for (auto const &i: *parent.m_node)
            if (i->getName() == name)
                m_nodes.push_back(TNodeView(i, parent.m_path));
    }
}


template<typename TNodeView>
typename NodeViewList<TNodeView>::iterator NodeViewList<TNodeView>::begin() {
    return m_nodes.begin(); // ----->
}


template<typename TNodeView>
typename NodeViewList<TNodeView>::iterator NodeViewList<TNodeView>::end() {
    return m_nodes.end(); // ----->
}


template<typename TNodeView>
typename NodeViewList<TNodeView>::const_iterator NodeViewList<TNodeView>::begin() const {
    return m_nodes.begin(); // ----->
}


template<typename TNodeView>
typename NodeViewList<TNodeView>::const_iterator NodeViewList<TNodeView>::end() const {
    return m_nodes.end(); // ----->
}


template<typename TNodeView>
size_t NodeViewList<TNodeView>::size() const {
    return m_nodes.size(); // ----->
}


template<typename TNodeView>
void NodeViewList<TNodeView>::add(TNodeView const &node) {
    m_parent.m_node->addChild(node.getNode());
    m_nodes.push_back(node);
}


template<typename TNodeView>
template<typename TValue>
void NodeViewList<TNodeView>::add(TValue const &value) {
    TNodeView node_view;
    node_view.set(value);
    add(node_view);
}
// -----


// ----- NodeList


template<typename TNodeView>
NodeViewPtr<TNodeView>::NodeViewPtr(NodeView<void> const &parent) {
    try {
        m_node_view = std::make_shared<TNodeView>(parent);
    } catch (...) {}
}


template<typename TNodeView>
std::shared_ptr<TNodeView> NodeViewPtr<TNodeView>::get() const {
    return m_node_view; // ----->
}
// -----


/// \~english @brief Converts a string from CamelCase to a delimited format (e.g., snake_case or kebab-case).
///     This is used by the serialization macros to automatically generate node names from class/struct member names.
///     For example, `MySettingName` with delimiter `'-'` becomes `"my-setting-name"`.
/// \~russian @brief Преобразует строку из CamelCase в формат с разделителями (например, snake_case или kebab-case).
///     Используется макросами сериализации для автоматической генерации имен узлов из имен членов класса/структуры.
///     Например, `MySettingName` с разделителем `'-'` превращается в `"my-setting-name"`.
/// \~english @param camel The input string in CamelCase.
/// \~russian @param camel Входная строка в CamelCase.
/// \~english @param delimeter_symbol The character to use as a delimiter between words.
/// \~russian @param delimeter_symbol Символ, используемый в качестве разделителя между словами.
/// \~english @return The converted string with words separated by the delimiter.
/// \~russian @return Преобразованная строка со словами, разделенными разделителем.
std::string convertCamelToSplittedBySymbol(std::string const &camel, char const &delimeter_symbol);


} // serialization
} // parsing
} // iridium

// todo: name_delimeter_symbol -> name_convertion_method
/// \~english @def DEFINE_ROOT_NODE_BEGIN_2(class_name, name_delimeter_symbol)
/// @brief Begins the definition of a root node structure for serialization/deserialization.
///     Generates a struct `T##class_name` that inherits from `NodeView<void>`.
///     It includes constructors for creating a new node or wrapping an existing `INode::TSharedPtr`.
///     The node name is automatically derived from `class_name` using `convertCamelToSplittedBySymbol`.
/// \~russian @def DEFINE_ROOT_NODE_BEGIN_2(class_name, name_delimeter_symbol)
/// @brief Начинает определение структуры корневого узла для сериализации/десериализации.
///     Генерирует структуру `T##class_name`, наследующую от `NodeView<void>`.
///     Включает конструкторы для создания нового узла или обертывания существующего `INode::TSharedPtr`.
///     Имя узла автоматически выводится из `class_name` с помощью `convertCamelToSplittedBySymbol`.
/// \~english @param class_name The base name for the generated struct (e.g., `MyConfig` results in `TMyConfig`).
/// \~russian @param class_name Базовое имя для генерируемой структуры (например, `MyConfig` приведет к `TMyConfig`).
/// \~english @param name_delimeter_symbol The delimiter character used to convert `class_name` from CamelCase to the actual node name.
/// \~russian @param name_delimeter_symbol Символ-разделитель, используемый для преобразования `class_name` из CamelCase в фактическое имя узла.
#define DEFINE_ROOT_NODE_BEGIN_2(class_name, name_delimeter_symbol) \
    struct T##class_name : protected iridium::parsing::serialization::NodeView<void> { \
        static int const NAME_DELIMETER_SYMBOL = name_delimeter_symbol; \
        T##class_name(iridium::parsing::INode::TSharedPtr const &node): \
            iridium::parsing::serialization::NodeView<void> \
                (node, iridium::parsing::serialization::convertCamelToSplittedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
            T##class_name(): iridium::parsing::serialization::NodeView<void> \
                (iridium::parsing::serialization::convertCamelToSplittedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
            iridium::parsing::INode::TSharedPtr getNode() const { \
                return iridium::parsing::serialization::NodeView<void>::getNode(); \
            } \
        T##class_name(iridium::parsing::serialization::NodeView<void> const &parent): \
            iridium::parsing::serialization::NodeView<void> \
                (parent, iridium::parsing::serialization::convertCamelToSplittedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {}

/// \~english @def DEFINE_ROOT_NODE_BEGIN_1(class_name)
/// @brief Begins the definition of a root node structure with a default name delimiter (`-`).
///     This is a convenience macro that calls `DEFINE_ROOT_NODE_BEGIN_2` with `'-'` as the delimiter.
/// \~russian @def DEFINE_ROOT_NODE_BEGIN_1(class_name)
/// @brief Начинает определение структуры корневого узла с разделителем имени по умолчанию (`-`).
///     Это вспомогательный макрос, который вызывает `DEFINE_ROOT_NODE_BEGIN_2` с `'-'` в качестве разделителя.
/// \~english @param class_name The base name for the generated struct.
/// \~russian @param class_name Базовое имя для генерируемой структуры.
#define DEFINE_ROOT_NODE_BEGIN_1(class_name) \
    DEFINE_ROOT_NODE_BEGIN_2(class_name, '-')

/// \~english @def DEFINE_ROOT_NODE_BEGIN(...)
/// @brief Chooser macro that dispatches to `DEFINE_ROOT_NODE_BEGIN_1` or `DEFINE_ROOT_NODE_BEGIN_2`
///     based on the number of arguments provided. Allows defining a root node with either a default
///     name delimiter or a custom one.
/// \~russian @def DEFINE_ROOT_NODE_BEGIN(...)
/// @brief Макрос-переключатель, который диспетчеризует вызов в `DEFINE_ROOT_NODE_BEGIN_1` или `DEFINE_ROOT_NODE_BEGIN_2`
///     в зависимости от количества предоставленных аргументов. Позволяет определить корневой узел
///     либо с разделителем имени по умолчанию, либо с пользовательским.
#define DEFINE_ROOT_NODE_BEGIN(...) \
    DEFINE_MACRO_CHOOSER(DEFINE_ROOT_NODE_BEGIN, __VA_ARGS__)(__VA_ARGS__)

/// \~english @def DEFINE_ROOT_NODE_END()
/// @brief Ends the definition of a root node structure started by `DEFINE_ROOT_NODE_BEGIN`.
///     Simply provides the closing brace for the generated struct.
/// \~russian @def DEFINE_ROOT_NODE_END()
/// @brief Завершает определение структуры корневого узла, начатое `DEFINE_ROOT_NODE_BEGIN`.
///     Просто предоставляет закрывающую фигурную скобку для генерируемой структуры.
#define DEFINE_ROOT_NODE_END() \
    };

/// \~english @def DEFINE_NODE_BEGIN(class_name)
/// @brief Begins the definition of a nested structured node within a parent node structure.
///     Generates a struct `T##class_name` that inherits from `NodeView<void>`.
///     This struct is initialized with a reference to its parent `NodeView<void>`.
///     The node name is derived from `class_name` using the parent's `NAME_DELIMETER_SYMBOL`.
/// \~russian @def DEFINE_NODE_BEGIN(class_name)
/// @brief Начинает определение вложенного структурированного узла внутри родительской структуры узла.
///     Генерирует структуру `T##class_name`, наследующую от `NodeView<void>`.
///     Эта структура инициализируется ссылкой на ее родительский `NodeView<void>`.
///     Имя узла выводится из `class_name` с использованием `NAME_DELIMETER_SYMBOL` родителя.
/// \~english @param class_name The base name for the generated struct member (e.g., `NestedData` results in `TNestedData NestedData;`).
/// \~russian @param class_name Базовое имя для генерируемого члена структуры (например, `NestedData` приведет к `TNestedData NestedData;`).
#define DEFINE_NODE_BEGIN(class_name) \
    struct T##class_name : protected iridium::parsing::serialization::NodeView<void> { \
        T##class_name(iridium::parsing::serialization::NodeView<void> const &parent): \
        iridium::parsing::serialization::NodeView<void> \
        (parent, iridium::parsing::serialization::convertCamelToSplittedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {}

/// \~english @def DEFINE_NODE_END(class_name)
/// @brief Ends the definition of a nested structured node and declares an instance of it.
///     Provides the closing brace for the struct defined by `DEFINE_NODE_BEGIN` and
///     declares a member variable of that struct type, named `class_name`, initialized with `*this` (the parent).
/// \~russian @def DEFINE_NODE_END(class_name)
/// @brief Завершает определение вложенного структурированного узла и объявляет его экземпляр.
///     Предоставляет закрывающую фигурную скобку для структуры, определенной `DEFINE_NODE_BEGIN`, и
///     объявляет переменную-член этого типа структуры с именем `class_name`, инициализированную `*this` (родителем).
/// \~english @param class_name The name of the member variable to be declared (should match the `class_name` in `DEFINE_NODE_BEGIN`).
/// \~russian @param class_name Имя объявляемой переменной-члена (должно совпадать с `class_name` в `DEFINE_NODE_BEGIN`).
#define DEFINE_NODE_END(class_name) \
    } class_name = *this;

/// \~english @def DEFINE_ATTRIBUTE_2(type, class_name)
/// @brief Defines a mandatory attribute (a simple value node) within a parent node structure.
///     Generates a struct `T##class_name` that inherits from `NodeView<type>` and declares a member
///     variable `class_name` of this struct type. The attribute name is derived from `class_name`.
/// \~russian @def DEFINE_ATTRIBUTE_2(type, class_name)
/// @brief Определяет обязательный атрибут (простой узел-значение) внутри родительской структуры узла.
///     Генерирует структуру `T##class_name`, наследующую от `NodeView<type>`, и объявляет член-переменную
///     `class_name` этого типа структуры. Имя атрибута выводится из `class_name`.
/// \~english @param type The data type of the attribute (e.g., `std::string`, `int`, `bool`).
/// \~russian @param type Тип данных атрибута (например, `std::string`, `int`, `bool`).
/// \~english @param class_name The name of the member variable and base for the attribute node name.
/// \~russian @param class_name Имя переменной-члена и база для имени узла атрибута.
#define DEFINE_ATTRIBUTE_2(type, class_name) \
    struct T##class_name : public iridium::parsing::serialization::NodeView<type> { \
        T##class_name(iridium::parsing::serialization::NodeView<void> const &parent): \
        iridium::parsing::serialization::NodeView<type> \
        (parent, iridium::parsing::serialization::convertCamelToSplittedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
        using iridium::parsing::serialization::NodeView<type>::operator =; \
    } class_name = *this;

/// \~english @def DEFINE_ATTRIBUTE_3(type, class_name, default_value)
/// @brief Defines an optional attribute with a default value within a parent node structure.
///     Similar to `DEFINE_ATTRIBUTE_2` but provides a default value if the attribute is not present in the source.
/// \~russian @def DEFINE_ATTRIBUTE_3(type, class_name, default_value)
/// @brief Определяет опциональный атрибут со значением по умолчанию внутри родительской структуры узла.
///     Аналогичен `DEFINE_ATTRIBUTE_2`, но предоставляет значение по умолчанию, если атрибут отсутствует в источнике.
/// \~english @param type The data type of the attribute.
/// \~russian @param type Тип данных атрибута.
/// \~english @param class_name The name of the member variable and base for the attribute node name.
/// \~russian @param class_name Имя переменной-члена и база для имени узла атрибута.
/// \~english @param default_value The default value for the attribute.
/// \~russian @param default_value Значение по умолчанию для атрибута.
#define DEFINE_ATTRIBUTE_3(type, class_name, default_value) \
    struct T##class_name : public iridium::parsing::serialization::NodeView<type> { \
        T##class_name(iridium::parsing::serialization::NodeView<void> const &parent): \
        iridium::parsing::serialization::NodeView<type> \
        (parent, iridium::parsing::serialization::convertCamelToSplittedBySymbol(#class_name, NAME_DELIMETER_SYMBOL), default_value) {} \
        using iridium::parsing::serialization::NodeView<type>::operator =; \
    } class_name = *this;

/// \~english @def DEFINE_ATTRIBUTE(...)
/// @brief Chooser macro that dispatches to `DEFINE_ATTRIBUTE_2` or `DEFINE_ATTRIBUTE_3`
///     based on the number of arguments. Allows defining attributes either as mandatory or with a default value.
/// \~russian @def DEFINE_ATTRIBUTE(...)
/// @brief Макрос-переключатель, который диспетчеризует вызов в `DEFINE_ATTRIBUTE_2` или `DEFINE_ATTRIBUTE_3`
///     в зависимости от количества аргументов. Позволяет определять атрибуты либо как обязательные, либо со значением по умолчанию.
#define DEFINE_ATTRIBUTE(...) \
DEFINE_MACRO_CHOOSER(DEFINE_ATTRIBUTE, __VA_ARGS__)(__VA_ARGS__)

/// \~english @def DEFINE_NODE_LIST_BEGIN(class_name)
/// @brief Begins the definition of a list of complex child nodes.
///     Generates two structs: `T##class_name` (for individual list items, inheriting `NodeView<void>`)
///     and `T##class_name##List` (for the list itself, inheriting `NodeViewList<T##class_name>`).
///     The inner struct `T##class_name` is then defined by subsequent `DEFINE_ATTRIBUTE` or `DEFINE_NODE_BEGIN/END` calls
///     before `DEFINE_NODE_LIST_END`.
///     Note: A `todo` in the source mentions a potential bug with node naming for lists (e.g., "NameList" instead of "Name").
/// \~russian @def DEFINE_NODE_LIST_BEGIN(class_name)
/// @brief Начинает определение списка сложных дочерних узлов.
///     Генерирует две структуры: `T##class_name` (для отдельных элементов списка, наследуя `NodeView<void>`)
///     и `T##class_name##List` (для самого списка, наследуя `NodeViewList<T##class_name>`).
///     Внутренняя структура `T##class_name` затем определяется последующими вызовами `DEFINE_ATTRIBUTE` или `DEFINE_NODE_BEGIN/END`
///     перед `DEFINE_NODE_LIST_END`.
///     Примечание: `todo` в исходном коде упоминает потенциальную ошибку с именованием узлов для списков (например, "NameList" вместо "Name").
/// \~english @param class_name The base name for the list items and the list member itself (e.g., `Item` leads to `TItem` and `TItemList Item;`).
/// \~russian @param class_name Базовое имя для элементов списка и самого члена-списка (например, `Item` приводит к `TItem` и `TItemList Item;`).
// todo: bugfix node name, e g NameList instead Name
#define DEFINE_NODE_LIST_BEGIN(class_name) \
    struct T##class_name##List; \
    struct T##class_name: public iridium::parsing::serialization::NodeView<void> { \
        T##class_name( \
        iridium::parsing::INode::TSharedPtr const &node, \
        std::string const &path): \
        iridium::parsing::serialization::NodeView<void> \
        (node, iridium::parsing::serialization::convertCamelToSplittedBySymbol(#class_name, NAME_DELIMETER_SYMBOL), path) {} \
        T##class_name(): iridium::parsing::serialization::NodeView<void> \
        (iridium::parsing::serialization::convertCamelToSplittedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \

/// \~english @def DEFINE_NODE_LIST_END(class_name)
/// @brief Ends the definition of a list of complex child nodes and declares an instance of the list.
///     Closes the `T##class_name` struct definition and defines the `T##class_name##List` struct,
///     then declares a member variable `class_name` of type `T##class_name##List`.
/// \~russian @def DEFINE_NODE_LIST_END(class_name)
/// @brief Завершает определение списка сложных дочерних узлов и объявляет экземпляр списка.
///     Закрывает определение структуры `T##class_name` и определяет структуру `T##class_name##List`,
///     затем объявляет переменную-член `class_name` типа `T##class_name##List`.
/// \~english @param class_name The name of the member variable for the list (should match `class_name` in `DEFINE_NODE_LIST_BEGIN`).
/// \~russian @param class_name Имя переменной-члена для списка (должно совпадать с `class_name` в `DEFINE_NODE_LIST_BEGIN`).
#define DEFINE_NODE_LIST_END(class_name) \
    }; \
    struct T##class_name##List: public iridium::parsing::serialization::NodeViewList<T##class_name> { \
        T##class_name##List(iridium::parsing::serialization::NodeView<void> const &parent): \
        iridium::parsing::serialization::NodeViewList<T##class_name> \
        (parent, iridium::parsing::serialization::convertCamelToSplittedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
    } class_name = *this;

/// \~english @def DEFINE_ATTRIBUTE_LIST(type, class_name)
/// @brief Defines a list of simple attributes (value nodes) within a parent node structure.
///     Generates `T##class_name` (inheriting `NodeView<type>`) for list items and `T##class_name##List`
///     (inheriting `NodeViewList<T##class_name>`) for the list itself, then declares a member `class_name` of the list type.
/// \~russian @def DEFINE_ATTRIBUTE_LIST(type, class_name)
/// @brief Определяет список простых атрибутов (узлов-значений) внутри родительской структуры узла.
///     Генерирует `T##class_name` (наследуя `NodeView<type>`) для элементов списка и `T##class_name##List`
///     (наследуя `NodeViewList<T##class_name>`) для самого списка, затем объявляет член `class_name` типа списка.
/// \~english @param type The data type of the attributes in the list.
/// \~russian @param type Тип данных атрибутов в списке.
/// \~english @param class_name The name of the member variable for the list and base for attribute node names.
/// \~russian @param class_name Имя переменной-члена для списка и база для имен узлов атрибутов.
#define DEFINE_ATTRIBUTE_LIST(type, class_name) \
    struct T##class_name : public iridium::parsing::serialization::NodeView<type> { \
        T##class_name(iridium::parsing::serialization::NodeView<void> const &parent): \
            iridium::parsing::serialization::NodeView<type> \
                (parent, iridium::parsing::serialization::convertCamelToSplittedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
        using iridium::parsing::serialization::NodeView<type>::operator =; \
        T##class_name( \
            iridium::parsing::INode::TSharedPtr const &node, \
            std::string const &path): \
                iridium::parsing::serialization::NodeView<type> \
                    (node, iridium::parsing::serialization::convertCamelToSplittedBySymbol(#class_name, NAME_DELIMETER_SYMBOL), path) {} \
        T##class_name(): \
            iridium::parsing::serialization::NodeView<type> \
                (iridium::parsing::serialization::convertCamelToSplittedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
    }; \
    struct T##class_name##List : public iridium::parsing::serialization::NodeViewList<T##class_name> { \
        T##class_name##List(iridium::parsing::serialization::NodeView<void> const &parent): \
        iridium::parsing::serialization::NodeViewList<T##class_name> \
        (parent, iridium::parsing::serialization::convertCamelToSplittedBySymbol(#class_name, NAME_DELIMETER_SYMBOL)) {} \
    } class_name = *this;

/// \~english @def DEFINE_NODE_EXTERNAL(class_name)
/// @brief Declares a member that is an instance of an externally defined root node structure.
///     This allows composition of separately defined serializable structures.
///     The member `class_name` will be of type `T##class_name`, which is expected to be defined elsewhere
///     using `DEFINE_ROOT_NODE_BEGIN/END`.
/// \~russian @def DEFINE_NODE_EXTERNAL(class_name)
/// @brief Объявляет член, являющийся экземпляром внешне определенной структуры корневого узла.
///     Это позволяет компоновать отдельно определенные сериализуемые структуры.
///     Член `class_name` будет иметь тип `T##class_name`, который, как ожидается, определен в другом месте
///     с использованием `DEFINE_ROOT_NODE_BEGIN/END`.
/// \~english @param class_name The name of the member variable and the base name of its type (e.g., `OtherConfig` implies type `TOtherConfig`).
/// \~russian @param class_name Имя переменной-члена и базовое имя ее типа (например, `OtherConfig` подразумевает тип `TOtherConfig`).
#define DEFINE_NODE_EXTERNAL(class_name) \
    T##class_name class_name = *this;

/// \~english @def DEFINE_NODE_PTR(class_name)
/// @brief Defines a member that is a lazily-initialized pointer (`NodeViewPtr`) to a nested structured node.
///     Useful for optional or recursive node definitions. Generates a typedef `T##class_name##Ptr`
///     and a member `class_name##_ptr` of this pointer type. `T##class_name` is expected to be defined
///     using `DEFINE_NODE_BEGIN/END` or `DEFINE_ROOT_NODE_BEGIN/END` (if referencing another root type).
/// \~russian @def DEFINE_NODE_PTR(class_name)
/// @brief Определяет член, являющийся лениво инициализируемым указателем (`NodeViewPtr`) на вложенный структурированный узел.
///     Полезно для опциональных или рекурсивных определений узлов. Генерирует typedef `T##class_name##Ptr`
///     и член `class_name##_ptr` этого типа указателя. Ожидается, что `T##class_name` определен
///     с использованием `DEFINE_NODE_BEGIN/END` или `DEFINE_ROOT_NODE_BEGIN/END` (если ссылается на другой корневой тип).
/// \~english @param class_name The base name for the node type (`T##class_name`) and the pointer member.
/// \~russian @param class_name Базовое имя для типа узла (`T##class_name`) и члена-указателя.
// recursive node
#define DEFINE_NODE_PTR(class_name) \
    typedef typename iridium::parsing::serialization::NodeViewPtr<T##class_name> T##class_name##Ptr; \
    T##class_name##Ptr class_name##_ptr = *this;

/// \~english @def DEFINE_NODE_LIST_PTR(class_name)
/// @brief Defines a member that is a lazily-initialized pointer (`NodeViewPtr`) to a list of complex child nodes.
///     Useful for optional or recursive lists of nodes. Generates `T##class_name##ListPtr` typedef
///     and a member `class_name##_list_ptr`. `T##class_name##List` is expected to be generated by
///     `DEFINE_NODE_LIST_BEGIN/END`.
/// \~russian @def DEFINE_NODE_LIST_PTR(class_name)
/// @brief Определяет член, являющийся лениво инициализируемым указателем (`NodeViewPtr`) на список сложных дочерних узлов.
///     Полезно для опциональных или рекурсивных списков узлов. Генерирует typedef `T##class_name##ListPtr`
///     и член `class_name##_list_ptr`. Ожидается, что `T##class_name##List` сгенерирован
///     `DEFINE_NODE_LIST_BEGIN/END`.
/// \~english @param class_name The base name for the node list type (`T##class_name##List`) and the pointer member.
/// \~russian @param class_name Базовое имя для типа списка узлов (`T##class_name##List`) и члена-указателя.
// recursive node pointer
#define DEFINE_NODE_LIST_PTR(class_name) \
    typedef typename iridium::parsing::serialization::NodeViewPtr<T##class_name##List> T##class_name##ListPtr; \
    T##class_name##ListPtr class_name##_list_ptr = *this;


#endif // HEADER_NODE_94200784_8C23_4200_B54C_65736B455736
