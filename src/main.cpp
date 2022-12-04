#define BLYNK_PRINT Serial // Defines the object that is used for printing
//#define BLYNK_DEBUG        // Optional, this enables more detailed prints
#define BLYNK_TEMPLATE_ID "TMPLo61F7WPj"
#define BLYNK_DEVICE_NAME "smart garden"
#define BLYNK_AUTH_TOKEN "VjFGkzXrMZ1NyLNuP3e8g2I0JMeawqu9"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "Arduino.h"

// Comment this out to disable prints and save space
//#define BLYNK_PRINT Serial
// PIN for soil sensor 1
#define SENSOR_1 33
// PIN for pump 1
#define PUMP_1 32

// Your WiFi credentials. Set password to "" for open networks.
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "virus_exe";
char pass[] = "nonpasdewifipourtoi";
// constant value for handling sensor
const int calib_air_1   = 3440;
const int calib_water_1 = 1555;
int soil_value_1    = 0;
int soil_percent_1  = 0;
bool switch_pump    = 0;
float water_time    = 0;
float elapsed_time;
// time deep sleep
int time_sleep = 10 * 1e6;
// blynk timer
BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes
BLYNK_WRITE(V1)
{
  switch_pump = param.asInt();
  BLYNK_LOG("switch %d:", switch_pump);
  water_time = millis();
}


// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // read soil sensor
  soil_value_1 = analogRead(SENSOR_1);
  soil_percent_1 = map(soil_value_1, calib_air_1, calib_water_1, 0, 100);
  // bug at start that make soil moisture > 100, ignore those cases
  if (soil_percent_1 > 100)
    return;
  // send value to virtual pin 4
  Blynk.virtualWrite(V0, soil_percent_1);
  // activate pump when switch is on
  elapsed_time = (millis() - water_time)/1e3; // use now or millis ?
  if (elapsed_time < 2)
  {
    digitalWrite(PUMP_1, HIGH);
    BLYNK_LOG("SWITCH ON");
  } else {
    digitalWrite(PUMP_1, LOW);
    if (switch_pump) {
      switch_pump = 0;
      Blynk.virtualWrite(V1, 0);
      BLYNK_LOG("SWITCH OFF");
    }
  }
  BLYNK_LOG("soil moisture: %d", soil_percent_1);
  // put device in deep sleep
  ESP.deepSleep(time_sleep);

}

void setup()
{
  Serial.begin(9600);
  // Blynk. Setup a function to be called every second
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(10000L, myTimerEvent);
  // Pin assignment
  pinMode(SENSOR_1, OUTPUT);
  pinMode(PUMP_1, OUTPUT);
  digitalWrite(PUMP_1, LOW);
  delay(2000);
}

void loop()
{
  Blynk.run();
  timer.run();
  // You can inject your own code or combine it with other sketches.
  // Check other examples on how to communicate with Blynk. Remember
  // to avoid delay() function!
}
