#ifndef HEADER_NODE_96BF1757_0A3E_46F0_94CB_A7AFC4877775
#define HEADER_NODE_96BF1757_0A3E_46F0_94CB_A7AFC4877775


#include <string>
#include <list>

#include "utility/pattern/prototype.h"
#include "utility/smart_ptr.h"
#include "utility/convertion/convert.h"


namespace utility {
namespace parsing {


///
template<typename TValue>
class INodeType : public pattern::IPrototype<INodeType<TValue> > {
public:
    DEFINE_SMART_PTR(INodeType<TValue>);
    ///
    typedef std::list<typename INodeType::TSharedPtr> TNodes;
    ///
    typedef std::list<typename INodeType::TConstSharedPtr> TNodesConst;
    ///
    typedef typename TNodes::iterator iterator;
    ///
    typedef typename TNodesConst::const_iterator const_iterator;
    ///
    typedef typename TNodesConst::const_reference const_reference;

    ///
    static std::string const PATH_DELIMITER;
    ///
    virtual ~INodeType() = default;
    ///
    virtual std::string getName() const = 0;
    ///
    virtual TValue getValue() const = 0;
    ///
    virtual void setValue(TValue const &value) = 0;
    ///
    virtual typename INodeType::TNodes findChilds(std::string const &path) = 0;
    ///// todo:
    //virtual typename INodeType::TNodesConst findChildNodesByPath(std::string const &path) const = 0;

    ///
    virtual iterator begin() = 0;
    ///
    virtual iterator end() = 0;
    ///
    virtual const_iterator begin() const = 0;
    ///
    virtual const_iterator end() const = 0;
    ///
    virtual size_t size() const = 0;
    ///
    virtual bool hasChilds() const = 0;

    ///
    virtual typename INodeType<TValue>::TSharedPtr getChild(std::string const &name) = 0;
    ///
    virtual typename INodeType<TValue>::TConstSharedPtr getChild(std::string const &name) const = 0;

    ///
    virtual typename INodeType::TSharedPtr addChild(typename INodeType::TSharedPtr const &child_node) = 0;
    ///
    virtual typename INodeType::TSharedPtr addChild(std::string const &name) = 0;
    ///
    virtual typename INodeType::TSharedPtr addChild(std::string const &name, TValue const &value) = 0;
};


template<typename TValue>
std::string const INodeType<TValue>::PATH_DELIMITER("/");


} // parsing
} // utility


#endif // HEADER_NODE_96BF1757_0A3E_46F0_94CB_A7AFC4877775
