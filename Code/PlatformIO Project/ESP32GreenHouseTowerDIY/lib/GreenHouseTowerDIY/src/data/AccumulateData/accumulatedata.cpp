#include "accumulatedata.hpp"

AccumulateData::AccumulateData() : _maxTemp(100), _numTempSensors(0), inList(false)
{
}

AccumulateData::~AccumulateData()
{
    // Before we die, we need to tell our master to let us go.
    timedTasks.unlinkObj(this);
}

void AccumulateData::addSelf(void)
{
    if (!inList)
    {
        timedTasks.addToTop(this);
        inList = true;
    }
}

void AccumulateData::begin()
{
    addSelf();
    config = {
        .humidity = 0.0,
        .humidity_temp = 0.0,
        .humidity_sht31 = 0.0,
        .humidity_sht31_2 = 0.0,
        .humidity_temp_sht31 = 0.0,
        .humidity_temp_sht31_2 = 0.0,
        .humidity_sht31_average = 0.0,
        .humidity_temp_sht31_average = 0.0,
        .temp_sensors = {0.0},
        .flow_rate = 0,
        .flow_rate_sensor_temp = 0,
        .water_level = 0};
}

//******************************************************************************
// * Function: Accumulate Data to send from sensors and store in json
// * Description: This function accumulates all sensor data and stores it in the main data structure.
// * Parameters: None
// * Return: None
//******************************************************************************/
void AccumulateData::InitAccumulateData()
{
    _numTempSensors = tower_temp.getSensorCount();
    cfg.config.numTempSensors = _numTempSensors;

    // Initialize the library
#if USE_SHT31_SENSOR
    humidity.ReadSensor();
    config.humidity_sht31_average = humidity.StackHumidity();
    config.humidity_temp_sht31_average = humidity.AverageStackTemp();
    switch (humidity._HUMIDITY_SENSORS_ACTIVE)
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

#if ENABLE_PH_SUPPORT
    phsensor.phSensorLoop();
#endif // ENABLE_PH_SUPPORT

    // loop through and store temp data
    for (int i = 0; i < _numTempSensors; i++)
    {
        config.temp_sensors[i] = tower_temp.temp_sensor_results.temp[i];
    }

    config.water_level = waterlevelSensor.getWaterLevel();

    // Relays
    for (int i = 0; i < sizeof(cfg.config.relays_pin) / sizeof(cfg.config.relays_pin[0]); i++)
    {
        Relay.RelayOnOff(cfg.config.relays_pin[i], cfg.config.relays[i]);
        log_d("Relay on pin: %d is %s", cfg.config.relays_pin[i], cfg.config.relays[i] ? "on" : "off");
    }
}

bool AccumulateData::SendData()
{
    String json = "";
    json += R"====({)====";

    json += R"====("stack_humidity":)====";
    json += (String)cfg.config.stack_humidity + ",\n";

    json += R"====("stack_temp":)====";
    json += (String)cfg.config.stack_temp + ",\n";

    json += R"====("relays":[)====";
    json += (String)cfg.config.relays[0] + "," + (String)cfg.config.relays[1] + "," + (String)cfg.config.relays[2] + "," + (String)cfg.config.relays[3] + "," + (String)cfg.config.relays[4] + "],\n";

    json += R"====("stack_voltage":)====";
    json += (String)cfg.config.stack_voltage + ",\n";

    json += R"====("mqtt_enable":)====";
    json += (String)cfg.config.MQTTEnabled + ",\n";

    json += R"====("charge_status":)====";
    json += (String)chargeState + ",\n";

    json += R"====("GraphData":[)====";
    json += "\n";
    for (int i = 0; i < 10; i++)
    {
        delay(0);
        json += R"====({"label": "🌡 )====" + (String)i + "\",\n";
        json += R"====("type": "temp",)====" + (String) "\n";
        json += R"====("value": )====" + (String)cfg.config.cell_temp[i] + (String) ",\n";
        json += R"====("maxValue": )====" + (String)_maxTemp;
        json += R"====(})====" + (String) "\n";
        json += R"====(,)====";

        json += R"====({"label": "⚡ )====" + (String)i + "\",\n";
        json += R"====("type": "volt",)====" + (String) "\n";
        json += R"====("value": )====" + (String)cfg.config.cell_voltage[i] + (String) ",\n";
        json += R"====("maxValue": )====" + (String)_maxVoltage;
        json += R"====(})====" + (String) "\n";

        if (i < 9)
        {
            json += R"====(,)====";
        };
    }
    json += R"====(])====";
    json += R"====(})====";

    if (json.length() > 0)
    {
        cfg.config.data_json_string = json;
        return true;
    }
    else
    {
        return false;
    }
}

AccumulateData accumulatedata;
