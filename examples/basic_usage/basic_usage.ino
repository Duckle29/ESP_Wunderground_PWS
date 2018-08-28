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

#include <ESP8266WiFi.h>
#include <ESP_Wunderground_PWS.h>

const char* ssid = "YOUR WIFI NAME";
const char* password = "YOUR WIFI PASS";

const char* WG_ID = "YOUR WG PWS ID";
const char* WG_PASS = "YOUR WG PWS PASS";


// Initialize the library with your credentials, and start a new query
Wunderground wg(WG_ID, WG_PASS);

void setup()
{
    Serial.begin(115200);
    WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
    WiFi.begin(ssid, password);
    
    Serial.print("\nConnecting");
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }

    //If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());  //IP address assigned to your ESP
    delay(1000);

    wg_example();
}

// This function contains the library specific example code
void wg_example()
{
    
    wg.add_temp_c(20);              // Add a temperature reading
    wg.add_temp_c(22, false, 2);    // Add a temperature reading from a second sensor
    wg.add_temp_c(25, true);        // Add a temperature from an indoor sensor

    wg.add_relative_humidity(50);       // Add a humidity reading
    wg.add_relative_humidity(30, true); // Add an indoor humidity reading

    // Add a custom item.
    // List of all items: 
    // http://wiki.wunderground.com/index.php/PWS_-_Upload_Protocol
    wg.add_item("baromin", String(29.9)); 

    // Print return-code of send_updates
    // -100 = dns lookup failed
    // else, refer to HTTPClient library for meaning of other return codes
    // In short, any return-code under 0 is client error, any return code above
    // 0 is an HTTP code (200 ok, 404 not found. etc)
    Serial.println(wg.send_update());
}

void loop()
{
    delay(0);
}