[![GitHub issues](https://img.shields.io/github/issues/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/issues) [![GitHub forks](https://img.shields.io/github/forks/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/network) [![GitHub stars](https://img.shields.io/github/stars/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/stargazers) [![GitHub license](https://img.shields.io/github/license/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/blob/main/LICENSE)

# :seedling: ESP32GreenhouseTowerDIY :seedling

This repo is dedicated to the **DIY ESP32** based automated *Aeroponic* or *Hydroponic* Modular Tower Garden :cactus: .

# WHAT IS THIS PROJECT

In the height of COVID my wife and i wanted to grow our own food indoors. This open source project is a culmination of my own work and many inspirations from the internet to develop an easy, approachable and beginner friendly project for hobbyists.

This project is designed to be modular and affordable. Featuring a vertical tower system with interlocking parts and an adapter to a common `20L/5gallon` bucket.

The design also features an optional 3D printable Aeroponics Nozzle for converting the system from hydroponic to aeroponics.  

> I decided to go with an ESP32 using Micropython firmware. While Arduino is perfectly fine, i wanted this project to be approachable to all skill levels, python seemed like the most common first "language".
>
> **Note**: I have added Arduino Core support for the ESP32. This is a work in progress and will be the main code-base in the future going forward.It simply has more sensor support and a few more features, plus it runs faster and consumes less power.

![tower garden](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/blob/main/3D%20Printing%20Files/Modular%20Hydroponic%20Tower%20Garden/images/IMG_20190523_094749.jpg)

![tower garden](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/blob/main/3D%20Printing%20Files/Modular%20Hydroponic%20Tower%20Garden/images/IMG_20200518_074307.jpg)

![tower garden](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/blob/main/3D%20Printing%20Files/5%20Gallon%20(20L)%20Bucket%20Lid%20Adapter/images/IMG_5375.JPG)

# HOW TO ORDER PCBS

PCBS can be ordered from [JLCPCB](https://jlcpcb.com/) or [PCBWay](https://www.pcbway.com/), or made yourself. The PCB files are still in prototype phase and I welcome any development ideas.

# HOW TO SETUP

Setup is very straight forward, thankfully. You will need to purchase a few components before you begin:

***Materials for purchase***

1. Raspberrypi - any model
   1. Install Mosquitto broker to the pi and connect it to your network
      1. Alternatively, you can use the MQTT client included with Home Assistant, as this project has full support for Home Assistant (Arduino Core version only currently).
2. 12V submersible water pump
3. 3m or more of Vinyl tubing with 10mm ID and 13mm OD

***SETUP***

1. Acquire PCB and ESP32 (WROVER and WROOM have both been tested).
2. Acquire The remaining sensors and components - assemble
3. Plug ESP32 into computer - open your IDE (i used Thonny for this project) Flash the micropython firmware to the esp32
   1. Upload ESP32MicropytyhonLibraries ---> rename to ESP32Micro
   2. Upload boot.py and greenhouse.py to root directory

## Important Notes

> **Note**: I have not tested this on a raspberry pi, but i have tested it on a WROOM and WROVER.
>
> **Warning**: If you receive the error:
>
> WebAuthentication.cpp:73: undefined reference to mbedtls_md5_starts
> Please remove the code *within* the `ifdef ESP32` block on line `72`. and paste the following:
>
>```ino
>   mbedtls_md5_init(&_ctx); mbedtls_md5_update_ret (&_ctx,data,len);
>   mbedtls_md5_finish_ret(&_ctx,data);
>   mbedtls_internal_md5_process( &_ctx ,data); // mbedtls_md5_starts(&_ctx); // mbedtls_md5_update(&_ctx, data, len); // mbedtls_md5_finish(&_ctx, _buf);
>```
>
> the comments are the old-lines.
