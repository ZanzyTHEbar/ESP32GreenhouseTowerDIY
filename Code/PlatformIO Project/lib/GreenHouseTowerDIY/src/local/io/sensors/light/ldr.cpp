#include "ldr.hpp"

// TODO: Fix this with a proper implementation of the LDR and lux

LDR::LDR(GreenHouseConfig& config) : config(config), _GAMMA(0.7), _RL10(50) {}

LDR::~LDR() {}

void LDR::begin() {
  switch (config.config.enabled_features.ldr_Features) {
    case GreenHouseConfig::LDRFeatures_t::NONE_LDR:
      /* code */
      break;
    case GreenHouseConfig::LDRFeatures_t::LDR:
      pinMode(LDR_PIN, INPUT);
      /* code */
      break;
    case GreenHouseConfig::LDRFeatures_t::BH1750: {  // Initialize the sensor
      // use BH1750_TO_GROUND or BH1750_TO_VCC depending how you wired the
      // address pin of the sensor.
#if bh1750_GND
      bool avail = BH1750_sensor.begin(
          BH1750_TO_GROUND);  // will be false no sensor found
#endif                        // bh1750_GND
#if bh1750_VCC
      bool avail =
          BH1750_sensor.begin(BH1750_TO_VCC);  // will be false no sensor found
#endif                                         // bh1750_VCC
      Serial.println(!avail ? "No BH1750 sensor found" : "BH1750 sensor found");
      if (!avail) {
      } else {
        Serial.println("BH1750 sensor found");
#if bh1750_FAST
        BH1750_sensor.calibrateTiming();
#endif                          // bh1750_FAST
        BH1750_sensor.start();  // start the first measurement in setup
      }
    } break;
    case GreenHouseConfig::LDRFeatures_t::ALL_LDR:
      /* code */
      break;
    default:
      break;
  }
}

float LDR::read() {
  float lux = 0;
  switch (config.config.enabled_features.ldr_Features) {
    case GreenHouseConfig::LDRFeatures_t::NONE_LDR:
      /* code */
      break;
    case GreenHouseConfig::LDRFeatures_t::LDR: {  // Convert the analog value
                                                  // into lux value:
      float readLDR = analogRead(LDR_PIN);
      float voltage = readLDR / 4096.0 * 3.3;
      float resistance = 2000.0 * voltage / (1.0 - voltage / 3.3);
      lux = pow(_RL10 * 1e3 * pow(10.0, _GAMMA) / resistance, (1.0 / _GAMMA));
      char buffer[100];
      dtostrf(lux, 10, 3, buffer);
      log_i("%s\n", buffer);
      Network_Utilities::my_delay(0.1L);
    } break;
    case GreenHouseConfig::LDRFeatures_t::BH1750: {
      if (!BH1750_sensor.hasValue())
        break;
      lux = BH1750_sensor.getLux();
      log_i("Light lux level: %.3f", lux);
      BH1750_sensor.start();
    } break;
    default:
      break;
  }
  return lux;
}

const std::string& LDR::getSensorName() {
  static std::string name = "ldr";
  return name;
}

void LDR::accept(Visitor<SensorInterface<float>>& visitor) {
  visitor.visit(this);
}