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
 * Description: This function accumulates all sensor data and stores it in the main json data structure.
 * Parameters: None
 * Return: None
 ******************************************************************************/
void AccumulateData::InitAccumulateData()
{
    // Initialize the library
#if USE_SHT31_SENSOR
    humidity.ReadSensor();
    cfg.config.stack_humidity = humidity.StackHumidity();
    cfg.config.stack_temp = humidity.AverageStackTemp();
#endif // USE_SHT31_SENSOR

    cfg.config.numSensors = numSensors;

    // loop through and store temp data
    for (int i = 0; i < numSensors; i++)
    {
        cfg.config.cell_temp[i] = cell_temp_sensor_results.temp[i];
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
    // Send the data to the server
}

AccumulateData accumulatedata;
