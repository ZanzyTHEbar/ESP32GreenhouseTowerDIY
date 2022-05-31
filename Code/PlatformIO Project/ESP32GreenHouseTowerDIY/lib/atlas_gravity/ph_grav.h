/*
MIT License

Copyright (c) 2020 Atlas Scientific

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE
*/

#ifndef PH_GRAV_H
#define PH_GRAV_H

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <base_grav.h>

class Gravity_pH : public Gravity_Base{
	public:
	
		Gravity_pH(uint8_t pin);
		
		bool begin();
	
		virtual float read_voltage();
		float read_ph(float voltage_mV);
		float read_ph();
		
		void cal_mid(float voltage_mV);
		void cal_mid();
		
		void cal_low(float voltage_mV);
		void cal_low();
		
		void cal_high(float voltage_mV);
		void cal_high();
	
		void cal_clear();
		
	private:
		
		struct PH {
		  const uint8_t magic = magic_char;
          const enum grav_type type = GRAV_PH;
		  float mid_cal = 1500;
		  float low_cal = 2030;
		  float high_cal = 975;
		};
		struct PH pH;
};

#endif