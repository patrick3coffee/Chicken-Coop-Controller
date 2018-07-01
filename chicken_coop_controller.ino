#include "MotorWithStops.h"

#define DEBUG

/* 
 *  Pin Assignments 
*/
// Motor pins
#define DIR_DOOR_PIN 12 // Direction control for motor A
#define PWM_DOOR_PIN 3  // PWM control (speed) for motor A
#define DIR_WINDOW_PIN 13
#define PWM_WINDOW_PIN 11 

// Stop pins
#define DOOR_CLOSED_LIMIT 7
#define DOOR_OPEN_LIMIT 6
#define WINDOW_CLOSED_LIMIT 5
#define WINDOW_OPEN_LIMIT 4

// Analog pins to read potentiometers for settings
#define LIGHT_SET_PIN A0   
#define TEMP_SET_PIN A1

// Analog pins to read sensors
#define LIGHT_SENSE_PIN A2
#define TEMP_SENSE_PIN A3


// Enable coop actuators
#define DOOR     // enable coop door funtionality
#define WINDOW   // enable coop window funcitonality

// Setup motor control objects
#ifdef DOOR
MotorWithStops door(DIR_DOOR_PIN, PWM_DOOR_PIN, DOOR_CLOSED_LIMIT, DOOR_OPEN_LIMIT);
#endif

#ifdef WINDOW
MotorWithStops window(DIR_WINDOW_PIN, PWM_WINDOW_PIN, WINDOW_CLOSED_LIMIT, WINDOW_OPEN_LIMIT);
#endif

void setup()
{

#ifdef DEBUG
// setup serial
  Serial.begin(9600);
  Serial.println("Chicken Coop Controller");
#endif

// setup pins
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
  delay(500);    // Check twice a second for serial output sake.
#else
  delay(60000);  // Check once a minute because temperature and sunlight change slowly.
#endif
}


// Check sensors and actuate motors if needed.
void adjustCoop() {
#ifdef DOOR
// Door and light
  if (sensorAboveThreshold(LIGHT_SET_PIN, LIGHT_SENSE_PIN)) {
    door.open();
  }
  else {
    door.close();
  }
#endif

// Window and temperature
#ifdef WINDOW
  if (sensorAboveThreshold(TEMP_SET_PIN, TEMP_SENSE_PIN)) {
    window.open();
  }
  else {
    window.close();
  }
#endif
}


// Compare sensor to setting
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

