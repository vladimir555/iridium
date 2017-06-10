#include "connector.h"


IMPLEMENT_ENUM(utility::db::IDBConnector::TDBType)


using utility::db::IDBConnector;
using std::vector;
using std::string;


string const utility::db::IDBConnector::FIELD_NAME_AFFECTED_ROWS = "affected_rows";


utility::db::DBException::DBException(std::string const &e)
:
    std::runtime_error(e.c_str()) 
{}


vector<string> IDBConnector::makeFields(TRows const &rows) {
    vector<string> fields;

    if (!rows.empty())
        for (auto const &field : rows[0])
            fields.push_back(field.first);

    return fields; // ----->
}
