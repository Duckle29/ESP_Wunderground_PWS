#include <wunderground.h>
#include <esp8266wifi.h>

const char* ssid = "yourNetworkName";
const char* password = "yourNetworkPassword";

// Initialize the library with your credentials, and start a new query
Wunderground wg("YOUR ID", "YOUR PASS");

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