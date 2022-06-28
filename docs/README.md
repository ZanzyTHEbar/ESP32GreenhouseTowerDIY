---
layout: default
title: Welcome to Cyan
nav_exclude: true
has_children: false
permalink: index.html
image_title: "Vertical Tower Logo"
image_description: ""
image_category: "logo"
image_tags: "logo blue animated"
image_src: "/ESP32GreenhouseTowerDIY/assets/images/logo.gif"
image_height: "500"
image_width: "600"
image_title_2: "Build Status"
image_description_2: "Full Tower Build"
image_category_2: "build-status"
image_tags_2: "build status"
image_src_2: "./assets/images/tower_garden_imgs/outdoors/IMG_20190417_153821_02.jpg"
image_height_2: "600"
image_width_2: "400"
image_title_3: "Tower Garden"
image_description_3: "picture of roots growing"
image_category_3: "build-status"
image_tags_3: "roots growing"
image_src_3: "./assets/images/tower_garden_imgs/outdoors/IMG_20200518_074307.jpg"
image_height_3: "600"
image_width_3: "400"
image_title_4: "image of lid mount"
image_description_4: "this is the 5 gallon lid mount"
image_category_4: "build-status"
image_tags_4: "lid mount"
image_src_4: "./assets/images/tower_garden_imgs/bucket/IMG_5375.jpg"
image_height_4: "600"
image_width_4: "400"
---

