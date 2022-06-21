#include "rtImplementation.hpp"
#include "sqlite3/sqlite.hpp"
#include <iostream>

const std::vector<std::string> modules
{
    "packages",
    "processes",
    "os",
    "hw",
    "hfs",
    "ports",
    "net_addr",
    "net_ifaces",
    "net_proto"
};

void RTImplementation::init()
{
    m_Connection = std::make_shared<SQLite::Connection>(":memory:");

    for (const auto& module : modules)
    {
        m_Connection->attachModule(module, nullptr);
    }
}

void RTImplementation::close()
{
    m_Connection.reset();
}

nlohmann::json RTImplementation::getData(const std::string& query) const
{
    auto stmt { std::make_shared<SQLite::Statement>(m_Connection, query) };
    nlohmann::json retVal {};

    while (SQLITE_ROW == stmt->step())
    {
        nlohmann::json row {};

        for (auto i = 0; i < stmt->columnsCount(); ++i)
        {
            auto column { stmt->column(i) };
            auto type { column->type() };
            auto columnName { column->name() };

            if (SQLITE_INTEGER == type)
            {
                row[columnName] = column->value(int64_t{});
            }
            else if (SQLITE_TEXT == type)
            {
                row[columnName] = column->value(std::string{});
            }
            else if (SQLITE_FLOAT == type)
            {
                row[columnName] = column->value(double_t{});
            }
        }
        retVal.push_back(row);
    }

    std::cout << retVal.dump(4) << std::endl;
    return {};
}

