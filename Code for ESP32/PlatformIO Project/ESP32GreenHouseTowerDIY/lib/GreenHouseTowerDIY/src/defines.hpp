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

// Temp Sensors
#include "sensors/temperature/CellTemp.hpp"

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

#define STR(x) #x
#define XSTR(x) STR(x)
#define MSG(x)

#ifdef DEFAULT_HOSTNAME
#pragma message DEFAULT_HOSTNAME
#endif

#ifdef PRODUCTION
_Pragma(STR(message(PRODUCTION)))
#endif

// Globally available functions
char *StringtoChar(String inputString);
char *appendChartoChar(const char *hostname, const char *def_host);
void my_delay(volatile long delay_time);
String generateDeviceID();

/*######################## MQTT Configuration ########################*/
_Pragma(STR(message(ENABLE_MQTT_SUPPORT)))
// MQTT includes

//! Deprecated - TODO: REMOVE ME 
// #include "Oldmqtt.hpp"

#include "mqtt/hassmqtt.hpp"

/*###################### MQTT Configuration END ######################*/

/*###################### Distance Sensor Values ######################*/
#define ECHO 13
#define TRIGGER 14
/*###################### Distance Sensor Values END ######################*/


// Variables
extern const char *mqtt_mDNS_clientId;
extern int period;
extern unsigned long time_now;

// Wifi Variables
extern bool wifiConnected;

#endif // DEFINES_hpp
