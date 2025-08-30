#define BLYNK_PRINT Serial // Defines the object that is used for printing
#define BLYNK_TEMPLATE_ID "TMPLo61F7WPj"
#define BLYNK_DEVICE_NAME "smart garden"
#define BLYNK_AUTH_TOKEN "VjFGkzXrMZ1NyLNuP3e8g2I0JMeawqu9"
//#define BLYNK_DEBUG      // Optional, this enables more detailed prints
//#define BLYNK_PRINT Serial // Comment this out to disable prints and save space

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "Arduino.h"

// PIN for soil sensor 
#define SENSOR_1 33
#define SENSOR_2 35
// PIN for pump 
#define PUMP_1 32
#define PUMP_2 13

// WiFi credentials
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "virus_exe";
char pass[] = WIFI_PASSWORD;
// constant value for handling sensor
const int calib_air_1   = 3440;
const int calib_water_1 = 1555;
const int calib_air_2   = 3420;
const int calib_water_2 = 1420;
int soil_value_1      = 0;
int soil_value_2      = 0;
int soil_percent_1    = 0;
int soil_percent_2    = 0;
bool switch_pump_1    = 0;
bool switch_pump_2    = 0;
bool switch_auto_mode = 0;
int moisture_threshold = 50;
int time_pump_1 = 1;

// blynk timer
BlynkTimer timer;

// This functions are called every time the Virtual Pin state changes
// switch to activate pump 1
BLYNK_WRITE(V1)
{
  switch_pump_1 = param.asInt();
  BLYNK_LOG("switch pump 1: %d", switch_pump_1);
}
// switch to activate pump 2
BLYNK_WRITE(V3)
{
  switch_pump_2 = param.asInt();
  BLYNK_LOG("switch pump 2: %d", switch_pump_2);
}
// slider to change moisture threshold for auto mode
BLYNK_WRITE(V4)
{
  moisture_threshold = param.asInt();
  BLYNK_LOG("moisture threshold: %d", moisture_threshold);
}
// slider to change time for pump activity
BLYNK_WRITE(V5)
{
  time_pump_1 = param.asInt();
  BLYNK_LOG("time pump 1: %d", time_pump_1);
}
// switch to activate auto mode
BLYNK_WRITE(V6)
{
  switch_auto_mode = param.asInt();
  BLYNK_LOG("switch auto mode: %d", switch_auto_mode);
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // read soil sensor
  soil_value_1   = analogRead(SENSOR_1);
  soil_percent_1 = map(soil_value_1, calib_air_1, calib_water_1, 0, 100);
  BLYNK_LOG("soil moisture 1: %d", soil_percent_1);
  soil_value_2 = analogRead(SENSOR_2);
  soil_percent_2 = map(soil_value_2, calib_air_2, calib_water_2, 0, 100);
  BLYNK_LOG("soil moisture 2: %d", soil_percent_2);
  // bug at start that make soil moisture > 100, ignore those cases
  if (soil_percent_1 > 100 || soil_percent_2 > 100)
    return;
  // send value to virtual pin 
  Blynk.virtualWrite(V0, soil_percent_1);
  Blynk.virtualWrite(V2, soil_percent_2);
  // activate pump for 'time_pump_1' second when switch is on
  if (switch_pump_1) {
    digitalWrite(PUMP_1, HIGH);
    BLYNK_LOG("SWITCH 1 ON");
    delay(time_pump_1*1e3);
    digitalWrite(PUMP_1, LOW);
    switch_pump_1 = 0;
    Blynk.virtualWrite(V1, 0);
    BLYNK_LOG("SWITCH 1 OFF");
  }
  if (switch_pump_2) {
    digitalWrite(PUMP_2, HIGH);
    BLYNK_LOG("SWITCH 2 ON");
    delay(1000);
    digitalWrite(PUMP_2, LOW);
    switch_pump_2 = 0;
    Blynk.virtualWrite(V3, 0);
    BLYNK_LOG("SWITCH 2 OFF");
  }
  // auto mode
  if (switch_auto_mode) {
    if (soil_percent_1 < moisture_threshold) {
      digitalWrite(PUMP_1, HIGH);
      BLYNK_LOG("SWITCH 1 ON");
      delay(time_pump_1*1e3);
      digitalWrite(PUMP_1, LOW);
      switch_pump_1 = 0;
      Blynk.virtualWrite(V1, 0);
      BLYNK_LOG("SWITCH 1 OFF");
    }
  }
}

void setup()
{
  Serial.begin(9600);
  // Blynk. Setup a function to be called every second
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(10000L, myTimerEvent);
  // Pin assignment
  pinMode(SENSOR_1, OUTPUT);
  pinMode(SENSOR_2, OUTPUT);
  pinMode(PUMP_1, OUTPUT);
  pinMode(PUMP_2, OUTPUT);
  digitalWrite(PUMP_1, LOW);
  digitalWrite(PUMP_2, LOW);
}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
