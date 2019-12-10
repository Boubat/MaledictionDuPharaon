/*
 WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32 
31.01.2017 by Jan Hendrik Berlin
 
 */

#include <ArduinoJson.h>
#include <WiFi.h>

char* ssid    = "FRITZ-NOVIRUS";
char* password = "iuio8789iuio8789";
//const char* ssid     = "4G-NOVIRUS";
//const char* password = "iuio8789iuio8789";
//const char* ssid     = "TREPOVIRUS";
//const char* password = "fnogktnx";
//const char* ssid     = "Sunrise_5GHz_C50130";
//const char* password = "5Xf9kNtY36lt";

#define TIMEOUT 15000
#define LED_IN 2

IPAddress local_IP(192, 168, 8, 101);
IPAddress gateway(192, 168, 8, 1);
IPAddress subnet(255, 255, 0, 0);

WiFiServer server(80);
DynamicJsonDocument bank(512*30);

void setup() {
    bank["size"] = 0;
    bank.createNestedArray("game");
  
    Serial.begin(115200);
    pinMode(LED_IN, OUTPUT);      // set the LED pin mode

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

/*    if (!WiFi.config(local_IP, gateway, subnet)) {
      Serial.println("STA Failed to configure");
    }*/
   // WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("ESP Mac Address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("DNS: ");
  Serial.println(WiFi.dnsIP());
    
  server.begin();

}

int value = 0;
int light = 0;
int i, j, k;

void loop(){
 WiFiClient client = server.available();   // listen for incoming clients
 int count = 0;
  if (client) {                             // if you get a client,
    Serial.println("######################");Serial.println("New Client.");          // print a message out the serial port
    digitalWrite(LED_IN,HIGH);
    String currentLine = "";                // make a String to hold incoming data from the client
    String answer = "END";
    while (client.connected() && !(count==TIMEOUT)) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        DynamicJsonDocument game(512);
        DeserializationError error = deserializeJson(game,client);
        if (error) {
          Serial.println("Deserializazion Error");
          serializeJsonPretty(game,Serial);Serial.println();
          client.println("NOK");
          break;
        } else {
          serializeJsonPretty(game,Serial);
          Serial.println();
        }
        int bankSize = bank["size"];
        if (!bankSize) {
          bank["game"].add(game);
          bank["size"] = bankSize+1;
        }
        for (i=0;i<bankSize;i++) {
          if (bank["game"][i]["name"] = game["name"]) {
            bank["game"][i] = game;
          }
          if (i == bankSize) {
            bank["game"].add(game);
            bank["size"] = bankSize+1;
          }
        }
        serializeJsonPretty(bank,Serial);Serial.println();
        client.print("OK");
      
        

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("/push:light1")) {
          light = 1;
        }
        if (currentLine.endsWith("/push:light0")) {
          light = 0;
        }
        if (currentLine.endsWith("/pull:light")) {
          answer = "light"; answer.concat(light);
          Serial.println("");
          Serial.println("Answer is :");
          Serial.println(answer);
        }
      }
      count ++;
      if(count==TIMEOUT) {
        Serial.println("TIMEOUT");
      }
    }
    // close the connection:
    delay(1);
    client.stop();
    Serial.println("Client Disconnected.");
    digitalWrite(LED_IN,LOW);
  }
}
