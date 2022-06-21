/*
 * Wazuh RTBroker
 * Copyright (C) 2015, Wazuh Inc.
 * June 19, 2022.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#ifndef _SQLITE_HPP
#define _SQLITE_HPP

#include "isqlite.hpp"
#include "sqlite3.h"
#include "singleton.hpp"
#include <string>
#include <memory>
#include <unordered_map>

namespace SQLite
{
    class Connection : public IConnection
    {
        public:
            Connection();
            ~Connection() = default;
            explicit Connection(const std::string& path);

            void close() override;
            void attachModule(const std::string& moduleName,
                              void *pClientData) override;
            const std::shared_ptr<sqlite3>& db() const override;
        private:
            std::shared_ptr<sqlite3> m_db;
            std::unordered_map<std::string, sqlite3_module> m_modules;
            const sqlite3_module* getModule(const std::string& moduleName);
    };

    class Column : public IColumn
    {
        public:
            ~Column() = default;
            Column(std::shared_ptr<sqlite3_stmt>& stmt, const int32_t index);

            bool hasValue() const override;
            int32_t type() const override;
            std::string name() const override;
            int32_t value(const int32_t&) const override;
            uint64_t value(const uint64_t&) const override;
            int64_t value(const int64_t&) const override;
            std::string value(const std::string&) const override;
            double_t value(const double_t&) const override;
        private:
            std::shared_ptr<sqlite3_stmt> m_stmt;
            const int32_t m_index;
    };

    class Statement : public IStatement
    {
        public:
            ~Statement() = default;
            Statement(std::shared_ptr<IConnection> connection,
                      const std::string& query);

            int32_t step() override;
            int columnsCount() const override;
            std::string expand() override;
            std::unique_ptr<IColumn> column(const int32_t index) override;

        private:
            std::shared_ptr<IConnection> m_connection;
            std::shared_ptr<sqlite3_stmt> m_stmt;
    };
}

#endif // _SQLITE_H
