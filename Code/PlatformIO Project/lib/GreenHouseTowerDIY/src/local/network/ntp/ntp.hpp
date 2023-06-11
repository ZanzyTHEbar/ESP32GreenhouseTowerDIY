/*
 ntp.h - ESP32GreenHouseDIY library
 Copyright (c) 2021 ZanzyTHEbar
*/
#pragma once
#ifndef NETWORKNTP_hpp
#    define NETWORKNTP_hpp
#    include <Arduino.h>
#    include <NTPClient.h>
#    include <WiFiUdp.h>
#    include <memory>

class NetworkNTP {
   public:
    // constructors
    NetworkNTP();
    virtual ~NetworkNTP();
    // Functions
    void begin();
    void NTPLoop();
#    if NTP_MANUAL_ENABLED
    time_t getNtpTime();
    void digitalClockDisplay();
    void printDigits(int digits);
    void sendNTPpacket(IPAddress& address);
#    else
    String getFullDate();
    String getDayStamp();
    String getTimeStamp();
    String getYear();
    String getMonth();
    String getDay();
#    endif  // NTP_MANUAL_ENABLED

    // Private variables
   private:
    // Variables to save date and time
    String _formattedDate;
    String _dayStamp;
    String _timeStamp;

    // NTP Servers:
    static const char ntpServerName[16];
    unsigned int localPort;  // local port to listen for UDP packets
    int timeZone;

    time_t prevDisplay;  // when the digital clock was displayed
    WiFiUDP ntpUDP;
    NTPClient timeClient;
};
#endif
