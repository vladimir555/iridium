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


DEFINE_ROOT_NODE_BEGIN(Authors)
    DEFINE_NODE_BEGIN(Rows)
        DEFINE_ATTRIBUTE(uint64_t   , id    , {})
        DEFINE_ATTRIBUTE(std::string, name  , {})
    DEFINE_NODE_END(Rows)
DEFINE_ROOT_NODE_END()


//template<typename TNode>
//class Table {
//public:
//    typedef typename TColumns TNode::TColumns;
//};


//template<typename ... TTables>
//class From {
//public:
//    From(TTables const & ... tables) {}

//    class Select {
//    public:
//        Select() {}
//       ~Select() = default;
//    };

//    Select *Select_() { return new From<TTables ... >::Select(); }
//private:
////    TTables m_tables;
//};


template<typename TValue>
struct Condition {
    Condition(TValue const &value) {
    }

    Condition operator <  (TValue const &value) {
        return *this;
    }

    Condition operator <= (TValue const &value) {
        return *this;
    }

    Condition operator >  (TValue const &value) {
        return *this;
    }

    Condition operator >= (TValue const &value) {
        return *this;
    }

    Condition operator == (TValue const &value) {
        return *this;
    }

    Condition operator && (TValue const &value) {
        return *this;
    }
};


struct SQL {
    std::string getSQL() {
        return "sql";
    }
};


struct Order: SQL {
    SQL order() {
        return SQL();
    }
};


struct Having: Order {
    Order having() {
        return Order();
    }
};


struct Group: Having {
    Having group() {
        return Having();
    }
};


struct Where: Group {
    Group where() {
        return Group();
    }
};


struct Select {
    template<typename ... TColumns>
    Where select(TColumns const & ... columns) {
        return Where();
    }
    Where select() {
        return Where();
    }
};


template<typename ... TTables>
Select from(TTables const & ... tables) {
    return Select();
}


//SELECT
//  [DISTINCT | DISTINCTROW | ALL]
//  select_expression,...
//FROM table_references
//[WHERE where_definition]
//[GROUP BY {unsigned_integer | col_name | formula}]
//[HAVING where_definition]
//[ORDER BY {unsigned_integer | col_name | formula} [ASC | DESC], ...]


//SELECT DATALENGTH(ProductName) as [SizeInBytes] FROM MyOrderTable
TEST(query) {
    TAuthors authors;

    from(authors).select(authors.Rows.id, authors.Rows.name).getSQL();

    from(authors).select(authors.Rows.id).order().getSQL();
    from(authors).select(authors.Rows.id).group().getSQL();
    from(authors).select(authors.Rows.id).where().group().getSQL();

    from(authors).select().where().getSQL();
    from(authors).select().where().group().getSQL();
    from(authors).select().where().order().getSQL();
    from(authors).select().group().getSQL();
    from(authors).select().having().getSQL();
    from(authors).select().group().having().order().getSQL();

//    Condition(5) < 5;
    
//    From f(authors);
//    f.Select_();

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
//    auto connector  = Connector::create(config);
}
