#include <Arduino.h>
#include <WiFi.h>
#include <ThingSpeak.h>

// WiFi Credentials
const char* WIFI_SSID = "Nguyen Huu Loi";
const char* WIFI_PASSWORD = "12346789";

// ThingSpeak Configuration
unsigned long myChannelNumber = 3475843;      // Replace with your Channel ID
const char * myWriteAPIKey = "TAXGZP60322S2NW9"; // Replace with your Write API Key
const char * myReadAPIKey = "AX1XHZR8ILSRE8MA"; // Replace with your Write API Key

WiFiClient client;

void setup() {
    Serial.begin(115200);
    
    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected to WiFi!");

    // Initialize ThingSpeak
    ThingSpeak.begin(client); 
}

void loop() {
    // Generate mock sensor data
    float fire = 15; 
    float door = 15;

    // Set ThingSpeak fields (Field 1 and Field 2)
    ThingSpeak.setField(1,fire);
    ThingSpeak.setField(2, door);

    // Write data to the cloud
    int httpResponseCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if (httpResponseCode == 200) {
        Serial.println("Channel update successful.");
    } else {
        Serial.println("Problem updating channel. HTTP error code " + String(httpResponseCode));
    }
    delay(20000); 
   int ret_value = ThingSpeak.readIntField(myChannelNumber, 1, myReadAPIKey);
    Serial.println("Fire status " + String(ret_value));
    ret_value = ThingSpeak.readIntField(myChannelNumber, 2, myReadAPIKey);
    Serial.println("Door status " + String(ret_value));
}
