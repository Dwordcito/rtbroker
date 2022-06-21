#ifndef _TABLE_MANAGER_HPP
#define _TABLE_MANAGER_HPP

#include "json.hpp"
#include "singleton.hpp"
#include <string>

enum FIELD_TYPE
{
    TEXT,
    INTEGER,
    BIGINT,
    DOUBLE,
    DATETIME,
    BOOLEAN,
    BLOB
};

const static std::unordered_map<int, std::string> fieldMap =
{
    { TEXT, "TEXT" },
    { INTEGER, "INTEGER" },
    { BIGINT, "BIGINT" },
    { DOUBLE, "DOUBLE" },
    { DATETIME, "DATETIME" },
    { BOOLEAN, "BOOLEAN" },
    { BLOB, "BLOB" },
};

using TableColumns = std::map<std::string, FIELD_TYPE>;

const static TableColumns packagesFields =
{
    { "name", TEXT },
    { "version", TEXT },
    { "vendor", TEXT },
    { "install_time", TEXT },
    { "location", TEXT },
    { "architecture", TEXT },
    { "groups", TEXT },
    { "description", TEXT },
    { "size", INTEGER },
    { "priority", TEXT },
    { "multiarch", TEXT },
    { "source", TEXT },
    { "format", TEXT },
};

const static TableColumns processesFields =
{
    { "pid", TEXT },
    { "name", TEXT },
    { "state", TEXT },
    { "ppid", BIGINT },
    { "utime", BIGINT },
    { "stime", BIGINT },
    { "cmd", TEXT },
    { "argvs", TEXT },
    { "euser", TEXT },
    { "ruser", TEXT },
    { "suser", TEXT },
    { "egroup", TEXT },
    { "rgroup", TEXT },
    { "sgroup", TEXT },
    { "fgroup", TEXT },
    { "priority", BIGINT },
    { "nice", BIGINT },
    { "size", BIGINT },
    { "vm_size", BIGINT },
    { "resident", BIGINT },
    { "share", BIGINT },
    { "start_time", BIGINT },
    { "pgrp", BIGINT },
    { "session", BIGINT },
    { "nlwp", BIGINT },
    { "tgid", BIGINT },
    { "tty", BIGINT },
    { "processor", BIGINT }
};

const static TableColumns osFields =
{
    { "hostname", TEXT },
    { "architecture", TEXT },
    { "os_name", TEXT },
    { "os_version", TEXT },
    { "os_codename", TEXT },
    { "os_major", TEXT },
    { "os_minor", TEXT },
    { "os_patch", TEXT },
    { "os_build", TEXT },
    { "os_platform", TEXT },
    { "sysname", TEXT },
    { "release", TEXT },
    { "version", TEXT },
    { "os_release", TEXT },
    { "os_display_version", TEXT },
};

const static TableColumns hwFields =
{
    { "board_serial", TEXT },
    { "cpu_name", TEXT },
    { "cpu_cores", INTEGER },
    { "cpu_mhz", DOUBLE },
    { "ram_total", INTEGER },
    { "ram_free", INTEGER },
    { "ram_usage", INTEGER },
};

const static TableColumns hfsFields =
{
    { "hotfix", TEXT },
};

const static TableColumns portsFields =
{
    { "protocol", TEXT },
    { "local_ip", TEXT },
    { "local_port", BIGINT },
    { "remote_ip", TEXT },
    { "remote_port", BIGINT },
    { "tx_queue", BIGINT },
    { "rx_queue", BIGINT },
    { "inode", BIGINT },
    { "state", TEXT },
    { "pid", BIGINT },
    { "process", TEXT },
};

const static TableColumns netIfacesFields =
{
    { "name", TEXT },
    { "adapter", TEXT },
    { "type", TEXT },
    { "state", TEXT },
    { "mtu", INTEGER },
    { "mac", TEXT },
    { "tx_packets", INTEGER },
    { "rx_packets", INTEGER },
    { "tx_bytes", INTEGER },
    { "rx_bytes", INTEGER },
    { "tx_errors", INTEGER },
    { "rx_errors", INTEGER },
    { "tx_dropped", INTEGER },
    { "rx_dropped", INTEGER },
};

const static TableColumns netProtoFields =
{
    { "iface", TEXT },
    { "type", TEXT },
    { "gateway", TEXT },
    { "dhcp", TEXT },
    { "metric", TEXT },
};

const static TableColumns netAddrFields =
{
    { "iface", TEXT },
    { "proto", TEXT },
    { "address", TEXT },
    { "netmask", TEXT },
    { "broadcast", TEXT },
};


const static std::unordered_map<std::string, TableColumns> tableColumnsSchema =
{
    {"packages", packagesFields },
    {"processes", processesFields },
    {"os", osFields },
    {"hw", hwFields },
    {"hfs", hfsFields },
    {"ports", portsFields },
    {"net_ifaces", netIfacesFields },
    {"net_proto", netProtoFields },
    {"net_addr", netAddrFields },
};

class TableManager : public Singleton<TableManager>
{
    public:
        std::string columnSchema(const std::string& table) const;
        nlohmann::json data(const std::string& table) const;
        TableColumns columns(const std::string& table) const;
};

#endif // _TABLE_MANAGER_HPP
