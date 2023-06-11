#include "towertemp.hpp"

TowerTemp::Temp temp_sensor_results;

TowerTemp::TowerTemp()
    : _sensors_count(0), oneWire(ONE_WIRE_BUS), sensors(&oneWire) {}

TowerTemp::~TowerTemp() {}

void TowerTemp::setSensorCount() {
  _sensors_count =
      sensors.getDeviceCount();  // returns the number of sensors found
}

int TowerTemp::getSensorCount() {
  return _sensors_count;
}

//******************************************************************************
// * Function: Setup DS18B20 sensors
// * Description: Setup DS18B20 sensors by beginning the Dallas Temperature
// library and counting the connected sensors
// * Parameters: None
// * Return: None
//******************************************************************************
bool TowerTemp::begin() {
  // Start up the ds18b20 library
  sensors.begin();
  setSensorCount();
  log_d(" Requesting temperatures...");
  sensors.requestTemperatures();

  log_d("Temperature is: %.3f", sensors.getTempCByIndex(0));

  size = sizeof(temp_sensor_addresses) / sizeof(temp_sensor_addresses[0]);
  // You can have more than one DS18B20 on the same bus.
  // 0 refers to the first IC on the wire

  // handle the case where no sensors are connected
  log_i("Locating devices...");
  if (_sensors_count == 0) {
    log_e(
        "No temperature sensors found - please connect them and restart the "
        "device");
    return false;
  }
  // locate devices on the bus
  log_i("Found %d devices", _sensors_count, DEC);

  // Loop through each device, print out address
  readAddresses();
  return true;
}

void TowerTemp::readAddresses() {
  for (int i = 0; i < _sensors_count; i++) {
    // Search the wire for address
    if (sensors.getAddress(temp_sensor_addresses, i)) {
      log_i("Found device index %d with address: %s", i,
            printAddress(temp_sensor_addresses, size).c_str(), DEC);
    } else {
      log_w(
          "Found ghost device at %d but could not detect address. Check power "
          "and cabling",
          i, DEC);
    }
  }
}

//******************************************************************************
// * Function: Print Address
// * Description: Print the addresses of the sensors
// * Parameters: DeviceAddress - Address of the sensor
// * Return: None
//******************************************************************************
// function to print a device address
std::string TowerTemp::printAddress(DeviceAddress deviceAddress, size_t size) {
  char hexstr[size * 2 + 1];
  for (uint8_t i = 0; i < _sensors_count; i++) {
    if (deviceAddress[i] < 16)
      log_d("0");
    for (uint8_t j = 0; j < size; j++) {
      snprintf(hexstr + j * 2, sizeof(hexstr), "%02x", deviceAddress[j]);
    }
    log_i("%s", hexstr);
  }
  std::string str(hexstr);
  return str;
}

void TowerTemp::checkSensors() {
  if (_sensors_count == 0) {
    float no_sensors[] = {0};
    for (int i = 0; i < _sensors_count; i++) {
      temp_sensor_results.temp.push_back(no_sensors[i]);
    }
    log_i(
        "No temperature sensors found - please connect them and restart the "
        "device");
  }
}

//******************************************************************************
// * Function: Get Temperature
// * Description: Get the temperatures of the sensors and allocate the memory
// for the temperatures
// * Parameters: None
// * Return: float array - Temperature of the sensors
//******************************************************************************
TowerTemp::Temp TowerTemp::getTempC() {
  // handle the case where no sensors are connected
  checkSensors();
  sensors.requestTemperatures();
  for (int i = 0; i < _sensors_count; i++) {
    // Search the wire for address
    if (sensors.getAddress(temp_sensor_addresses, i)) {
      temp_sensor_results.temp.push_back(
          sensors.getTempC(temp_sensor_addresses));
      printAddress(temp_sensor_addresses, size);
    } else {
      log_w(
          "Found ghost device at %d but could not detect address. Check power "
          "and cabling",
          i, DEC);
    }
  }
  return temp_sensor_results;
}

//******************************************************************************
// * Function: Get Temperature
// * Description: Get the temperatures of the sensors and allocate the memory
// for the temperatures
// * Parameters: None
// * Return: float array - Temperature of the sensors in fahrenheit
//******************************************************************************
TowerTemp::Temp TowerTemp::getTempF() {
  // handle the case where no sensors are connected
  checkSensors();
  sensors.requestTemperatures();
  for (int i = 0; i < _sensors_count; i++) {
    // Search the wire for address
    if (sensors.getAddress(temp_sensor_addresses, i)) {
      temp_sensor_results.temp.push_back(
          sensors.getTempC(temp_sensor_addresses) * (9.0 / 5.0) + 32.0);
      printAddress(temp_sensor_addresses, size);
    } else {
      log_w(
          "Found ghost device at %d but could not detect address. Check power "
          "and cabling",
          i, DEC);
    }
  }
  return temp_sensor_results;
}