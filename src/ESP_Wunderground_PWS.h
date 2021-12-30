/*
MIT License

Copyright (c) 2018 Mikkel Jeppesen

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
SOFTWARE.
*/

#pragma once

#include <Arduino.h>
#include <math.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

class Wunderground
{
	public:
		Wunderground(const char * ID, const char * pass);
		
		int16_t send_update(bool realtime = false);
		void new_query();

		void add_item(String item, String value);
		void add_temp_f(float tempF, bool indoor = false, uint8_t sensor = 1);
		void add_temp_c(float tempC, bool indoor = false, uint8_t sensor = 1);
		void add_relative_humidity(float RH, bool indoor = false);

		// This function is made available to get a dew point, but should not be
		// used to send to wg. This is automatically done if both temp and rh
		// is added to the query
		float _dew_point_c(float tempC, float rh);

	private:
		String _API_HOST            = "weatherstation.wunderground.com";
		String _API_HOST_REALTIME   = "rtupdate.wunderground.com";
		String _API_URI				= "/weatherstation/updateweatherstation.php";

		String _ID, _pass, _query;
		WiFiClient wifiClient;
		HTTPClient _http;

		bool _date_set  = false;

		int8_t _tempC	= -128;
		int8_t _rh  	= -1;
		
		String _urlencode(String str);
		float _celcius_to_fahrenheit(float tempC);
		float _fahrenheit_to_celcius(float tempF);
};