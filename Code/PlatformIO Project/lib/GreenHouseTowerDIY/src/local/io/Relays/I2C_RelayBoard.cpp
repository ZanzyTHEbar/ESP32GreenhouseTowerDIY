#include <Wire.h>
#include "Adafruit_MCP23008.h"

Adafruit_MCP23008 relay;

void setup() {
  relay.begin(0x20);      // use default address 0
  relay.pinMode(0, OUTPUT);
  relay.pinMode(1, OUTPUT);
  relay.pinMode(2, OUTPUT);
  relay.pinMode(3, OUTPUT);
  relay.pinMode(4, OUTPUT);
  relay.pinMode(5, OUTPUT);
  relay.pinMode(6, OUTPUT);
  relay.pinMode(7, OUTPUT);
}

void loop() {
  relay.digitalWrite(0,HIGH);
  delay(500);
  relay.digitalWrite(0,LOW);

  relay.digitalWrite(1,HIGH);
  delay(500);
  relay.digitalWrite(1,LOW);

  relay.digitalWrite(2,HIGH);
  delay(500);
  relay.digitalWrite(2,LOW);

  relay.digitalWrite(3,HIGH);
  delay(500);
  relay.digitalWrite(3,LOW);

  relay.digitalWrite(4,HIGH);
  delay(500);
  relay.digitalWrite(4,LOW);

  relay.digitalWrite(5,HIGH);
  delay(500);
  relay.digitalWrite(5,LOW);

  relay.digitalWrite(6,HIGH);
  delay(500);
  relay.digitalWrite(6,LOW);

  relay.digitalWrite(7,HIGH);
  delay(500);
  relay.digitalWrite(7,LOW);
}
