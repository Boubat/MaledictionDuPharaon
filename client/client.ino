/*
  Web client

 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe, based on work by Adrian McEwen

 */



// included libraries
//
  #include <SPI.h>
  #include <Ethernet.h>
  #include<ArduinoJson.h>
//
/////////////////////



// definitions of constants
//
//  #define RELAY         11
//  #define LIGHT         12
  #define BUILTIN_LED   13
//
///////////////////////////



// definitions of global variables
//  
  byte macEthernet [] = { 0x98, 0x76, 0xB6, 0x11, 0x56, 0x42 };
  char server[] = "192.168.178.22";                                           // ip adress of the HT bank server
  
  IPAddress ip(192, 168, 178, 23);                                            // ip adress wanted for the current board
  IPAddress dns(192, 168, 178, 1);
  EthernetClient client;                                                      // Initialize the Ethernet client library
  
  StaticJsonDocument<512> game;
  int won = false;
  int i, j, k;                                                                // Some counters
//
//////////////////////////////////





void setup() {
  
  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // pin 10 is the chip select (CS) pin for feather M0


  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while(!Serial) continue;


  // pins' mode
  // 
    pinMode(BUILTIN_LED,OUTPUT);
    digitalWrite(BUILTIN_LED,LOW);
  //
  /////////////


  // Json state object
  //
    game["name"] = "simon";
    game["has changed"] = false;
    game["request"] = "";
    won = false;
    game["won"] = won;  
  //
  ////////////////////

  
  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(macEthernet) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(macEthernet, ip, dns);
  } else {
    Serial.print("  DHCP assigned IP ");
    ip = Ethernet.localIP();
    String ipStr = "";
    for (i=0;i<4;i++) {
      if (i) {
        ipStr.concat(".");
      }
      ipStr.concat(ip[i]);
    }
    game["ip"] = ipStr;
  }

  
  // give the Ethernet shield a second to initialize:
  delay(2000);
}






void loop() {
  delay(1500);
  String pushRequest, pullRequest = String("pull:light"), answer = "";
  bool push = false, pull = false;
  switch (won) {
    case true   :
        push = false;
        pull = true;
        break;
    case false    :
        //pushRequest = String("push:light");pushRequest.concat(light);
        push = true;
        pull = false;
        break;    
  }


  
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

// - PUSH TO SERVER

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

  while (push) {
    while(!client.connect(server,80)) continue;
    
    // if you get a connection, report back via serial:
    if (client.connect(server, 80)) {
      Serial.println("Push request started.");
      //client.println(pushRequest);
      serializeJson(game,client);
    } else {
      // if you didn't get a connection to the server:
      Serial.println("connection failed");
    }

    while(!client.available() && client.connected()) continue;
    
    while(client.available() && client.connected()) {
      char c = client.read();
      answer.concat(c);
      if(answer.endsWith("END")) {
        push = false;
      }
      if(answer.endsWith("NOK")) {
        break;
      }
    }
    client.stop();
    delay(2000);
  }


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

// - PULL FROM SERVER

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

/*answer = "";

  while (pull) {
    while(!client.connect(server,80)){
      Serial.println(client.connect(server,80));
      delay(50);
    }
    // if you get a connection, report back via serial:
    if (client.connect(server, 80)) {
      Serial.println("Pull request started.");
      client.println(pullRequest);
      client.println();
    } else {
      // if you didn't get a connection to the server:
      Serial.println("connection failed");
    }

    while(!client.available() && client.connected()) {
      delay(1);
    }
    while(client.available() && client.connected()) {
      char c = client.read();
      answer.concat(c);
      if(answer.endsWith("light0")) {
        digitalWrite(RELAY,LOW);
      }
      if(answer.endsWith("light1")) {
        digitalWrite(RELAY,HIGH);
        delay(2000);
      }
    }
    Serial.println(answer);
    if(answer.indexOf("END")!=-1) {
      pull = false;
    }
    client.stop();
  }*/
}
