/*
 defines.hpp - ESP32GreenHouseDIY library
 Copyright (c) 2021 ZanzyTHEbar
 */
#ifndef DEFINES_hpp
#define DEFINES_hpp
#if !(defined(ESP32))
#error This code is intended to run on the ESP32 platform! Please check your Board setting.
#endif

#define LED_BUILTIN 2

#include <Arduino.h>
#include <stdio.h>  /* printf, NULL */
#include <stdlib.h> /* strtoul */
#include <ArduinoJson.h>
#include <strTools.h>
#include <PID_v1.h>

// File System
#include <SPIFFS.h>

// data Struct
#include "data/config.hpp"

// IO
#include <Wire.h>
#include "io/i2cscan.hpp"
#include "sensors/light/ldr.hpp"

// Temp Sensors
#include "sensors/temperature/towertemp.hpp"

// Humidity Sensors
#include "sensors/humidity/Humidity.hpp"
//#include <sfm3003.hpp>

// wifi definitions
#include "network/network.hpp"

// Accumulate Data
#include "data/accumulatedata.hpp"

// Timed tasks
#include "data/timedtasks.hpp"

// Relays
#include "io/Relays.hpp"

#define Stringize(L) #L
#define MakeString(M, L) M(L)

#define $Line MakeString(Stringize, __LINE__)

#define _TODO __FILE__ "(" $Line ") : TODO:: "
#define Reminder __FILE__ "(" $Line ") : Reminder:: "
#define Feature __FILE__ "(" $Line ") : Feature:: "

#define _STR(x) #x
#define STR(x) _STR(x)
#define TODO(x) _Pragma(STR(message("TODO: " STR(x) "::" __FILE__ "@" "(" $Line ")")))

#define Message(desc) _Pragma(STR(message(__FILE__ "(" $Line "):" #desc)))

/**
 * @brief The below Macros print data to the terminal during compilation.
 * !TODO("");
 * !TODO(Variable);
 * !Message("");
 */

// Globally available functions
char *StringtoChar(String inputString);
char *appendChartoChar(const char *hostname, const char *def_host);
void my_delay(volatile long delay_time);
String generateDeviceID();

/*######################## MQTT Configuration ########################*/
// MQTT includes
//! Deprecated - TODO: REMOVE ME
// #include "Oldmqtt.hpp"
#include "mqtt/hassmqtt.hpp"
/*###################### MQTT Configuration END ######################*/

// Global Variables
extern const char *mqtt_mDNS_clientId;
extern int period;
extern unsigned long time_now;

// Wifi Variables
extern bool wifiConnected;

#endif // DEFINES_hpp
