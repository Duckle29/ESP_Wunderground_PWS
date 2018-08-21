#include <ESP_Wunderground_PWS.h>
#include <esp8266wifi.h>

/*
if you want to keep your credentials in a seperate file,
then create a file called credentials.h in the same folder
as your main sketch, and move the credential variables in to it:
Then include this file
*/
// #include "credentials.h" 

const char* ssid = "YOUR WIFI NAME";
const char* password = "YOUR WIFI PASS";

const char* WG_ID = "YOUR WG ID";
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

    wg.add_temp_c(20);
    wg.add_relative_humidity(50);
    Serial.println(wg.send_update());
}

void loop()
{
	delay(0);
}