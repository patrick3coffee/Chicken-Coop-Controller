#include "MotorWithStops.h"

#define DEBUG

// Pin Assignments //
#define DIR_DOOR_PIN 12 // Direction control for motor A
#define PWM_DOOR_PIN 3  // PWM control (speed) for motor A
#define DIR_WINDOW_PIN 13 // Direction control for motor B
#define PWM_WINDOW_PIN 11 // PWM control (speed) for motor B

#define LIGHT_SET_PIN A0
#define LIGHT_SENSE_PIN A2

#define TEMP_SET_PIN A1
#define TEMP_SENSE_PIN A3

#define DOOR_CLOSED_LIMIT 7
#define DOOR_OPEN_LIMIT 6

#define WINDOW_CLOSED_LIMIT 5
#define WINDOW_OPEN_LIMIT 4

// Enable coop actuators
#define DOOR     // enable coop door funtionality
#define WINDOW   // enable coop window funcitonality


#ifdef DOOR
MotorWithStops door(DIR_DOOR_PIN, PWM_DOOR_PIN, DOOR_CLOSED_LIMIT, DOOR_OPEN_LIMIT);
#endif

#ifdef WINDOW
MotorWithStops window(DIR_WINDOW_PIN, PWM_WINDOW_PIN, WINDOW_CLOSED_LIMIT, WINDOW_OPEN_LIMIT);
#endif

void setup()
{

#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Chicken Coop Controller");
#endif
#ifdef DOOR
  pinMode(LIGHT_SET_PIN, INPUT);
  pinMode(LIGHT_SENSE_PIN, INPUT);
#endif
#ifdef WINDOW
  pinMode(TEMP_SET_PIN, INPUT);
  pinMode(TEMP_SENSE_PIN, INPUT);
#endif
}


void loop()
{
  adjustCoop();
  wasteSomeTime();
}


void wasteSomeTime() {
#ifdef DEBUG
  delay(500);
#else
  delay(60000);
#endif
}


void adjustCoop() {
#ifdef DOOR
  // door and light
  if (sensorAboveThreshold(LIGHT_SET_PIN, LIGHT_SENSE_PIN)) {
    door.open();
  }
  else {
    door.close();
  }
#endif

  // window and temperature
#ifdef WINDOW
  if (sensorAboveThreshold(TEMP_SET_PIN, TEMP_SENSE_PIN)) {
    window.open();
  }
  else {
    window.close();
  }
#endif
}


bool sensorAboveThreshold(int setting_pin, int sensor_pin) {
  int sensor_val = analogRead(sensor_pin);
  int setting_val = analogRead(setting_pin);
#ifdef DEBUG
  Serial.print("sensor reading: ");
  Serial.println(sensor_val);
  Serial.print("sensor setting: ");
  Serial.println(setting_val);
#endif

  if ( sensor_val > setting_val) {

#ifdef DEBUG
    Serial.println("sensor above threshold.");
#endif

    return true;
  }
  else {

#ifdef DEBUG
    Serial.println("sensor below threshold.");
#endif

    return false;
  }
}

