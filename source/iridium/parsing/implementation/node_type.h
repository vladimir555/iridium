// Copyright © 2019 Bulaev Vladimir.
// Contacts: <bulaev_vladimir@mail.ru>
// License: https://www.gnu.org/licenses/lgpl-3.0

#ifndef HEADER_NODE_0A80E93C_07DA_494B_8C4D_8D0AF4B134A5
#define  HEADER_NODE_0A80E93C_07DA_494B_8C4D_8D0AF4B134A5


#include <string>
#include <list>
#include <unordered_map>

#include "iridium/smart_ptr.h"
#include "iridium/parsing/node_type.h"
//#include "iridium/caching/cached_value.h"


namespace iridium {
namespace parsing {
namespace implementation {


// ----- interface


template<typename TValue>
class CNodeType : public INodeType<TValue> {
public:
    DEFINE_IMPLEMENTATION(CNodeType)
    ///
    explicit CNodeType(std::string const &name, TValue const &value);
    ///
    explicit CNodeType(std::string const &name);
    ///
    std::string getName()  const override final;
    ///
    TValue      getValue() const override;
    ///
    void        setValue(TValue const &value) override;
    ///
    typename INodeType<TValue>::TNodes          slice(std::string const &path) override;
    ///
    typename INodeType<TValue>::iterator        begin() override;
    ///
    typename INodeType<TValue>::iterator        end() override;
    ///
    typename INodeType<TValue>::const_iterator  begin() const override;
    ///
    typename INodeType<TValue>::const_iterator  end() const override;
    ///
    size_t size() const override;
    ///
    bool hasChilds() const override;
    ///
    typename INodeType<TValue>::TSharedPtr      getChild(std::string const &name) override;
    ///
    typename INodeType<TValue>::TConstSharedPtr getChild(std::string const &name) const override;
    ///
    typename INodeType<TValue>::TSharedPtr      addChild(typename INodeType<TValue>::TSharedPtr const &child_node) override;
    ///
    typename INodeType<TValue>::TSharedPtr      addChild(std::string const &name) override;
    ///
    typename INodeType<TValue>::TSharedPtr      addChild(std::string const &name, TValue const &value) override;
    ///
    typename INodeType<TValue>::TSharedPtr      clone() const override;

private:
    ///
    typedef std::unordered_map<std::string, std::string const *> TMapStringPtr;
    ///
    explicit CNodeType(std::string const &name, TValue const &value, std::shared_ptr<TMapStringPtr> const &map_name_ptr);
public:
    ///
    explicit CNodeType(std::string const &name, std::shared_ptr<TMapStringPtr> const &map_name_ptr);
private:
    ///
    std::string const *makeName(std::string const &name);
    ///
    std::shared_ptr<TMapStringPtr>      m_map_name_ptr;
    ///
    std::string const * const           m_name = nullptr;
    ///
    TValue                              m_value;
    ///
    typename INodeType<TValue>::TNodes  m_nodes;
};


// ----- implementation


template<typename TValue>
std::string const * CNodeType<TValue>::makeName(std::string const& name) {
    auto it = m_map_name_ptr->find(name);
    if (it == m_map_name_ptr->end()) {
        auto inserted = m_map_name_ptr->insert({name, nullptr}); // Вставляем новый элемент
        inserted.first->second = &inserted.first->first;
        return inserted.first->second;
    }
    return it->second;
}


template<typename TValue>
CNodeType<TValue>::CNodeType(std::string const &name, TValue const &value)
:
    m_map_name_ptr  (std::make_shared<TMapStringPtr>()),
    m_name          (makeName(name)),
    m_value         (value)
{}


template<typename TValue>
CNodeType<TValue>::CNodeType(std::string const &name)
:
    m_map_name_ptr  (std::make_shared<TMapStringPtr>()),
    m_name          (makeName(name)),
    m_value         ()
{}


template<typename TValue>
CNodeType<TValue>::CNodeType(std::string const &name, TValue const &value, std::shared_ptr<TMapStringPtr> const &map_name_ptr)
:
    m_map_name_ptr  (map_name_ptr),
    m_name          (makeName(name)),
    m_value         (value)
{}


template<typename TValue>
CNodeType<TValue>::CNodeType(std::string const &name, std::shared_ptr<TMapStringPtr> const &map_name_ptr)
:
    m_map_name_ptr  (map_name_ptr),
    m_name          (makeName(name)),
    m_value         ()
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
    return addChild(CNodeType<TValue>::TSharedPtr(new CNodeType<TValue>(name, m_map_name_ptr))); // ----->
}


template<typename TValue>
typename INodeType<TValue>::TSharedPtr CNodeType<TValue>::addChild(std::string const &name, TValue const &value) {
    return addChild(CNodeType<TValue>::TSharedPtr(new CNodeType<TValue>(name, value, m_map_name_ptr))); // ----->
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
