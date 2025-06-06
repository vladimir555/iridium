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


template<typename TValue>
class CNodeType : public INodeType<TValue> {
public:
    DEFINE_CREATE(CNodeType)
    ///
    explicit CNodeType(std::string const &name, TValue const &value);
    ///
    explicit CNodeType(std::string const &name);
    ///
    virtual ~CNodeType();
    ///
    virtual typename INodeType<TValue>::TNodesPtr getParents() const override;
    ///
    std::string getName() const override final;
    ///
    TValue      getValue() const override;
    ///
    void        setValue(TValue const &value) override;
    ///
    typename INodeType<TValue>::TNodesSharedPtr slice(std::string const &path) const override;
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
    typename INodeType<TValue>::TNodesSharedPtr getChilds(std::string const &name) const override;
    ///
    typename INodeType<TValue>::TSharedPtr      addChild(typename INodeType<TValue>::TSharedPtr const &child_node) override;
    ///
    typename INodeType<TValue>::TSharedPtr      addChild(std::string const &name) override;
    ///
    typename INodeType<TValue>::TSharedPtr      addChild(std::string const &name, TValue const &value) override;
    ///
    void                                        delChild(std::string const &name) override;
    ///
    typename INodeType<TValue>::TSharedPtr      clone() const override;

private:
    ///
    typedef std::unordered_set<std::string> TStrings;
    ///
    explicit CNodeType(std::string const &name, TValue const &value, std::shared_ptr<TStrings> const &strings);
    ///
    explicit CNodeType(std::string const &name, std::shared_ptr<TStrings> const &strings);
    ///
    std::string const *makeName(std::string const &name);
    ///
    std::shared_ptr<TStrings> m_strings;
    ///
    std::string const * const m_name = nullptr;
    ///
    TValue m_value;
    ///
    typename INodeType<TValue>::TNodesSharedPtr m_nodes;
    ///
    typename INodeType<TValue>::TNodesPtr       m_parent_nodes;
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
