/*
* This is an independent project of an individual developer. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
*/
#include "connector.h"


using iridium::db::IConnector;
using std::vector;
using std::string;


string const iridium::db::IConnector::FIELD_NAME_AFFECTED_ROWS = "affected_rows";


iridium::db::Exception::Exception(std::string const &e)
:
    std::runtime_error(e.c_str()) 
{}


vector<string> IConnector::makeFields(TRows const &rows) {
    vector<string> fields;

    if (!rows.empty())
        for (auto const &field : rows[0])
            fields.push_back(field.first);

    return fields; // ----->
}
