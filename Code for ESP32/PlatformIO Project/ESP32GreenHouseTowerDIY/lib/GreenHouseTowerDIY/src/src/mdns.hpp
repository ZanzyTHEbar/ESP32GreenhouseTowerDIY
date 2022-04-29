/*
 Network.h - HBAT library
 Copyright (c) 2021 Zacariah Austin Heim.
*/
#pragma once
#ifndef HMSMDNS_hpp
#define HMSMDNS_hpp
#include "defines.hpp"
#include <ESPmDNS.h>

class HMSmdns
{
public:
    // constructors
    HMSmdns();
    virtual ~HMSmdns();
    int DiscovermDNSBroker();
    void SetupmDNSServer();
    bool mDNSLoop();

    // variables
};

extern HMSmdns Mdns;
#endif
