#include "HMS.hpp"

// ESP 32 (requires resistors to step down the logic voltage)
// ACS712  ACS(25, 5.0, 4095, 185);
#if !PRODUCTION

uint8_t _amppin = 33;

#else
uint8_t _amppin = 10;
#endif

ACS712 ACS(_amppin, 5.0, 4095, 100);

bool mux_enabled_voltage = false;
bool mux_enabled_amps = false;
int power_mux_pin_amps = 20;
int power_mux_pin_voltage = 46;
bool POWER_MUX_ENABLED_AMPS = digitalRead(power_mux_pin_amps);
bool POWER_MUX_ENABLED_VOLTAGE = digitalRead(power_mux_pin_voltage);
float *cell_voltage = (float *)malloc(sizeof(float) * maxCellCount);

HMS::HMS()
{
}

HMS::~HMS()
{
}

/******************************************************************************
 * Function: Read Voltage per Cell
 * Description: Reads the voltage from the cell and returns the value
 * Parameters: int pinnumber
 * Return: float
 ******************************************************************************/
float HMS::readVoltage(int pinnumber)
{
    return (float)((analogRead(pinnumber) * 3300.0) / 4095.0);
}

/******************************************************************************
 * Function: Read Voltage per Cell and average
 * Description: This function loops through 5 times and reads the voltage from each celland stores it in a float array
 * Parameters: None
 * Return: float array
 ******************************************************************************/
float *HMS::readSensAndCondition()
{
    int numtoaverage = 10;
    for (int i = 0; i < numtoaverage; i++)
    {
        if (!PRODUCTION)
        {
            cell_voltage[0] = readVoltage(36);
            cell_voltage[1] = readVoltage(39);
            cell_voltage[2] = readVoltage(34);
            cell_voltage[3] = readVoltage(35); // voltage leads on analog pins ADC1 - ADC2 pins do not work when wifi is enabled
            cell_voltage[4] = readVoltage(32);

            digitalWrite(power_mux_pin_amps, LOW);
            my_delay(0.1L);
            if (!mux_enabled_amps && !POWER_MUX_ENABLED_AMPS)
            {
                mux_enabled_voltage = true;
                digitalWrite(power_mux_pin_voltage, HIGH);
                cell_voltage[5] = readVoltage(33);
            }
            digitalWrite(power_mux_pin_voltage, LOW);
            my_delay(0.1L);
            mux_enabled_voltage = false;
        }
        else
        {
            cell_voltage[0] = readVoltage(1);
            cell_voltage[1] = readVoltage(2);
            cell_voltage[2] = readVoltage(3);
            cell_voltage[3] = readVoltage(4); // voltage leads on analog pins ADC1 - ADC2 pins do not work when wifi is enabled
            cell_voltage[4] = readVoltage(5);
            cell_voltage[5] = readVoltage(6);
            cell_voltage[6] = readVoltage(7);
            cell_voltage[7] = readVoltage(8);
            cell_voltage[8] = readVoltage(9);
            digitalWrite(power_mux_pin_amps, LOW);
            my_delay(0.1L);
            if (!mux_enabled_amps && !POWER_MUX_ENABLED_AMPS)
            {
                mux_enabled_voltage = true;
                digitalWrite(power_mux_pin_voltage, HIGH);
                cell_voltage[9] = readVoltage(10);
            }
            digitalWrite(power_mux_pin_voltage, LOW);
            my_delay(0.1L);
            mux_enabled_voltage = false;
        }
    }

    for (int i = 0; i < numtoaverage; i++)
    {
        cell_voltage[i] = cell_voltage[i] / numtoaverage;
    }
    return cell_voltage;
}

/******************************************************************************
 * Function: Setup the ACS712 sensor
 * Description: This function setups the ACS712 sensor by calculating the automidpoint and setting the sensitivity to the correct value
 * Parameters: None
 * Return: None
 * ACS712 5A  uses 185 mV per A
 * ACS712 20A uses 100 mV per A
 * ACS712 30A uses  66 mV per A
 ******************************************************************************/
void HMS::setupSensor()
{
    // Setup the ACS712 sensor
    digitalWrite(power_mux_pin_voltage, LOW);
    if (!mux_enabled_voltage && !POWER_MUX_ENABLED_VOLTAGE)
    {
        mux_enabled_amps = true;
        digitalWrite(power_mux_pin_amps, HIGH);
        ACS.autoMidPoint();
        my_delay(0.1L);
        digitalWrite(power_mux_pin_amps, LOW);
        mux_enabled_amps = false;
    }
}

/******************************************************************************
 * Function: Read MilliAmps for stack
 * Description: This function reads the current from the ACS712 sensor and returns the value
 * Parameters: None
 * Return: Integer
 *************************
 *****************************************************/
int HMS::readAmps()
{
    // Setup the ACS712 sensor
    digitalWrite(power_mux_pin_voltage, LOW);
    if (!mux_enabled_voltage && !POWER_MUX_ENABLED_VOLTAGE)
    {
        mux_enabled_amps = true;
        digitalWrite(power_mux_pin_amps, HIGH);
        int mA = ACS.mA_DC();
        String amps = String(mA);
        digitalWrite(power_mux_pin_amps, LOW);
        mux_enabled_amps = false;
        Serial.print("Stack mA:");
        Serial.println(amps);
        my_delay(0.1L);
        return amps.toInt();
    }
    return 0;
}

/******************************************************************************
 * Function: Calibrate the ACS712 sensor
 * Description: This function allows for manual calibration of the ACS712 sensor by setting the sensitivity to the correct value
 * Parameters: None
 * Return: None
 ******************************************************************************/
void HMS::calibrateAmps()
{
    // Setup the ACS712 sensor
    digitalWrite(power_mux_pin_voltage, LOW);
    if (!mux_enabled_voltage && !POWER_MUX_ENABLED_VOLTAGE)
    {
        mux_enabled_amps = true;
        digitalWrite(power_mux_pin_amps, HIGH);
        if (Serial.available())
        {
            char c = Serial.read();
            switch (c)
            {
            case '+':
                ACS.incMidPoint();
                break;
            case '-':
                ACS.decMidPoint();
                break;
            case '0':
                ACS.setMidPoint(512);
                Serial.print("," + String(ACS.getMidPoint()));
                break;
            case '*':
                ACS.setmVperAmp(ACS.getmVperAmp() * 1.05);
                break;
            case '/':
                ACS.setmVperAmp(ACS.getmVperAmp() / 1.05);
                Serial.print("," + String(ACS.getmVperAmp()));
                break;
            default:
                Serial.println("No input detected");
            }
        }
        my_delay(0.01L);
        digitalWrite(power_mux_pin_amps, LOW);
        mux_enabled_amps = false;
        my_delay(0.1L);
    }
}

HMS HMSmain;