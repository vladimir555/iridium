#include <iridium/testing/tester.h>

#include <iridium/parsing/serialization/node.h>


//class Table {
//public:
//
//};


//class Connector {
//public:
//    DEFINE_CREATE(Connector)
//    Connector(config::TDatebaseConnector const &config) {
//        m_connector = createConnector(config);
//    }

//    virtual ~Connector() = default;

//    void initialize() {
//        m_connector->initialize();
//    }

//    void finalize() {
//        m_connector->finalize();
//    }

//private:
//    IConnector::TSharedPtr m_connector;
//};


//DEFINE_ROOT_NODE_BEGIN(Authors)
//    DEFINE_NODE_LIST_BEGIN(Rows)
//        DEFINE_ATTRIBUTE_DEFAULT(uint64_t   , id    , {})
//        DEFINE_ATTRIBUTE_DEFAULT(std::string, name  , {})
//    DEFINE_NODE_LIST_END(Rows)
//DEFINE_ROOT_NODE_END()


//template<typename TNode>
//class Table {
//public:
//    typedef typename TColumns TNode::TColumns;
//};


//class Select {
//public:
//    Select(std::list<TBook::Columns>);
//   ~Select() = default;
//};


//class From {
//public:
//    From(TBook const &book) {}
//   ~From() {}
//};


//template<typename TTable>
//class Select {
//public:
//    Select(TTable const &table): m_table(table) {}

////    template<typename TTable::TValue>
//    class Condition {
//    public:
//        Condition() = default;
//       ~Condition() = default;

////        typedef typename TTable::TRows::TValue TValue;

//        template<TTable::TRows>
//        bool operator <  (TTable::TRows::TValue const &value) {
//            return false;
//        }
//    };

//    void Where(Condition const &condition) {}
//private:
//    TTable m_table;
//};


//SELECT DATALENGTH(ProductName) as [SizeInBytes] FROM MyOrderTable
TEST(query) {
//    TAuthors authors;
    
//    Select s(authors);
//    s.Where(Condition());

//    LOGT << TBook::TColumns().

//    From()

//    LOGT << book.getNode();
//    LOGT << book.Columns.

//    From f(b);

//    TDatebaseConnector config;
//    config.Type     = TDatebaseConnector::TDBType::POSTGRES;
//    config.Url      = URL("postgres://localhost");
//    config.User     = "postgres";
//    config.Password = "postgres";
//    config.Database = "postgres";

//    auto connector = Connector::create(config);
}
