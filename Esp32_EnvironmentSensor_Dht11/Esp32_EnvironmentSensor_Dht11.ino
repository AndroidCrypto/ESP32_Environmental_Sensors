/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-bme680-sensor-arduino/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  Modified by Michael Fehr / AndroidCrypto for an article series on medium.com
*********/

// Font Icons from https://fontawesome.com/

/*
Wiring OLED SSD1306:
OLED - ESP32
VIN      3.3 V
GND      GND
SCL      P 22
SDA      P 21 

Wiring DHT11:
DHT11  - ESP32
Left     Data Pin  
Middle   3.3 V
Right    GND
*/

// ----------------------------------------------------------------
// sensor data from DHT11 module
// for multiple sensor see https://robojax.com/learn/arduino/?vid=robojax-DHT22_2_or_more_DHT22
#include "DHT.h" // https://github.com/adafruit/DHT-sensor-library
// Adafruit Unified Sensor library
#include <Adafruit_Sensor.h>  // https://github.com/adafruit/Adafruit_Sensor
#define DHT11_PIN 18 // for DHT11 module
#define DHTTYPE11 DHT11   // DHT 11
DHT dht(DHT11_PIN, DHTTYPE11);//for first DHT module
const String SENSOR_NAME = "DHT11 T & H";  // don't extend the string with more than 14 characters !
float temperature = -99;
float humidity = -99;

// ----------------------------------------------------------------
// vars for "delay" without blocking the loop
const unsigned long period1s = 1000;    //the value is a number of milliseconds, ie 1 second
const unsigned long period3s = 3000;    //the value is a number of milliseconds, ie 3 seconds
const unsigned long period10s = 10000;  //the value is a number of milliseconds, ie 10 seconds
const unsigned long period30s = 30000;  //the value is a number of milliseconds, ie 30 seconds
unsigned long currentMillis;
unsigned long previousMillisDisplay = 0;

// ----------------------------------------------------------------
// OLED SSD1306
//#include <Wire.h>
#include <SPI.h>
#include <Adafruit_GFX.h>      // https://github.com/adafruit/Adafruit-GFX-Library
#include <Adafruit_SSD1306.h>  // https://github.com/adafruit/Adafruit_SSD1306
Adafruit_SSD1306 display = Adafruit_SSD1306();
#define SCREEN_ADDRESS 0x3C  ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// vars for displaying line 1 to 4 to display in a loop
String display1 = "";
String display2 = "";
String display3 = "";
String display4 = "";

// displaying data in blocks as the display is limited in size
// one block is the WiFi block with the IP address and the connection state
// the second block is the sensor data (temperature, humidity and air pressure)
unsigned int displayBlockCounter = 0;

// ----------------------------------------------------------------
// WiFi manager
#include <WiFiManager.h>  // https://github.com/tzapu/WiFiManager
// It creates a webpage at an address of 192.168.4.1 when no valid WiFi access data are stored in the EEPROM

// ----------------------------------------------------------------
// use a Webserver within your WiFi network to display the data as well

#include <WiFi.h>
#include <AsyncTCP.h>           // https://github.com/me-no-dev/AsyncTCP
#include "ESPAsyncWebServer.h"  // https://github.com/me-no-dev/ESPAsyncWebServer
AsyncWebServer server(80);
AsyncEventSource events("/events");

// this is our sample website to display the sensor data values with
// Server Side Evenets (SSE)
#include "website.h"

// ----------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  Serial.println(F("Environment Sensor DHT11"));

  // ----------------------------------------------------------------
  // init the OLED display
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed, programm stops"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  // clear the buffer
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Starting Sensor Init");
  display.display();

  // ----------------------------------------------------------------
  // setup the sensor
  // DHT11 & DHT22 initialization
  dht.begin();

  // ----------------------------------------------------------------
  // start the WiFi connection
  //WiFiManager, Local initialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  //wm.resetSettings();
  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result
  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  //res = wm.autoConnect("AutoConnectAP", "12345678");  // password protected ap
  res = wm.autoConnect("ESP32 AP Env Sensor");  // not password protected ap
  //wm.setConfigPortalTimeout(180);

  if (!res) {
    Serial.println(F("Failed to connect"));
    // ESP.restart();
    return;
  } else {
    //if you get here you have connected to the WiFi
    Serial.println(F("ESP32 is connected"));
  }
  // Print ESP Local IP Address
  Serial.println();
  Serial.print(F("Your IP-Address: "));
  Serial.println(WiFi.localIP());
  display.setCursor(0, 16);
  display.print("WiFi IP addr:");
  display.setCursor(0, 24);
  display.print(WiFi.localIP().toString());
  display.display();
  delay(3000);

  // ----------------------------------------------------------------
  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println(F("Connection to Webserver"));
    request->send_P(200, "text/html", index_html, processor);
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);

  server.begin();
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // ----------------------------------------------------------------
  // Webserver stuff
  // Handle Web Server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html, processor);
  });

  // Handle Web Server Events
  events.onConnect([](AsyncEventSourceClient *client) {
    if (client->lastId()) {
      Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
    }
    // send event with message "hello!", id current millis
    // and set reconnect delay to 1 second
    client->send("hello!", NULL, millis(), 10000);
  });
  server.addHandler(&events);
  server.begin();

  displayBlockCounter = 0;
}


void loop() {

  currentMillis = millis();
  if (currentMillis - previousMillisDisplay > period3s) {

    getDht11Values();
    printValues();
    updateWebserverData();
    if (displayBlockCounter == 0) {
      setupDisplayWiFi();
      displayValues();
      displayBlockCounter = 1;
    } else if (displayBlockCounter == 1) {
      setupDisplaySensorValues();
      displayValues();
      displayBlockCounter = 0;
    }

    previousMillisDisplay = currentMillis;
  }
}

void getDht11Values() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
}

void printValues() {
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");

  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/

  Serial.print("Humidity = ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.println();
}

void setupDisplayWiFi() {
  display1 = "Type:  " + SENSOR_NAME;
  display2 = "IP:  " + WiFi.localIP().toString();
  display3 = "Connection: " + String(WiFi.status());
  display4 = "Channel|RSSI: " + String(WiFi.channel()) + "|" + String(WiFi.RSSI());
}

void setupDisplaySensorValues() {
  display1 = "Type:  " + SENSOR_NAME;
  display2 = "TEMP:  " + String(temperature) + " *C";
  display3 = "HUMID: " + String(humidity) + " %";
  display4 = "PRESS:     n/a";
}

void displayValues() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print(display1);
  display.setCursor(0, 8);
  display.print(display2);
  display.setCursor(0, 16);
  display.print(display3);
  display.setCursor(0, 24);
  display.print(display4);
  display.display();
}

// Webserver processing

String processor(const String &var) {
  //Serial.println(var);
  if (var == "TEMPERATURE") {
    return String(temperature);
  } else if (var == "HUMIDITY") {
    return String(humidity);
  } else if (var == "PRESSURE") {
    return "n/a";
  } else if (var == "SENSORNAME") {
    return SENSOR_NAME;
  }
}

void updateWebserverData() {
  // Send Events to the Web Server with the Sensor Readings
  events.send("ping", NULL, millis());
  events.send(String(temperature).c_str(), "temperature", millis());
  events.send(String(humidity).c_str(), "humidity", millis());
  events.send("n/a", "pressure", millis());
}               