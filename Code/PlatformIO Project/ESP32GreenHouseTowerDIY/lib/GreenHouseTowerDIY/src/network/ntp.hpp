/*
 ntp.h - ESP32GreenHouseDIY library
 Copyright (c) 2021 ZanzyTHEbar
*/
#pragma once
#ifndef NETWORKNTP_hpp
#define NETWORKNTP_hpp

#include <defines.hpp>
#include <NTPClient.h>
#include <WiFiUdp.h>

class NetworkNTP
{
public:
    // constructors
    NetworkNTP();
    virtual ~NetworkNTP();
    // Functions
    void SetupNTP();
    void NTPLoop();
#if NTP_MANUAL_ENABLED
    time_t getNtpTime();
    void digitalClockDisplay();
    void printDigits(int digits);
    void sendNTPpacket(IPAddress &address);
#else
    String getFullDate();
    String getDayStamp();
    String getTimeStamp();
    String getYear();
    String getMonth();
    String getDay();
#endif // NTP_MANUAL_ENABLED

    // Private variables
private:
    // Variables to save date and time
    String _formattedDate;
    String _dayStamp;
    String _timeStamp;
};

extern NetworkNTP networkntp;
#endif
