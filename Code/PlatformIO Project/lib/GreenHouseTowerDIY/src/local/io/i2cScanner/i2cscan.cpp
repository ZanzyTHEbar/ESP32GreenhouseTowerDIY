#include "i2cscan.hpp"

Scanner::Scanner(void) {}

Scanner::~Scanner(void) {}

void Scanner::SetupScan() {
  Serial.println(F("\nI2C Scanner Setup"));
}

void Scanner::BeginScan() {
  byte error, address;
  int nDevices;
  Serial.println(F("Scanning..."));
  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print(F("I2C device found at address 0x"));
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      nDevices++;
    } else if (error == 4) {
      Serial.print(F("Unknow error at address 0x"));
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println(F("No I2C devices found\n"));
  } else {
    Serial.println(F("done\n"));
  }
  delay(5000);  // wait 5 seconds for next scan
}

Scanner Scan;
