#include "accumulatedata.hpp"

int numSensors = 10;

AccumulateData::AccumulateData()
{
    maxVoltage = 24;
    maxTemp = 100;
}

AccumulateData::~AccumulateData()
{
}

/******************************************************************************
 * Function: Accumulate Data to send from sensors and store in json
 * Description: This function accumulates all sensor data and stores it in the main data structure.
 * Parameters: None
 * Return: None
 ******************************************************************************/
void AccumulateData::InitAccumulateData()
{
    cfg.config.numSensors = numSensors;

    // Initialize the library
#if USE_SHT31_SENSOR
    humidity.ReadSensor();
    config.humidity_sht31_average = humidity.StackHumidity();
    config.humidity_temp_sht31_average = humidity.AverageStackTemp();
    switch (HUMIDITY_SENSORS_ACTIVE)
    {
    case 0:
        config.humidity_sht31 = 0;
        config.humidity_sht31_2 = 0;
        config.humidity_temp_sht31 = 0;
        config.humidity_temp_sht31_2 = 0;
        break;
    case 1:
        config.humidity_sht31 = humidity.result.humidity_sht31;
        config.humidity_sht31 = humidity.result.temp_sht31;
        break;
    case 2:
        config.humidity_sht31_2 = humidity.result.humidity_sht31_2;
        config.humidity_temp_sht31_2 = humidity.result.temp_sht31_2;
        break;
    case 3:
        config.humidity_sht31 = humidity.result.humidity_sht31;
        config.humidity_sht31 = humidity.result.temp_sht31;
        config.humidity_sht31_2 = humidity.result.humidity_sht31_2;
        config.humidity_temp_sht31_2 = humidity.result.temp_sht31_2;
        break;
    default:
        config.humidity_sht31 = 0;
        config.humidity_sht31_2 = 0;
        config.humidity_temp_sht31 = 0;
        config.humidity_temp_sht31_2 = 0;
        break;
    }
#endif // USE_SHT31_SENSOR

#if USE_DHT_SENSOR
    humidity.readDHT();
    config.humidity = humidity.result.humidity;
    config.humidity_temp = humidity.result.temp;
#endif // USE_DHT_SENSOR

    phsensor.phSensorLoop();

    // loop through and store temp data
    for (int i = 0; i < numSensors; i++)
    {
        config.temp_sensors[i] = tower_temp.temp_sensor_results.temp[i];
    }

    // Relays
    for (int i = 0; i < sizeof(cfg.config.relays_pin) / sizeof(cfg.config.relays_pin[0]); i++)
    {
        Relay.RelayOnOff(cfg.config.relays_pin[i], cfg.config.relays[i]);
        log_d("Relay on pin: %d is %s", cfg.config.relays_pin[i], cfg.config.relays[i] ? "on" : "off");
    }
}

bool AccumulateData::SendData()
{
    // Send the data to the server - Use ArduinoJson library
}

AccumulateData accumulatedata;
