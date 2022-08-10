#include "waterlevelsensor.hpp"

//***********************************************************************************************************************
// * Capacitive Water Level Sensor Class
// * This class is used to read the water level from a capacitive sensor.
// * The sensor is connected to the ESP32 via a GPIO Touch pin.
//! * Manual calibration is needed!!!
//************************************************************************************************************************

// This is the function that is called on a quick click.
void quickCallback(void)
{
    if (waterlevelSensor._activateCalibration)
    {
        if (calibrationButton.trueFalse())
        {
            // the button is not being pressed..
            return;
        }

        // the button is being pressed..
        waterlevelSensor.calibrateSensor();
    }
}

// This is the function that is called on a 10 second long click.
void holdCallback(void)
{
    // Long press has happened.
    my_delay(1L);
    waterlevelSensor._activateCalibration = !waterlevelSensor._activateCalibration;
}

// This is the function that is called on a 5 second long click.
void longholdCallback(void)
{
    // Long press has happened.
    if (waterlevelSensor._activateCalibration)
    {
        if (calibrationButton.trueFalse())
        {
            // the button is not being pressed..
            return;
        }

        // the button is being pressed..
        waterlevelSensor.setCapSensorRange();
    }
}

WaterLevelSensor::WaterLevelSensor() : _activateCalibration(false),
                                       _depth(0),
                                       _depthRange(0),
                                       _qNumberReadings{0},
                                       _depthArray{0},
                                       _qNumberDepth{0},
                                       _distanceSensor{std::make_shared<UltraSonicDistanceSensor>(ECHO_PIN, TRIG_PIN)} {}

WaterLevelSensor::~WaterLevelSensor() {}

void WaterLevelSensor::begin()
{
    _radius = RES_RADIUS_1;
    _height = RES_HEIGHT;
#if USE_CAP
    _calibration = {CAP_MAX, CAP_MIN};
    calibrationButton.setCallback(quickCallback);               // When the button gets clicked. Call this function.
    calibrationButton.setLongCallback(holdCallback);            // If it's been held down for a "long" time call this.
    calibrationButton.setCalibrationCallback(longholdCallback); // If it's been held down for a "long" time call this.
#endif                                                          // USE_CAP
}

int WaterLevelSensor::readWaterLevelCapacitive()
{
    if ((_calibration._min && _calibration._max) == 0)
    {
        log_e("The capacitive sensor is not calibrated, please calibrate by pressing the calibrate button. Exiting...");
        return 0;
    }

    int touchValue = touchRead(CAP_WATER_LEVEL_SENSOR_PIN);
    log_d("Water Level Sensor Raw Readings: %d", touchValue);
    return touchValue;
}

/*
multiMap myMultiMap;          // These start out empty..
myMultiMap.addPoint(X1,Y1);   // Then you take your x,y data table..
myMultiMap.addPoint(X2,Y2);   // And add all the points for your curve.
myMultiMap.addPoint(X3,Y3);
myMultiMap.addPoint(X4,Y4);
float mappedValue = myMultiMap.map(inputValue);  // Once loaded, use it the same way.
 */
int WaterLevelSensor::getPercentage()
{
    //! TODO: Replace with MultiMap as map is linear and touch read is not.
    int percentage = map(getWaterLevel(), _calibration._min, _calibration._max, 0, 100);
    log_i("Water Level Sensor Percentage: %d", percentage);
    return percentage;
}

