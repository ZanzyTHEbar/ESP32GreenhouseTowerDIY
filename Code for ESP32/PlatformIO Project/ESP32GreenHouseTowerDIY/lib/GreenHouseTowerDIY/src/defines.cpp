#include "defines.hpp"

// Globally available functions
char *StringtoChar(String inputString)
{
  char *outputString;
  outputString = NULL;
  resizeBuff(inputString.length() + 1, &outputString);
  strcpy(outputString, inputString.c_str());
  return outputString;
}

char *appendChartoChar(const char *hostname, const char *def_host)
{
  // create hostname
  int numBytes = strlen(hostname) + strlen(def_host) + 1; // +1 for the null terminator | allocate a buffer of the required size
  char *hostname_str = NULL;
  resizeBuff(numBytes, &hostname_str);
  strcpy(hostname_str, hostname);
  strcat(hostname_str, def_host); // append default hostname to hostname
  return hostname_str;
}

void my_delay(volatile long delay_time)
{
  delay_time = delay_time * 1e6L;
  for (volatile long count = delay_time; count > 0L; count--)
    ;
}

// a function to generate the device ID and called generateDeviceID()
String generateDeviceID()
{
  uint32_t chipId = 0;
  for (int i = 0; i < 17; i = i + 8)
  {
    chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }

  log_i("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  log_i("This chip has %d cores\n", ESP.getChipCores());
  log_i("Chip ID: %d", chipId);
  String deviceID = String(chipId);
  return deviceID;
}

// Variables
const char *mqtt_mDNS_clientId = StringtoChar(DEFAULT_HOSTNAME);

int period = 500;
unsigned long time_now = 0;
bool Charge_State;
// Wifi Variables
bool wifiMangerPortalRunning = false;
bool wifiConnected = false;
