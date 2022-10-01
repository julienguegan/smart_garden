#include "Arduino.h"

// PIN for soil sensor 1
#define SENSOR_1 4 

// calibration for sensor 1
const int calib_air_1 = 3440;
const int calib_water_1 = 1555;
int soil_value_1 = 0;
int soil_percent_1 = 100;
// PIN for pump 1
#define PUMP_1 0
// threshold for triggering pump
#define THRESHOLD 50

void setup()
{
  Serial.begin(9600);
  // pin assignment
  pinMode(SENSOR_1, OUTPUT);
  pinMode(PUMP_1, OUTPUT);
  // set pumps to OFF
  digitalWrite(PUMP_1, LOW);
  digitalWrite(alim, HIGH);
  delay(1000);
}

void loop()
{
  // read soil sensor
  soil_value_1 = analogRead(SENSOR_1);
  soil_percent_1 = map(soil_value_1, calib_air_1, calib_water_1, 0, 100);
  Serial.println((String) soil_value_1 + " ==> " + soil_percent_1 + " %");

  // trigger pump : for testing we trigger it when water
  if (soil_percent_1 > THRESHOLD)
  { 
    digitalWrite(PUMP_1, HIGH);
  } else {
    digitalWrite(PUMP_1, LOW);
  }

  // wait for a second
  delay(1000);
}