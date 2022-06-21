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

#ifndef _ISQLITE_HPP
#define _ISQLITE_HPP

#include <string>
#include <string_view>
#include <sqlite3.h>
#include <memory>
#include <math.h>

namespace SQLite
{
    class IConnection
    {
        public:
            // LCOV_EXCL_START
            virtual ~IConnection() = default;
            // LCOV_EXCL_STOP
            virtual void close() = 0;
            virtual const std::shared_ptr<sqlite3>& db() const = 0;
            virtual void attachModule(const std::string& moduleName,
                                      void *pClientData) = 0;
    };

    class IColumn
    {
        public:
            // LCOV_EXCL_START
            virtual ~IColumn() = default;
            // LCOV_EXCL_STOP
            virtual int32_t type() const = 0;
            virtual std::string name() const = 0;
            virtual bool hasValue() const = 0;
            virtual int32_t value(const int32_t&) const = 0;
            virtual uint64_t value(const uint64_t&) const = 0;
            virtual int64_t value(const int64_t&) const = 0;
            virtual std::string value(const std::string&) const = 0;
            virtual double_t value(const double_t&) const = 0;
    };

    class IStatement
    {
        public:
            // LCOV_EXCL_START
            virtual ~IStatement() = default;
            // LCOV_EXCL_STOP
            virtual int32_t step() = 0;
            virtual int columnsCount() const = 0;

            virtual std::string expand() = 0;

            virtual std::unique_ptr<IColumn> column(const int32_t index) = 0;
    };
}

#endif // _ISQLITE_HPP
