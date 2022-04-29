import gc
import time
from machine import Pin
import machine
import onewire
import ds18x20
from umqtt.simple import MQTTClient
import ubinascii
from ESP32Micro.lib import hcsr04, powersave
import math
import micropython
import esp
import dht
esp.osdebug(None)
gc.collect()

mqtt_server = '192.168.1.247'


client_id = ubinascii.hexlify(machine.unique_id())

topic_pub_temp = b'esp/ds18b20/temperature'
topic_pub_hum = b'esp/dht/humidity'
topic_pub_airtemp = b'esp/dht/temp'
topic_pub_waterlevel = b'esp/uc/waterlevel'
topic_pub_stocklevel = b'esp/uc/stock'


last_message = 0
message_interval = 5


ds_pin = machine.Pin(13)
ds_sensor = ds18x20.DS18X20(onewire.OneWire(ds_pin))

# DRY = 595 # Measure value for dry sensor and change accordingly
# WET = 239 # Measure value of wet sensor and change accordingly
sensor = dht.DHT11(machine.Pin(14))
# soilsensor = machine.ADC(machine.Pin(36)) # creating analog sensor object
# soilsensor.atten(machine.ADC.ATTN_11DB) # creating analog sensor breadth
# soilsensorvalue = soilsensor.read()

one = machine.Pin(15, machine.Pin.OUT)
two = machine.Pin(12, machine.Pin.OUT)
newmin = 0
newmax = 100

# Translate scale of soil sensor from 0% to 100%
# old_value = soilsensor_value
# new_min = 0
# new_max = 100
# new_value = ( (old_value - WET) / (DRY - WET) ) * (new_max - new_min) + new_min

UC = hcsr04.HCSR04(trigger_pin=33, echo_pin=5, echo_timeout_us=1000000)
led1 = machine.Pin(15, machine.Pin.OUT)
led2 = machine.Pin(2, machine.Pin.OUT)
led3 = machine.Pin(4, machine.Pin.OUT)

off = 0
on = 1

# tank diameter and height, in centimeters
radius = pow(100, 2)
height = 200

# pause between readings, in seconds:
readings = 30


def read_waterlevel():
    try:
        distance = int(UC.distance_cm())
        v = ((radius*math.pi)*(height)/1000)
        stock = ((radius*math.pi)*(height-distance)/1000)
        print('Stock is:', stock, 'liters')
        p = (stock/v)*100

        print('Percent Full: {}%'.format(p))
        print('True Distance:', distance, 'cm')

        if p <= 25:
            led3.value(off)
            led2.value(off)
            led1.value(on)
        elif p > 25 and p < 50:
            led3.value(off)
            led1.value(off)
            led2.value(on)
        elif p > 75:
            led1.value(off)
            led2.value(off)
            led3.value(on)
        else:
            led3.value(off)
            led2.value(off)
            led1.value(off)

        client.publish(topic_pub_stocklevel, str(stock))

        if (isinstance(p, float) or (isinstance(p, int))):
            p = (b'{0:3.1f},'.format(p))
            client.publish(topic_pub_waterlevel, p)
            return p
        else:
            return('Invalid sensor readings.')
    except OSError as e:
      return('Failed to read ultrasonic sensor.')


def connect_mqtt():
    global client_id, mqtt_server
    client = MQTTClient(client_id, mqtt_server)
    #client = MQTTClient(client_id, mqtt_server, user=your_username, password=your_password)
    client.connect()
    print('Connected to %s MQTT broker' % (mqtt_server))
    return client


def restart_and_reconnect():
    print('Failed to connect to MQTT broker. Reconnecting...')
    time.sleep(10)
    machine.reset()


def read_sensor():
    try:
        roms = ds_sensor.scan()
        ds_sensor.convert_temp()
        time.sleep_ms(750)
        for rom in roms:
            temp = ds_sensor.read_temp(rom)
            # uncomment for Fahrenheit
            # temp = temp * (9/5) + 32.0
        if (isinstance(temp, float) or (isinstance(temp, int))):
            temp = (b'{0:3.1f},'.format(temp))
            return temp
        else:
            return('Invalid sensor readings.')
    except OSError as e:
        return('Failed to read Water Temp sensor.')


# def sensor1():
        # two.off()
        # one.on() # Turn sensor1 On
        # time.sleep(0.10) # Wait for sensor
        # sensorvalue = soilsensorvalue # Read Analog pin as sensor1
        # one.value(0) # Turn sensor1 Off
        #oldvalueone = sensorvalue
        #newvalueone = ( (oldvalueone - WET) / (DRY - WET) ) * (newmax - newmin) + newmin
        #print("Sensor1 =", newvalueone,'%')


# def sensor2():
        # one.off()
        # two.on() # Turn sensor1 On
        # time.sleep(0.10) # Wait for sensor
        # sensorvaluetwo = soilsensorvalue # Read Analog pin as sensor1
        # two.off() # Turn sensor1 Off
        #oldvaluetwo = sensorvaluetwo
        #newvaluetwo = ( (oldvaluetwo - WET) / (DRY - WET) ) * (newmax - newmin) + newmin
        #print("Sensor2 =", newvaluetwo,'%')


def read_airtemp():
    try:
        time.sleep(2)
        sensor.measure()
        time.sleep(2.5)  # for stability
        airtemp = sensor.temperature()
        temp_f = airtemp * (9/5) + 32.0
        print('temp {}°C'.format(airtemp))
        print('temp {}°F'.format(temp_f))

        if (isinstance(airtemp, float) or (isinstance(airtemp, int))):
            airtemp = (b'{0:3.1f},'.format(airtemp))
            return airtemp
        else:
            return('Invalid sensor readings.')
    except OSError as e:
        return('Failed to read DHT11 sensor.')


def read_hum():
    try:
        time.sleep(2)
        sensor.measure()
        time.sleep(2.5)  # for stability
        airtemp = sensor.temperature()
        perchum = sensor.humidity()
        hum = sensor.humidity()
        temp_f = airtemp * (9/5) + 32.0
        print('temp {}°C'.format(airtemp))
        print('temp {}°F'.format(temp_f))

        if (isinstance(perchum, float) or (isinstance(perchum, int))):
            perchum = (b'{0:3.1f},'.format(perchum))
            return perchum
        else:
            return('Invalid sensor readings.')
    except OSError as e:
        return('Failed to read DHT11 sensor.')


try:
    client = connect_mqtt()
except OSError as e:
    restart_and_reconnect()

while True:
    try:
        if (time.time() - last_message) > message_interval:
            temp = read_sensor()
            print(temp)
            hum = read_hum()
            print(hum)
            airtemp = read_airtemp()
            print(airtemp)
            waterlevel = read_waterlevel()
            print(waterlevel)
            client.publish(topic_pub_temp, temp)
            client.publish(topic_pub_hum, hum)
            client.publish(topic_pub_airtemp, airtemp)
            last_message = time.time()
            time.sleep(readings)
            # powersave()
    except OSError as e:
        restart_and_reconnect()
