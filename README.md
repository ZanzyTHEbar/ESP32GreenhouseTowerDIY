[![GitHub issues](https://img.shields.io/github/issues/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/issues) [![GitHub forks](https://img.shields.io/github/forks/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/network) [![GitHub stars](https://img.shields.io/github/stars/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/stargazers) [![GitHub license](https://img.shields.io/github/license/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/blob/main/LICENSE)

# :seedling: ESP32GreenhouseTowerDIY :seedling:

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

# Introduction

**Purpose:** These are instructions to build an open hardware, DIY Vertical Greenhouse Garden.

**Expected results:** A fully functional and (reasonably) automated vertical tower system.

**Minimal parts cost:** `<$20` for the basic system which consists of a `20L/5gallon` (can be smaller - minimum size is `4L/1gallon`) humidifier box/water reservoir and submersible pump - about `$100`. You will also need to purchase the PCB's `~$40`, power supply and sensors `~$30`. The cost of the parts will vary depending on the size of the tower, and the number of sensors. The cost of 3D printing the tower will be `~$25` or less. The full `1m` tower consumes `~2kg` of `PETG` - which is the recommended printing material for outdoor use. The tower can be printed in `ABS` and/or `PETG` materials.

> **Note:** The cost of plant nutrients is not taken into account, and is up to you to control.

**Goal:** The goal of this project was to build a high-efficiency, very low cost direct DIY vertical tower garden for growing small fruits and vegetables.

{% for tags in page.image_tags %}
    {% assign alt_tags = tags | split: "  " %}
{% endfor %}

{% include modal/image.html height=page.image_height width=page.image_width title=page.image_title image=page.image_category description=page.image_description src=page.image_src tags=alt_tags %}

____

## How to Setup  - Hardware

Setup is very straight forward, thankfully. You will need to purchase a few components before you begin:

### ***Materials for purchase***


1. Raspberrypi - *any model*
   1. Install the MQTT client included with Home Assistant (install Home Assistant), as this project has full support for Home Assistant (Arduino Core version only currently).
      1. Alternatively, you can install Mosquitto broker to the pi and connect it to your network
2. 12V submersible water pump
3. 3m or more of Vinyl tubing with 10mm ID and 13mm OD

____

## How to Setup - Software

1. Acquire all parts and ESP32 (WROVER and WROOM has both been tested).
2. Acquire The remaining sensors and components - assemble

### ***SETUP***


____

#### Micrpython


1. Plug ESP32 into computer - open your IDE (i used Thonny for this project) Flash the micropython firmware to the esp32
   1. Upload ESP32MicropytyhonLibraries ---> rename to ESP32Micro
   2. Upload boot.py and greenhouse.py to root directory

____

#### Arduino Core


1. Plug ESP32 into computer - navigate to [WebSerial](/ESP32GreenhouseTowerDIY/webserial/)
   1. Configure your chosen sensors and features
   2. Press build and upload, and wait for the green light to turn on - it is safe to unplug the ESP32 at that point.

____

#### MQTT Broker Configuration and Connection


If you have chosen the mDNS option, you will need to setup mDNS on your network for MQTT Discovery. The mDNS service on the ESP is already configure, if you chose that feature, and should appear on your network as at the `<name>.tower.local` address.

The MQTT Broker is a free service that allows you to publish and subscribe to MQTT messages. To automate the connection process, this project offers Multi-Cast DNS (mDNS/Zeroconf) to find the broker. For this to work, you must have a broker on your local network, and you must have only 1 MQTT broker within the range of the client, otherwise the client will simply connect to the images broker it finds.

Using a Zeroconf approach we can avoid having to hard code the broker's IP address or hostname into the client device's firmware. Instead we can use DNS-SD and Avahi/Bonjour to discover the server hosting the MQTT broker.

To enable MQTT discovery on the broker, simply install avahi-daemon.

##### Option One:


If you are using Home Assistant, you can use the MQTT Discovery feature within the firmware, and the MQTT broker will be discovered automatically. Nothing is needed from you for this to work. Simply turn on the ESP32 and have the `mDNS` feature enabled in the firmware.

You can refer to the [ZeroConf Documentation here](https://www.home-assistant.io/integrations/zeroconf/) and [here](https://developers.home-assistant.io/docs/creating_integration_manifest/#zeroconf) for home assistance for more information and troubleshooting.

If for some reason you are unable to use the MQTT Discovery feature, you can still use the mDNS feature to discover the broker, see Option Two below.

##### Option Two:


You will need to install the [avahi](https://avahi.org/) package. For a Raspberry Pi, use the following command:

    sudo apt-get install avahi-daemon

For this to work, the MQTT service needs to be advertised. On a Linux host system, Avahi can be configured to do this by including the following in `/etc/avahi/services/mqtt.service`:

```xml
<!DOCTYPE service-group SYSTEM "avahi-service.dtd">
<service-group>
 <name replace-wildcards="yes">MQTT on %h</name>
  <service>
   <type>_mqtt._tcp</type>
   <port>1883</port>
  </service>
</service-group>
```

____

## Benefits

____

## Challenges and Limitations

____

## Safety Considerations

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