//***********************************************************************************************************************
//* Firstly, we need to input the max and min values of the capacitive sensor.
//* Only after this step is completed, can you move on.
//* 1. Place ruler or tape measure in the water next to the sensor.
//* 2a. Plug the sensor into the PCB and the PCB into the computer. Press and hold the calibration button for 10 seconds.
//* 2b. Please insert your sensor into the water and submerge according to getting the min and max values.
//* 2c. After each measurement, please press the calibration button and hold for 5 seconds.
//* 2d. When the above step is completed, please press the calibration button and hold for 10 second to enter the next step.
//* 3. Press the calibrate sensor button (do not hold), wait for the sensor to read a value, and then lower the sensor by 1cm.
//* 4. Repeat step #3 for each depth (this is to mitigate error) until the sensor is fully submerged.
//* 5. Press and hold the sensor calibrate button for 1 second to exit calibration.
//************************************************************************************************************************
void WaterLevelSensor::calibrateSensor()
{
    _depth++;
    _depthArray[_depth] = _depth;
    byte numtoaverage = 5;
    int _readings[] = {0};

    for (byte i = 0; i < numtoaverage; i++)
    {
        _readings[i] += getWaterLevel();
        my_delay(0.1L);
    }

    for (byte i = 0; i < sizeof(_readings) / _readings[0]; i++)
    {
        _readings[i] = _readings[i] / numtoaverage;
    }

    //! FIX THIS - ADD SUPPORT FOR SMALLER BUCKETS
    //* 34 is the height in cm of an average 5gallon (25L) bucket.
    if (_depth > 34)
    {
        convertToQNumber(_readings, _depthArray);
    }
}

void WaterLevelSensor::setCapSensorRange()
{
    byte numtoaverage = 5;
    int _readings = 0;
    byte exeCount = 0;
    int touchValue = touchRead(CAP_WATER_LEVEL_SENSOR_PIN);

    exeCount++;

    if ((_depthRange >= 1) && (exeCount >= 2))
    {
        log_w("[Warning]: This function should only be called once.");
        return;
    }

    for (byte i = 0; i < numtoaverage; i++)
    {
        _readings += touchValue;
        my_delay(0.1L);
    }

    for (byte i = 0; i < numtoaverage; i++)
    {
        _readings = _readings / numtoaverage;
    }

    if (_depthRange == 0)
    {

        _calibration._min = _readings;
    }
    else if (_depthRange == 1)
    {
        _calibration._max = _readings;
    }
    else
    {
        log_e("[Error]: The depth range is out of bounds.");
        return;
    }
    _depthRange++;
}

void WaterLevelSensor::convertToQNumber(int *readings, int *depth)
{
    for (byte i = 0; i < sizeof(readings) / readings[0]; i++)
    {
        _qNumberReadings[i] = round(readings[i] * pow(2, 8));
        _qNumberDepth[i] = round(depth[i] * pow(2, 8));
    }
}

int WaterLevelSensor::convertToQNumber(int readings)
{
    int qNumber = 0;
    qNumber = round(readings * pow(2, 8));
    return qNumber;
}

double WaterLevelSensor::readSensor()
{
    double distance = _distanceSensor->measureDistanceCm(tower_temp.temp_sensor_results.temp[0]);
    log_d("Distance: %.3f cm", distance, DEC);
    // Every 1 second, do a measurement using the sensor and print the distance in centimeters.
    my_delay(1L);
    return distance;
}

int WaterLevelSensor::readWaterLevelUltraSonic()
{
    if (readSensor() <= 0.0)
    {
        log_i("Failed to read ultrasonic sensor.");
        return 0.0;
    }

    // tank diameter and height, in centimeters
    double diameter = pow(_radius, 2.0);
    double height = _height;
    double v = ((diameter * PI) * (height) / 1000.0);
    double stock = ((diameter * PI) * (height - readSensor()) / 1000.0);
    log_i("Stock is: %.3f liters", stock, DEC);
    double p = (stock / v) * 100.0;

    log_i("Percent Full: %.3f", p, DEC);
    log_d("True Water Level Distance: %.3f cm", readSensor(), DEC);

    if (isnan(p))
    {
        log_e("Error: %s", "Sensor Value is NaN");
        return 0.0;
    }

    // pause between readings, in seconds:
    my_delay(30L);
    return (int)p;
}

int WaterLevelSensor::getWaterLevel()
{
#if USE_CAP
    return getPercentage();
#elif USE_UC
    return readWaterLevelUltraSonic();
#endif // USE_CAP
}

WaterLevelSensor waterlevelSensor;