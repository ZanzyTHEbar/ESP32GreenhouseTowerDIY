# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
#import webrepl
#webrepl.start()

try:
    import usocket as socket
except:
    import socket

from machine import Pin

import network

import esp

import gc


esp.osdebug(None)

gc.collect()

ssid = 'LoveHouse2G'
password = 'vxwbywtswp'

station = network.WLAN(network.STA_IF)

station.active(True)
station.connect(ssid, password)

while not station.isconnected():
    pass

print('Connection successful')
print(station.ifconfig())

led = Pin(2, Pin.OUT)