/*
 ntp.h - ESP32GreenHouseDIY library
 Copyright (c) 2021 ZanzyTHEbar
*/
#pragma once
#ifndef NETWORKNTP_hpp
#define NETWORKNTP_hpp
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <string>
#include "local/data/visitor.hpp"

class NetworkNTP : public Element<Visitor<SensorInterface<std::string>>>,
                   public SensorInterface<std::string> {
 public:
  // constructors
  NetworkNTP();
  virtual ~NetworkNTP();
  // Functions
  void begin();
  void ntpLoop();
  std::string read() override;
  const std::string& getSensorName() override;
  void accept(Visitor<SensorInterface<std::string>>& visitor) override;
#if NTP_MANUAL_ENABLED
  time_t getNtpTime();
  void digitalClockDisplay();
  void printDigits(int digits);
  void sendNTPpacket(IPAddress& address);
#else
  const std::string& getFullDate();
  const std::string& getDayStamp();
  const std::string& getTimeStamp();
  const std::string getYear();
  const std::string getMonth();
  const std::string getDay();
#endif  // NTP_MANUAL_ENABLED

  // Private variables
 private:
  // Variables to save date and time
  std::string _formattedDate;
  std::string _dayStamp;
  std::string _timeStamp;

  // NTP Servers:
  static const char ntpServerName[16];
  unsigned int localPort;  // local port to listen for UDP packets
  int timeZone;

  time_t prevDisplay;  // when the digital clock was displayed
  WiFiUDP ntpUDP;
  NTPClient timeClient;
};
#endif
