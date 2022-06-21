/*
 * Wazuh RealTime Broker
 * Copyright (C) 2015, Wazuh Inc.
 * June 19, 2022.
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#include "rtBroker.hpp"
#include "rtImplementation.hpp"
#include <iostream>

RTBroker::RTBroker()
{
    RTImplementation::instance().init();
}
RTBroker::~RTBroker()
{
    RTImplementation::instance().close();
}

nlohmann::json RTBroker::getData(const std::string & query) const
{
    return RTImplementation::instance().getData(query);
}
