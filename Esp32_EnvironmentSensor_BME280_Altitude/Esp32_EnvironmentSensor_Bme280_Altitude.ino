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
// 192.168.178.40

/*
Wiring OLED SSD1306:
OLED - ESP32
VIN      3.3 V
GND      GND
SCL      P 22
SDA      P 21 

Wiring BME280:
Sensor - ESP32
VIN      3.3 V
GND      GND
SCL      P 22
SDA      P 21 

*/


//#include <Wire.h>
#include <Adafruit_Sensor.h>  // https://github.com/adafruit/Adafruit_Sensor
#include <Adafruit_BME280.h>  // https://github.com/adafruit/Adafruit_BME280_Library

/*#include <SPI.h>
#define BME_SCK 18
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 5*/

//const String SENSOR_NAME ="RANDOM Sens"; // don't extend the string with more than 14 characters !
const String SENSOR_NAME = "BME280 T/H/P";         // don't extend the string with more than 14 characters !
const String ALTITUDE_DISPLAY = "BME280 Altitde";  // don't extend the string with more than 14 characters !
const String ALTITUDE_WEB = "BME280 ALTITUDE";     // don't extend the string with more than 14 characters !
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;  // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

float temperature = -99;
float humidity = -99;
float pressure = -99;
float altitude = -99;

unsigned long delayTime;

// ----------------------------------------------------------------
// vars for "delay" without blocking the loop
const unsigned long period1s = 1000;    //the value is a number of milliseconds, ie 1 second
const unsigned long period3s = 3000;    //the value is a number of milliseconds, ie 3 seconds
const unsigned long period10s = 10000;  //the value is a number of milliseconds, ie 10 seconds
const unsigned long period30s = 30000;  //the value is a number of milliseconds, ie 30 seconds
const unsigned long period60s = 60000;  //the value is a number of milliseconds, ie 30 seconds
const unsigned long period30m = 30 * 60000;  //the value is a number of milliseconds, ie 30 minutes
unsigned long currentMillis;
unsigned long previousMillisDisplay = 0;
unsigned long previousMillisMeteomatics = 0;

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
// NTP time server
// uses the Built-in ESP32 NTP time client
#include <HTTPClient.h>
#include "sntp.h"

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

struct tm lastTimeinfo;
int day = 0;
int month = 0;
int year = 0;
int dayOfWeek = 0;
int dayOfYear = 0;
int hrs = 0;
int mins = 0;
int secs = 0;

//#define TIME_OFFSET 1UL * 3600UL  // UTC + 1 hour
#define TIME_OFFSET -2UL * 3600UL  // UTC + 2 hours // change for your region, ratingen/germany is - 2h
// Variable to save current epoch time
unsigned long epochTime;

// ----------------------------------------------------------------
// use Meteomatics.com weather api to retrieve the sea level pressure

const char *meteomaticsUserName = "USE YOUR OWN CREDENTIALS";
const char *meteomaticsUserPassword = "USE YOUR OWN CREDENTIALS";

// This are the coordinates of Airport Dusseldorf, Germany
String latitude = "51.28939920674677";
String longitude = "6.771708494352207";

String meteomaticsApiBaseUrl = "https://api.meteomatics.com/";

int seaLevelPressure = -99;
bool getSeaLevelPressureOneTime = true;

// ----------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  Serial.println(F("Environment Sensor BME280 Altitude"));

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
  bool status;
  // default settings
  //status = bme.begin(BME280_ADDRESS); // 0x77 If the SDO pin of the sensor is connected with + 3.3V this address is used
  status = bme.begin(BME280_ADDRESS_ALTERNATE);  // 0x76 // If the SDO pin of the sensor is not connected or with GND this address is used = DEFAULT
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    display.setCursor(0, 16);
    display.print("Sensor init");
    display.setCursor(0, 24);
    display.print(" FAILURE");
    display.display();
    while (1)
      ;
  }

  // ----------------------------------------------------------------
  // set notification call-back function
  sntp_set_time_sync_notification_cb(timeavailable);
  //sntp_servermode_dhcp(1);  // (optional)
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

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
  if ((WiFi.status() == WL_CONNECTED) && (getSeaLevelPressureOneTime == true)) {
    getMslPressureFromMeteomatics();
    if(seaLevelPressure > 0) {
      getSeaLevelPressureOneTime = false;
    }
  }

  // retrieve the Sea Level Pressure from Metematics every 30 minutes
  if (currentMillis - previousMillisMeteomatics > period30m) {
    if (WiFi.status() == WL_CONNECTED) {
      getMslPressureFromMeteomatics();
      previousMillisMeteomatics = currentMillis;
    }
  }

  if (currentMillis - previousMillisDisplay > period3s) {
    getBme280Values();
    printValues();
    updateWebserverData();
    if (displayBlockCounter == 0) {
      setupDisplayWiFi();
      displayValues();
      displayBlockCounter = 1;
    } else if (displayBlockCounter == 1) {
      setupDisplaySensorValues();
      displayValues();
      displayBlockCounter = 2;
    } else if (displayBlockCounter == 2) {
      setupDisplayAltitudeValues();
      displayValues();
      displayBlockCounter = 0;
    }

    previousMillisDisplay = currentMillis;
  }
}

