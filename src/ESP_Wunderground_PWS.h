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
		void add_temp_f(float tempF, uint8_t sensor = 1);
		void add_temp_c(float tempC, uint8_t sensor = 1);
		void add_relative_humidity(float RH);

		


	private:
		String _API_HOST            = "weatherstation.wunderground.com";
		String _API_HOST_REALTIME   = "rtupdate.wunderground.com";
		String _API_URI				= "/weatherstation/updateweatherstation.php";

		String _ID, _pass, _query;
		HTTPClient _http;

		bool _date_set  = false;

		int8_t _tempC   = -128;
		int8_t _rh  = -1;
		
		String _urlencode(String str);
		float _celcius_to_fahrenheit(float tempC);
		float _fahrenheit_to_celcius(float tempF);
		float _dew_point_c(float tempF, float rh);
};