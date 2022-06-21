/*
 * Wazuh SysInfo
 * Copyright (C) 2015, Wazuh Inc.
 * October 7, 2020.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */
#include "sysInfo.hpp"

nlohmann::json SysInfo::hardware()
{
    nlohmann::json ret;
    ret["board_serial"] = getSerialNumber();
    ret["cpu_name"] = getCpuName();
    ret["cpu_cores"] = getCpuCores();
    ret["cpu_mhz"] = double(getCpuMHz());
    getMemory(ret);
    return ret;
}

nlohmann::json SysInfo::packages()
{
    return getPackages();
}

nlohmann::json SysInfo::os()
{
    return getOsInfo();
}

nlohmann::json SysInfo::processes()
{
    return getProcessesInfo();
}

nlohmann::json SysInfo::networks()
{
    return getNetworks();
}

nlohmann::json SysInfo::ports()
{
    return getPorts();
}

void SysInfo::processes(std::function<void(nlohmann::json&)> callback)
{
    getProcessesInfo(callback);
}

void SysInfo::packages(std::function<void(nlohmann::json&)> callback)
{
    getPackages(callback);
}

nlohmann::json SysInfo::hotfixes()
{
    return getHotfixes();
}

