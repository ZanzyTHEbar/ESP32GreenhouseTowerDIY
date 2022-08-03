#include <defines.hpp>

unsigned long PreviousMillis = 0;

// Globally available functions
char *StringtoChar(String inputString)
{
  char *outputString;
  outputString = NULL;
  resizeBuff(inputString.length() + 1, &outputString);
  strcpy(outputString, inputString.c_str());
  return outputString;
}

char *appendChartoChar(const char *first, const char *second)
{
  int numBytes = strlen(first) + strlen(second) + 1; // +1 for the null terminator | allocate a buffer of the required size
  char *first_str = NULL;
  resizeBuff(numBytes, &first_str);
  strcpy(first_str, first);
  strcat(first_str, second); // append second to first
  return first_str;
}

void my_delay(volatile long delay_time)
{
  delay_time = delay_time * 1e6L;
  for (volatile long count = delay_time; count > 0L; count--)
    ;
}

void custom_delay(int time)
{
  unsigned long currentMillis = millis();
  if (currentMillis - PreviousMillis >= time)
  {
    PreviousMillis = currentMillis;
    return;
  }
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
