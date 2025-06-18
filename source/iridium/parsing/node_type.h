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


// todo: Depth-First Search, DFS) and (Breadth-First Search, BFS) methods maybe parallel
///
/// \brief The INodeType class
///
template<typename TValue>
class INodeType : public pattern::IPrototype<INodeType<TValue> > {
public:
    DEFINE_INTERFACE(INodeType)
    ///
    typedef std::list<typename INodeType::TSharedPtr>   TNodesSharedPtr;
    ///
    typedef std::list<INodeType<TValue> *>              TNodesPtr;
    ///
    typedef typename TNodesSharedPtr::iterator          iterator;
    ///
    typedef typename TNodesSharedPtr::const_iterator    const_iterator;
    ///
    typedef typename TNodesSharedPtr::const_reference   const_reference;
    ///
    virtual typename INodeType::TNodesPtr getParents() const = 0;
    ///
    virtual std::string     getName() const = 0;
    ///
    virtual TValue          getValue() const = 0;
    ///
    virtual void            setValue(TValue const &value) = 0;
    ///
    virtual typename INodeType::TNodesSharedPtr slice(std::string const &path) const = 0;
    ///// todo:
    //virtual typename INodeType::TNodesConst findChilds(std::string const &path) const = 0;
    ///
    virtual iterator        begin() = 0;
    ///
    virtual iterator        end() = 0;
    ///
    virtual const_iterator  begin()     const = 0;
    ///
    virtual const_iterator  end()       const = 0;
    ///
    virtual size_t          size()      const = 0;
    ///
    virtual bool            hasChilds() const = 0;

    ///
    virtual typename INodeType<TValue>::TSharedPtr      getChild(std::string const &name) = 0;
    ///
    virtual typename INodeType<TValue>::TConstSharedPtr getChild(std::string const &name) const = 0;
    ///
    virtual          TNodesSharedPtr                    getChilds(std::string const &name) const = 0;
    ///
    virtual typename INodeType::TSharedPtr              addChild(typename INodeType::TSharedPtr const &child_node) = 0;
    ///
    virtual typename INodeType::TSharedPtr              addChild(std::string const &name) = 0;
    ///
    virtual typename INodeType::TSharedPtr              addChild(std::string const &name, TValue const &value) = 0;
    ///
    virtual void                                        delChilds(std::string const &name) = 0;
};


} // parsing
} // iridium


#endif // HEADER_NODE_96BF1757_0A3E_46F0_94CB_A7AFC4877775
