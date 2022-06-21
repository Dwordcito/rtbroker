/*
 * Wazuh RTBroker
 * Copyright (C) 2015, Wazuh Inc.
 * June 18, 2022.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#include "sqlite.hpp"
#include "../vTabsFunctions.hpp"
#include <iostream>

constexpr auto DB_DEFAULT_PATH {"temp.db"};

static void checkSqliteResult(const int result,
                              const std::string& exceptionString)
{
    if (SQLITE_OK != result)
    {
        throw std::system_error(result, std::system_category(), exceptionString);
    }
}

static sqlite3* openSQLiteDb(const std::string& path)
{
    sqlite3* pDb{ nullptr };
    const auto result
    {
        sqlite3_open_v2(path.c_str(), &pDb, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr)
    };
    checkSqliteResult(result, "Unspecified type during initialization of SQLite.");
    return pDb;
}

const sqlite3_module* SQLite::Connection::getModule(const std::string& moduleName)
{
    sqlite3_module* pModule{ nullptr };
    if (m_modules.find(moduleName) != m_modules.end()) {
        pModule = &m_modules[moduleName];
    }
    else
    {
        m_modules[moduleName] = {};
        m_modules[moduleName].xCreate = tables::sqlite::xCreate;
        m_modules[moduleName].xConnect = tables::sqlite::xCreate;
        m_modules[moduleName].xBestIndex = tables::sqlite::xBestIndex;
        m_modules[moduleName].xDisconnect = tables::sqlite::xDestroy;
        m_modules[moduleName].xDestroy = tables::sqlite::xDestroy;
        m_modules[moduleName].xOpen = tables::sqlite::xOpen;
        m_modules[moduleName].xClose = tables::sqlite::xClose;
        m_modules[moduleName].xFilter = tables::sqlite::xFilter;
        m_modules[moduleName].xNext = tables::sqlite::xNext;
        m_modules[moduleName].xEof = tables::sqlite::xEof;
        m_modules[moduleName].xColumn = tables::sqlite::xColumn;
        m_modules[moduleName].xRowid = tables::sqlite::xRowid;

        pModule = &m_modules[moduleName];
    }

    return pModule;
}

SQLite::Connection::Connection(const std::string& path)
    : m_db
    {
        openSQLiteDb(path), [](sqlite3 * p)
        {
            sqlite3_close_v2(p);
        }
    }
{}

void SQLite::Connection::close()
{
    m_db.reset();
}

const std::shared_ptr<sqlite3>& SQLite::Connection::db() const
{
    return m_db;
}

SQLite::Connection::Connection()
    : Connection(DB_DEFAULT_PATH)
{}


void SQLite::Connection::attachModule(const std::string& moduleName, void *pClientData)
{
    int rc = sqlite3_create_module(m_db.get(),
                                   moduleName.c_str(),
                                   getModule(moduleName),
                                   &(pClientData));
    checkSqliteResult(rc, "Unspecified type during initialization of SQLite Module.");
}

static sqlite3_stmt* prepareSQLiteStatement(std::shared_ptr<SQLite::IConnection>& connection,
                                            const std::string& query)
{
    sqlite3_stmt* pStatement{ nullptr };
    const auto result
    {
        sqlite3_prepare_v2(connection->db().get(), query.c_str(), -1, &pStatement, nullptr)
    };
    checkSqliteResult(result, sqlite3_errmsg(connection->db().get()));
    return pStatement;
}

SQLite::Statement::Statement(std::shared_ptr<IConnection> connection,
                             const std::string& query)
    : m_connection{ connection }
    , m_stmt
    {
        prepareSQLiteStatement(m_connection, query), [](sqlite3_stmt * p)
        {
            sqlite3_finalize(p);
        }
    }
{}

int32_t SQLite::Statement::step()
{
    auto ret { SQLITE_ERROR };

    ret = sqlite3_step(m_stmt.get());

    if (SQLITE_ROW != ret && SQLITE_DONE != ret)
    {
        checkSqliteResult(ret, sqlite3_errmsg(m_connection->db().get()));
    }

    return ret;
}

std::string SQLite::Statement::expand()
{
    return sqlite3_sql(m_stmt.get());
}

std::unique_ptr<SQLite::IColumn> SQLite::Statement::column(const int32_t index)
{
    return std::make_unique<SQLite::Column>(m_stmt, index);
}

int SQLite::Statement::columnsCount() const
{
    return sqlite3_column_count(m_stmt.get());
}

SQLite::Column::Column(std::shared_ptr<sqlite3_stmt>& stmt,
                       const int32_t index)
    : m_stmt{ stmt }
    , m_index{ index }
{}

bool SQLite::Column::hasValue() const
{
    return SQLITE_NULL != sqlite3_column_type(m_stmt.get(), m_index);
}

int32_t SQLite::Column::type() const
{
    return sqlite3_column_type(m_stmt.get(), m_index);
}

std::string SQLite::Column::name() const
{
    return sqlite3_column_name(m_stmt.get(), m_index);
}

int32_t SQLite::Column::value(const int32_t&) const
{
    return sqlite3_column_int(m_stmt.get(), m_index);
}

uint64_t SQLite::Column::value(const uint64_t&) const
{
    return sqlite3_column_int64(m_stmt.get(), m_index);
}

int64_t SQLite::Column::value(const int64_t&) const
{
    return sqlite3_column_int64(m_stmt.get(), m_index);
}

double_t SQLite::Column::value(const double_t&) const
{
    return sqlite3_column_double(m_stmt.get(), m_index);
}

std::string SQLite::Column::value(const std::string&) const
{
    const auto str
    {
        reinterpret_cast<const char*>(sqlite3_column_text(m_stmt.get(), m_index))
    };
    return nullptr != str ? str : "";
}

