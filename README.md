# :seedling: ESP32GreenhouseTowerDIY :seedling:

This repo is dedicated to the **DIY ESP32** based automated *Aeroponic* or *Hydroponic* Modular Tower Garden :cactus: . 

# WHAT IS THIS PROJECT

In the height of COVID my wife and i wanted to grow our own food indoors. This open source project is a culmination of my own work and many inspirations from the internet to develop an easy, approachable and beginner friendly project for hobbyists. 

This project is designed to be modular and affordable. Featuring a vertical tower system with interlocking parts and an adapter to a common 20L/5gallon bucket. 

The design also features an optional 3D printable Aeroponic Nozzle for converting the system from hydroponic to aeroponic.  

I decided to go with an ESP32 using Micropython firmware. While Arduino is perfectly fine, i wanted this project to be approachable to all skil levels, python seemed like the most comon first "language". 

# HOW TO ORDER PCBS

PCBS can be ordered from JLCPCB or PCBWay, or made yourself. The PCB files are still in prototype phase and i welcome any development ideas. 

The PCB is designed to be used with a GPIO extension board - however this was for my personal use-case. This can be changed, or i can make this adaptation upon request. 

# HOW TO SETUP

Setup is very straight forward, thankfully. You will need to purchase a few compents before you begin: 

__*Materials for purchase*__

1. Raspberrypi - any model
   1. Install Mosquitto broker to the pi and connect it to your network
2. 12V submersable water pump
3. 3m or more of Vinyl tubing with 10mm ID and 13mm OD


__*SETUP*__

1. Acquire PCB and ESP32 (WROVER and WROOM has both been tested). 
2. Acquire The remaining sensors and components - assemble 
3. Plug ESP32 into computer - open your IDE (i used Thonny for this project) Flash the micropython firmware to the esp32
   1. Upload ESP32MicropytyhonLibraries ---> rename to ESP32Micro
   2. Upload boot.py and greenhouse.py to root directory

![tower garden](/3D Printing Files/Modular Hydroponic Tower Garden/images/IMG_20190523_094749.jpg)
Format: ![Alt Text](url)