void getBme280Values() {
  temperature = bme.readTemperature();
  pressure = bme.readPressure() / 100.0F;
  altitude = bme.readAltitude(seaLevelPressure);
  humidity = bme.readHumidity();
}

void printValues() {
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" *C");

  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/

  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(altitude);
  Serial.println(" m");

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
  display4 = "PRESS: " + String(pressure) + " hPa";
}

void setupDisplayAltitudeValues() {
  display1 = "Type:  " + ALTITUDE_DISPLAY;
  display2 = "Sea Level Pressure";
  display3 = "PRESS: " + String(seaLevelPressure) + " hPa";
  display4 = "ALTIT: " + String(altitude) + " Meters";
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
    return String(pressure);
  } else if (var == "SENSORNAME") {
    return SENSOR_NAME;
  } else if (var == "ALTITUDEMENU") {
    return ALTITUDE_WEB;
  } else if (var == "SEALEVELPRESSURE") {
    return String(seaLevelPressure);
  } else if (var == "ALTITUDE") {
    return String(altitude);
  }
}

void updateWebserverData() {
  // Send Events to the Web Server with the Sensor Readings
  events.send("ping", NULL, millis());
  events.send(String(temperature).c_str(), "temperature", millis());
  events.send(String(humidity).c_str(), "humidity", millis());
  events.send(String(pressure).c_str(), "pressure", millis());
  events.send(String(seaLevelPressure).c_str(), "sealevelpressure", millis());
  events.send(String(altitude).c_str(), "altitude", millis());
}

// ----------------------------------------------------------------
// NTP set time server
void setLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println(F("No time available (yet)"));
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  hrs = timeinfo.tm_hour;
  mins = timeinfo.tm_min;
  secs = timeinfo.tm_sec;
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval* t) {
  Serial.println(F("Got time adjustment from NTP!"));
  setLocalTime();
}

// Function that gets current epoch time
unsigned long getTime() {
  time_t now;
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    //Serial.println("Failed to obtain time");
    return (0);
  }
  time(&now);
  return now;
}

// we need a timestamp in this format:
// 2024-07-20T11:35:00.000+02:00
// https://randomnerdtutorials.com/esp32-ntp-timezones-daylight-saving/
String printTimestamp() {
  struct tm timeinfo;
  getLocalTime(&timeinfo);
  // Convert to HH:MM:SS
  char locTime[30];

  if (timeinfo.tm_isdst == 0) {
    sprintf(locTime, "%04d-%02d-%02dT%02d:%02d:00.000+01:00", 1900 + timeinfo.tm_year, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min);
  } else {
    // timeinfo.tm_isdst == 1
    sprintf(locTime, "%04d-%02d-%02dT%02d:%02d:00.000+02:00", 1900 + timeinfo.tm_year, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min);
  }

  // get epoch time
  time_t now;
  time(&now);
  epochTime = now;
  return locTime;
}

// ----------------------------------------------------------------
// use Meteomatics.com weather api to retrieve the sea level pressure

void getMslPressureFromMeteomatics() {
      //String serverPath = "https://api.meteomatics.com/2024-07-20T11:35:00.000+02:00/msl_pressure:hPa/51.28939920674677,6.771708494352207/csv"; // DUS Airport
      // buid serverPath
      String serverPath = meteomaticsApiBaseUrl + printTimestamp() + "/msl_pressure:hPa/" + latitude + "," + longitude + "/csv";
      String httpResponseString = httpGETRequest(serverPath.c_str());
      seaLevelPressure = getMslPressure(httpResponseString);
      Serial.print(F("Sea Level Pressure: "));
      Serial.println(seaLevelPressure);

}

String httpGETRequest(const char* serverName) {
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(serverName);
  // for reference: https://metar-taf.com/de/EDDL
  // 1010 hPa = identical to json data
  http.setAuthorization(meteomaticsUserName, meteomaticsUserPassword);

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0) {

    Serial.println(F("HTTPS REQUEST OK."));

    if (httpResponseCode == HTTP_CODE_OK) {
      Serial.println(F("Server responded with HTTP status 200."));
      payload = http.getString();
      Serial.print(F("Payload: "));
      Serial.print(payload);
      Serial.println(F("##"));
    } else {
      Serial.printf("Got HTTP status: %d", httpResponseCode);
      payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("Error occurred while sending HTTP Get: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  // Free resources
  http.end();

  return payload;
}

/*
Response of the CSV request:
Payload: validdate;msl_pressure:hPa
2024-07-20T09:35:00Z;1010
*/
int getMslPressure(String meteomaticsCsvResponse) {
  return getValueOfString(meteomaticsCsvResponse, ';', 2).toInt();
}

String getValueOfString(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
