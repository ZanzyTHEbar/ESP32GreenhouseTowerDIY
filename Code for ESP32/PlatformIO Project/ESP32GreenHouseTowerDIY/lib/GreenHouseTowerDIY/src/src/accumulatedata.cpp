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
    // humidity.ReadSensor();
    if (numSensors > maxCellCount)
    {
        numSensors = maxCellCount;
    }

    /******************************************************************************
     * Function: Setup the Stack Climate Data
     * Description: This function setups the stack climate data.
     * Parameters: None
     * Return: The mean Stack Climate Data
     ******************************************************************************/
    // Stack Data to send
    cfg.config.stack_humidity = humidity.StackHumidity();
    cfg.config.stack_temp = humidity.AverageStackTemp();
    cfg.config.stack_current = HMSmain.readAmps();

    // Add arrays for Cell level Data.
    float *cell_voltage = HMSmain.readSensAndCondition();
    // loop through and store per cell voltage
    for (int i = 0; i < numSensors; i++)
    {
        cfg.config.cell_voltage[i] = cell_voltage[i];
    }

    /******************************************************************************
     * Function: Setup the Stack Voltage
     * Description: This function setups the mean Stack Voltage by calculating the sum of the cell_voltage array
     * Parameters: None
     * Return: The mean Stack Voltage
     ******************************************************************************/
    float array[10];
    float sum = 0;
    int size = sizeof(cell_voltage) / sizeof(cell_voltage[0]);
    for (int i = 0; i < 10; i++)
    {
        array[i] = cell_voltage[i];
    }

    for (int i = 0; i < size; i++)
    {
        sum += array[i];
    }
    Serial.print("Stack Voltage: ");
    Serial.println(sum);

    cfg.config.stack_voltage = sum;

    // free(&cell_voltage); // free the memory
    cfg.config.cell_count_max = maxCellCount;
    cfg.config.numSensors = numSensors;

    // Flow Rate dataTosend
    cfg.config.flow_rate = humidity.loopSFM3003();
    // returns a float array of cell temperatures

    // loop through and store per cell temp data

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

/******************************************************************************
 * Function: Return Charge Status of Stack
 * Description: This function reads the current stack voltage and returns a number representing the charge status
 * Parameters: None
 * Return: String
 ******************************************************************************/
int AccumulateData::ChargeStatus()
{
    if (cfg.config.stack_voltage < 8.00 && cfg.config.stack_voltage >= 0.00)
    {
        return 1;
        log_i("Stack is fully discharged");
    }
    else if (cfg.config.stack_voltage < 10.00 && cfg.config.stack_voltage >= 8.00)
    {
        return 2;
        log_i("Stack needs to be charged");
    }
    else if (cfg.config.stack_voltage < 13.00 && cfg.config.stack_voltage >= 11.0)
    {
        return 3;
        log_i("Stack has a full charge");
    }
    else if (cfg.config.stack_voltage < 14.0 && cfg.config.stack_voltage >= 12.0)
    {
        return 4;
        log_i("Stack is charging");
    }
    else if (cfg.config.stack_voltage < 16.0 && cfg.config.stack_voltage >= 14.0)
    {
        return 5;
        log_w("[DANGER]: Stack has encountered an overcharge condition");
    }

    else
    {
        return 0;
        log_w("[DANGER]: Stack is in an unknown state!!");
    }
    return 0;
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

    json += R"====("GraphData":[)====";
    json += "\n";
    for (int i = 0; i < 10; i++)
    {
        delay(0);
        json += R"====({"label": "🌡 )====" + (String)i + "\",\n";
        json += R"====("type": "temp",)====" + (String) "\n";
        json += R"====("value": )====" + (String)cfg.config.cell_temp[i] + (String) ",\n";
        json += R"====("maxValue": )====" + (String)maxTemp;
        json += R"====(})====" + (String) "\n";
        json += R"====(,)====";

        json += R"====({"label": "⚡ )====" + (String)i + "\",\n";
        json += R"====("type": "volt",)====" + (String) "\n";
        json += R"====("value": )====" + (String)cfg.config.cell_voltage[i] + (String) ",\n";
        json += R"====("maxValue": )====" + (String)maxVoltage;
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
