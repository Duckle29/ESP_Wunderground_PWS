#include <ESP_Wunderground_PWS.h>

Wunderground::Wunderground(const char * ID, const char * pass)
{
    _ID = String(ID);
    _pass = String(pass);
    new_query();
}

// Clears the query, and sets action, ID, and PASSWORD
void Wunderground::new_query()
{
    _date_set = false;
    _query = String("?action=updateraw");
    add_item(String("ID"), _ID);
    add_item(String("PASSWORD"), _pass);
}

// ------ Helper functions to easilly add common fields -----

// Add one multiple temperature readings to the query
void Wunderground::add_temp_c(float tempC, uint8_t sensor)
{
    add_temp_f(_celcius_to_fahrenheit(tempC), sensor);
}

void Wunderground::add_temp_f(float tempF, uint8_t sensor)
{
    String item = "";

    if(sensor > 1)
    {
        item = "temp";
        item += String(sensor);
        item += 'f';
    }
    else
    {
        item = String("tempf");
    }

    add_item(item, String(tempF));

    _tempC = _fahrenheit_to_celcius(tempF); // Used for dewpoint_calc
}

void Wunderground::add_relative_humidity(float RH)
{
    add_item("humidity", String(RH));
    _rh = RH;
}

// ------ END ------


// Add field that doesn't have a helper function
void Wunderground::add_item(String item, String value)
{
    if(item == "dateutc")
    {
        _date_set = true;
    }

    _query += '&';
    _query += item;
    _query += '=';
    _query += _urlencode(value);
}

int16_t Wunderground::send_update(bool realtime)
{
    if(!_date_set)
    {
        add_item("dateutc", "now");
    }

    if(_tempC != -128 && _rh != -1)
    {
        float dewptf = _celcius_to_fahrenheit(_dew_point_c(_tempC, _rh));
        add_item("dewptf", String(dewptf));
    }
    
    IPAddress HOST_IP;

    if(!WiFi.hostByName(realtime ? _API_HOST_REALTIME.c_str() : _API_HOST.c_str(), HOST_IP)) //
    {
        return -100;
    }

    String final_query = "http://";
    final_query += String(HOST_IP.toString());
    final_query += _API_URI;
    final_query += _query;
    
    // Send the query
    _http.begin(final_query);
    
    int16_t ret_code = _http.GET();

    _http.end();

    new_query();
    // Return return code    
    return ret_code;
    
}


//------ Private functions ------

float Wunderground::_celcius_to_fahrenheit(float tempC)
{
    return tempC * 1.8 + 32;
}

float Wunderground::_fahrenheit_to_celcius(float tempF)
{
    return (tempF - 32) / 1.8;
}

// Uses the Magnus formula
float Wunderground::_dew_point_c(float tempC, float rh)
{
    float b = 17.62, l = 243.12; // Magnus parameters

    float numer = l * ( (b*tempC)/(l+tempC) + log(rh/100) );
    float denom = b - ( (b*tempC)/(l+tempC) + log(rh/100) );
    
    return numer/denom;
}

// Function from zenmanenergys urlencode.
String Wunderground::_urlencode(String str)
{
    String encodedString="";
    char c, code0, code1;
    
    for (uint16_t i =0; i < str.length(); i++)
    {   
        c=str.charAt(i);
        if (c == ' ')
        {
            encodedString+= '+';
        }
        else if (isalnum(c))
        {
            encodedString+=c;
        }
        else
        {
            code1=(c & 0xf)+'0';
            if ((c & 0xf) >9)
            {
                code1=(c & 0xf) - 10 + 'A';
            }

            c=(c>>4)&0xf;
            code0=c+'0';

            if (c > 9)
            {
                code0=c - 10 + 'A';
            }

            encodedString+='%';
            encodedString+=code0;
            encodedString+=code1;
        }
        delay(0); // Yield doesn't work in libraries in platformIO
    }
    return encodedString;  
}
