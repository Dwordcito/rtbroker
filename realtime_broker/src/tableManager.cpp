#include "tableManager.hpp"
#include "sysInfo.hpp"
#include <iostream>

constexpr auto IPV4 { 0 };
constexpr auto IPV6 { 1 };
static const std::map<int, std::string> IP_TYPE
{
    { IPV4, "ipv4" },
    { IPV6, "ipv6" }
};

class FactoryData final
{
    public:
        static nlohmann::json getData(const std::string& table, SysInfo& sysInfo)
        {
            nlohmann::json retVal;

            if (table == "packages")
            {
                retVal = sysInfo.packages();
            }
            else if (table == "processes")
            {
                retVal = sysInfo.processes();
            }
            else if (table == "os")
            {
                retVal.push_back(sysInfo.os());
            }
            else if (table == "hw")
            {
                retVal.push_back(sysInfo.hardware());
            }
            else if (table == "hfs")
            {
                retVal = sysInfo.hotfixes();
            }
            else if (table == "ports")
            {
                retVal = sysInfo.ports();
            }
            else if (table == "net_addr")
            {
                const auto networks = sysInfo.networks();
                const auto& itIface { networks.find("iface") };

                if (networks.end() != itIface)
                {
                    nlohmann::json addressTableDataList {};

                    for (const auto& item : itIface.value())
                    {
                        if (item.find("IPv4") != item.end())
                        {
                            for (auto addressTableData : item.at("IPv4"))
                            {
                                // "dbsync_network_address" table data to update and notify
                                addressTableData["iface"]     = item.at("name");
                                addressTableData["proto"]     = "ipv4";
                                addressTableDataList.push_back(std::move(addressTableData));
                            }
                        }

                        if (item.find("IPv6") != item.end())
                        {
                            for (auto addressTableData : item.at("IPv6"))
                            {
                                // "dbsync_network_address" table data to update and notify
                                addressTableData["iface"]     = item.at("name");
                                addressTableData["proto"]     = "ipv6";
                                addressTableDataList.push_back(std::move(addressTableData));
                            }
                        }
                    }
                    retVal = std::move(addressTableDataList);
                }
            }
            else if (table == "net_proto")
            {
                const auto networks = sysInfo.networks();
                const auto& itIface { networks.find("iface") };

                if (networks.end() != itIface)
                {
                    nlohmann::json protoTableDataList {};

                    for (const auto& item : itIface.value())
                    {
                        if (item.find("IPv4") != item.end())
                        {
                            // "dbsync_network_protocol" table data to update and notify
                            nlohmann::json protoTableData {};
                            protoTableData["iface"]   = item.at("name");
                            protoTableData["gateway"] = item.at("gateway");
                            protoTableData["type"]    = IP_TYPE.at(IPV4);
                            protoTableData["dhcp"]    = item.at("IPv4").begin()->at("dhcp");
                            protoTableData["metric"]  = item.at("IPv4").begin()->at("metric");
                            protoTableDataList.push_back(std::move(protoTableData));
                        }

                        if (item.find("IPv6") != item.end())
                        {
                            // "dbsync_network_protocol" table data to update and notify
                            nlohmann::json protoTableData {};
                            protoTableData["iface"]   = item.at("name");
                            protoTableData["gateway"] = item.at("gateway");
                            protoTableData["type"]    = IP_TYPE.at(IPV6);
                            protoTableData["dhcp"]    = item.at("IPv6").begin()->at("dhcp");
                            protoTableData["metric"]  = item.at("IPv6").begin()->at("metric");
                            protoTableDataList.push_back(std::move(protoTableData));
                       }
                    }

                    retVal = std::move(protoTableDataList);
                }
            }
            else if (table == "net_ifaces")
            {
                const auto networks = sysInfo.networks();
                const auto& itIface { networks.find("iface") };

                if (networks.end() != itIface)
                {
                    nlohmann::json ifaceTableDataList {};

                    for (const auto& item : itIface.value())
                    {
                        // Split the resulting networks data into the specific DB tables
                        // "dbsync_network_iface" table data to update and notify
                        nlohmann::json ifaceTableData {};
                        ifaceTableData["name"]       = item.at("name");
                        ifaceTableData["adapter"]    = item.at("adapter");
                        ifaceTableData["type"]       = item.at("type");
                        ifaceTableData["state"]      = item.at("state");
                        ifaceTableData["mtu"]        = item.at("mtu");
                        ifaceTableData["mac"]        = item.at("mac");
                        ifaceTableData["tx_packets"] = item.at("tx_packets");
                        ifaceTableData["rx_packets"] = item.at("rx_packets");
                        ifaceTableData["tx_errors"]  = item.at("tx_errors");
                        ifaceTableData["rx_errors"]  = item.at("rx_errors");
                        ifaceTableData["tx_bytes"]   = item.at("tx_bytes");
                        ifaceTableData["rx_bytes"]   = item.at("rx_bytes");
                        ifaceTableData["tx_dropped"] = item.at("tx_dropped");
                        ifaceTableData["rx_dropped"] = item.at("rx_dropped");
                        ifaceTableDataList.push_back(std::move(ifaceTableData));
                    }

                    retVal = std::move(ifaceTableDataList);
                }
            }
            return retVal;
        }
};


std::string TableManager::columnSchema(const std::string& table) const
{
    std::string retVal;
    retVal += " (";

    for (auto& column : tableColumnsSchema.at(table))
    {
        retVal += column.first + " " + fieldMap.at(column.second) + ",";
    }
    retVal.pop_back();
    retVal += ")";
    return retVal;
}

TableColumns TableManager::columns(const std::string& table) const
{
    return tableColumnsSchema.at(table);
}

nlohmann::json TableManager::data(const std::string& table) const
{
    static SysInfo sysInfo;
    return FactoryData::getData(table, sysInfo);
}