[![GitHub issues](https://img.shields.io/github/issues/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/issues) [![GitHub forks](https://img.shields.io/github/forks/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/network)

-----

{% for tags in page.image_tags %}
    {% assign alt_tags = tags | split: "  " %}
{% endfor %}

{% include modal/image.html height=page.image_height width=page.image_width title=page.image_title image=page.image_category description=page.image_description src=page.image_src tags=alt_tags %}

# Welcome to ESP32 Greenhouse Tower DIY
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

* TOC
{:toc}

Welcome to the documentation for the *ESP32* based Greenhouse Tower DIY :seedling:, version 1.
{: .fs-6 .fw-300 }

A DIY vertical tower garden for indoor or outdoor use :cactus:.
{: .fs-6 .fw-300 }

{% for tags_2 in page.image_tags_2 %}
    {% assign alt_tags_2 = tags_2 | split: "  " %}
{% endfor %}

{% include modal/image.html height=page.image_height_2 width=page.image_width_2 title=page.image_title_2 image=page.image_category_2 description=page.image_description_2 src=page.image_src_2 tags=alt_tags_2 %}

[![GitHub issues](https://img.shields.io/github/issues/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/issues) [![GitHub forks](https://img.shields.io/github/forks/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/network) [![GitHub stars](https://img.shields.io/github/stars/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/stargazers) [![GitHub license](https://img.shields.io/github/license/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/blob/main/LICENSE)

# :seedling: ESP32GreenhouseTowerDIY :seedling:

This repo is dedicated to the **DIY ESP32** based automated *Aeroponic* or *Hydroponic* Modular Tower Garden :cactus: .

# What is this project?

In the height of COVID my wife and i wanted to grow our own food indoors. This open source project is a culmination of my own work and many inspirations from the internet to develop an easy, approachable and beginner friendly project for hobbyists.

This project is designed to be modular and affordable. Featuring a vertical tower system with interlocking parts and an adapter to a common `20L/5gallon` bucket.

The design also features an optional 3D printable Aeroponics Nozzle for converting the system from hydroponic to aeroponics. This is a great way to grow food indoors without having to worry about the water.

><p>
>   <span class="icon-span-info"><svg class="octicon octicon-info mr-2" viewBox="0 0 16 16" version="1.1" width="16" height="16" aria-hidden="true"><path fill-rule="evenodd" d="M8 1.5a6.5 6.5 0 100 13 6.5 6.5 0 000-13zM0 8a8 8 0 1116 0A8 8 0 010 8zm6.5-.25A.75.75 0 017.25 7h1a.75.75 0 01.75.75v2.75h.25a.75.75 0 010 1.5h-2a.75.75 0 010-1.5h.25v-2h-.25a.75.75 0 01-.75-.75zM8 6a1 1 0 100-2 1 1 0 000 2z"></path></svg><b>Note:</b></span> I have added Arduino Core support for the ESP32. This is still a work in progress and will be the main code-base in the future going forward. It simply has more sensor support and a few more features, plus it runs faster and consumes less energy.
> </p>

{% for tags_3 in page.image_tags_3 %}
    {% assign alt_tags_3 = tags_3 | split: "  " %}
{% endfor %}

{% include modal/image.html height=page.image_height_3 width=page.image_width_3 title=page.image_title_3 image=page.image_category_3 description=page.image_description_3 src=page.image_src_3 tags=alt_tags_3 %}

{% for tags_4 in page.image_tags_4 %}
    {% assign alt_tags_4 = tags_4 | split: "  " %}
{% endfor %}

{% include modal/image.html height=page.image_height_4 width=page.image_width_4 title=page.image_title_4 image=page.image_category_4 description=page.image_description_4 src=page.image_src_4 tags=alt_tags_4 %}

# How To Order PCBs

PCBS can be ordered from  [JLCPCB](https://jlcpcb.com/) or [PCBWay](https://www.pcbway.com/), or made yourself. The PCB files are still in prototype phase and I welcome any development ideas.

The old PCB is designed to be used with a GPIO extension board - however this was for my personal use-case. This can be changed, or i can make this adaptation upon request.

## Important Notes

> <p>
>   <span class="icon-span-info"><svg class="octicon octicon-info mr-2" viewBox="0 0 16 16" version="1.1" width="16" height="16" aria-hidden="true"><path fill-rule="evenodd" d="M8 1.5a6.5 6.5 0 100 13 6.5 6.5 0 000-13zM0 8a8 8 0 1116 0A8 8 0 010 8zm6.5-.25A.75.75 0 017.25 7h1a.75.75 0 01.75.75v2.75h.25a.75.75 0 010 1.5h-2a.75.75 0 010-1.5h.25v-2h-.25a.75.75 0 01-.75-.75zM8 6a1 1 0 100-2 1 1 0 000 2z"></path></svg><b>Note:</b></span>
> I have not tested this on a raspberry pi, but i have tested it on a WROOM and WROVER.
> </p>
>
> <p>
>   <span class="icon-span-warning"><svg class="octicon octicon-alert mr-2" viewBox="0 0 16 16" version="1.1" width="16" height="16" aria-hidden="true"><path fill-rule="evenodd" d="M8.22 1.754a.25.25 0 00-.44 0L1.698 13.132a.25.25 0 00.22.368h12.164a.25.25 0 00.22-.368L8.22 1.754zm-1.763-.707c.659-1.234 2.427-1.234 3.086 0l6.082 11.378A1.75 1.75 0 0114.082 15H1.918a1.75 1.75 0 01-1.543-2.575L6.457 1.047zM9 11a1 1 0 11-2 0 1 1 0 012 0zm-.25-5.25a.75.75 0 00-1.5 0v2.5a.75.75 0 001.5 0v-2.5z"></path></svg><b>Warning:</b></span>
> If you receive the error:
> </p>
>
> WebAuthentication.cpp:73: undefined reference to mbedtls_md5_starts
> Please remove the code *within* the `ifdef ESP32` block on line `72`. and paste the following:
>
>```ino
>   mbedtls_md5_init(&_ctx); mbedtls_md5_update_ret (&_ctx,data,len);
>   mbedtls_md5_finish_ret(&_ctx,data);
>   mbedtls_internal_md5_process( &_ctx ,data); 
>   // mbedtls_md5_starts(&_ctx); 
>   // mbedtls_md5_update(&_ctx, data, len); 
>   // mbedtls_md5_finish(&_ctx, _buf);
>```
>
> the comments are the old-lines.

## Licenses

[![GitHub license](https://img.shields.io/github/license/ZanzyTHEbar/ESP32GreenhouseTowerDIY?style=plastic)](https://github.com/ZanzyTHEbar/ESP32GreenhouseTowerDIY/blob/master/LICENSE){:target="_blank"}

***All hardware materials and designs provided here are licensed under the [CERN-OHL-P](https://opensource.org/CERN-OHL-P){:target="_blank"} hardware license.
All software is under the [MIT License](https://opensource.org/licenses/MIT).
All documentation, including the Wiki, is under the Creative Commons [CC-BY-SA-4.0 license](https://creativecommons.org/licenses/by-sa/4.0/)***.

<div align="center">
    <img src="./assets/images/licenses/licenses.svg" width="300" alt="Open Licenses" />
</div>
