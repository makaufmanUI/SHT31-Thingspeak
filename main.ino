#include <Wire.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Arduino_JSON.h>
#include <Adafruit_SHT31.h>
#include <ESP8266HTTPClient.h>
Adafruit_SHT31 sht31 = Adafruit_SHT31();


String apiKey = "";                               // Thingspeak write API key
const char* server = "api.thingspeak.com";        // Thingspeak API server URL
const char *ssid = "Not Creepy Van Outside";      // WiFi name
const char *pass = "";                            // WiFi password
WiFiClient client;


void setup()
{
  Serial.begin(115200);
  delay(10);

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.println();

  if (!sht31.begin(0x44))
  {
    Serial.println("Couldn't connect to SHT31");
    while (1) delay(1);
  }
}



void loop()
{

  float t = sht31.readTemperature();
  float h = sht31.readHumidity();
  float tf = ( t * 1.8 ) + 32;

  if (client.connect(server, 80))                 // "184.106.153.149" or api.thingspeak.com
  {
    delay(100);
    String sendData = apiKey+"&field1="+String(tf)+"&field3="+String(h)+"\r\n\r\n";
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(sendData.length());
    client.print("\n\n");
    client.print(sendData);   
  }

  delay(300000);                                   // Wait 5 minutes
  
}
