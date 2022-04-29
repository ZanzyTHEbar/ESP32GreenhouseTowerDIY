import esp32
from machine import Pin, deepsleep
from time import sleep

wake1 = Pin(34, Pin.IN)

#level parameter can be: esp32.WAKEUP_ANY_HIGH or esp32.WAKEUP_ALL_LOW
esp32.wake_on_ext0(pin = wake1, level = esp32.WAKEUP_ANY_HIGH)

#your main code goes here to perform a task
def powersave():
    print('Im awake. Going to sleep in 10 seconds')
    sleep(10)
    print('Going to sleep now')
    deepsleep()