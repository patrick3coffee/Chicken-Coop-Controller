#include "MotorWithStops.h"
/*
    MotorWithStops library located here:
    https://github.com/patrick3making/Motor-with-Stops
*/

#define DEBUG

/*
    Pin Assignments
*/
// Motor pins
#define DIR_CHICKEN_DOOR_PIN 12 // Direction control for motor A
#define PWM_CHICKEN_DOOR_PIN 3  // PWM control (speed) for motor A
#define DIR_WINDOW_PIN 13
#define PWM_WINDOW_PIN 11

// Stop pins
#define CHICKEN_DOOR_CLOSED_LIMIT 7
#define CHICKEN_DOOR_OPEN_LIMIT 6
#define WINDOW_CLOSED_LIMIT 5
#define WINDOW_OPEN_LIMIT 4

// Analog pins to read potentiometers for settings
#define LIGHT_SET_PIN A0
#define TEMP_SET_PIN A1

// Analog pins to read sensors
#define LIGHT_SENSE_PIN A3
#define TEMP_SENSE_PIN A2

// Mode selector switch
#define MANUAL_CLOSE_PIN 8
#define MANUAL_OPEN_PIN 2

// Other pin assignments
#define HUMAN_DOOR_SWITCH 9
#define LIGHT_CONTROL_PIN 10    // PWM would be nice for lights, but not necessary

// Enable coop actuators
#define HUMAN_DOOR    // enable human door switch to enable coop lights
#define CHICKEN_DOOR  // enable coop door funtionality
//#define WINDOW      // enable coop window funcitonality

// Setup motor control objects

#ifdef CHICKEN_DOOR
MotorWithStops chickenDoor(DIR_CHICKEN_DOOR_PIN, PWM_CHICKEN_DOOR_PIN, true, CHICKEN_DOOR_CLOSED_LIMIT, CHICKEN_DOOR_OPEN_LIMIT, false, true);
#endif

#ifdef WINDOW
MotorWithStops window(DIR_WINDOW_PIN, PWM_WINDOW_PIN, false, WINDOW_CLOSED_LIMIT, WINDOW_OPEN_LIMIT);
#endif


void setup()
{

  // setup serial
  Serial.begin(9600);
  Serial.println("Chicken Coop Controller");

  // setup pins
  pinMode(LIGHT_SET_PIN, INPUT);
  pinMode(LIGHT_SENSE_PIN, INPUT); //  Light sensor used for status light brightness

  pinMode(TEMP_SET_PIN, INPUT);
  pinMode(TEMP_SENSE_PIN, INPUT);


  pinMode(MANUAL_OPEN_PIN, INPUT_PULLUP);
  pinMode(MANUAL_CLOSE_PIN, INPUT_PULLUP);

  pinMode(HUMAN_DOOR_SWITCH, INPUT_PULLUP);
  pinMode(LIGHT_CONTROL_PIN, OUTPUT);
}

int analogSmoothing(int pin, int samples = 10) {
  int sum, average = 0;

  for (int sample = 0; sample < samples; sample++) {
    sum += analogRead(pin);
  }

  average = sum / samples;

  return average;
}

// Compare sensor to setting
bool sensorAboveThreshold(int setting_pin, int sensor_pin) {
  int sensor_val = analogSmoothing(sensor_pin);
  int setting_val = analogSmoothing(setting_pin);
  Serial.print("sensor reading, setting: ");
  Serial.print(sensor_val);
  Serial.print(", ");
  Serial.print(setting_val);

  if ( sensor_val > setting_val) {
    Serial.println("    ^");
    return true;
  }
  else {
    Serial.println("    V");
    return false;
  }
}

#ifdef CHICKEN_DOOR
void adjustChickenDoor() {
  // Chicken Door and light
  chickenDoor.check();
  Serial.print("Light ");
  bool lightOutside = sensorAboveThreshold(LIGHT_SET_PIN, LIGHT_SENSE_PIN);
  bool doorOpen = chickenDoor.isOpen();
  bool doorClosed = chickenDoor.isClosed();
  bool doorRunning = chickenDoor.isRunning();
  if ( doorRunning ) {
    // nothing to do
  }
  else {
    if ( lightOutside ) {
      if ( !doorOpen ) {
        Serial.println(" Chicken Door: Opening");
      }
      chickenDoor.open();
    }
    else {
      if ( !doorClosed ) {
        Serial.println(" Chicken Door: Closing");
      }
      chickenDoor.close();
    }

#ifdef DEBUG
    if ( !doorClosed && !doorOpen && !doorRunning )  {
      Serial.println("**** DOOR ERROR ****");
    }
#endif

  }
}
#endif

#ifdef WINDOW
void adjustWindow() {
  // Window and temperature
  window.check();
  Serial.print(" Temp ");
  bool warm = sensorAboveThreshold(TEMP_SET_PIN, TEMP_SENSE_PIN);
  bool windowOpen = window.isOpen();
  bool windowClosed = window.isClosed();
  bool windowRunning = window.isRunning();
  if ( windowRunning ) {
    // nothing to do
  }
  else {
    if ( warm ) {
      if ( !windowOpen ) {
        Serial.println(" Window: Opening");
      }
      window.open();
    }
    else {
      if ( !windowClosed ) {
        Serial.println(" Window: Closing");
      }
      window.close();
    }

#ifdef DEBUG
    if ( !windowClosed && !windowOpen && !windowRunning )  {
      Serial.println("**** WINDOW ERROR ****");
    }
#endif

  }
}
#endif


#ifdef HUMAN_DOOR
void adjustLights() {
  if (digitalRead(HUMAN_DOOR_SWITCH) == LOW) {
    digitalWrite(LIGHT_CONTROL_PIN, HIGH);
  }
  else {
    digitalWrite(LIGHT_CONTROL_PIN, HIGH);
  }
}
#endif

bool manualOverride() {
  delay(20);  // debounce
  if ( digitalRead(MANUAL_CLOSE_PIN) == LOW) {
    manualClose();
    return true;
  }

  if (digitalRead(MANUAL_OPEN_PIN) == LOW) {
    manualOpen();
    return true;
  }
  return false;
}

void manualClose() {
  Serial.println("*** Manual close all motors ***");
#ifdef CHICKEN_DOOR
  chickenDoor.close();
#endif

#ifdef WINDOW
  window.close();
#endif
}

void manualOpen() {
  Serial.println("*** Manual open all motors ***");

#ifdef CHICKEN_DOOR
  chickenDoor.open();
#endif

#ifdef WINDOW
  window.open();
#endif
}

// Check sensors and actuate motors if needed.
void adjustCoop() {

#ifdef CHICKEN_DOOR
  adjustChickenDoor();
#endif

#ifdef WINDOW
  adjustWindow();
#endif

#ifdef HUMAN_DOOR
  adjustLights();
#endif
}

void loop()
{
  if (!manualOverride()) {
    adjustCoop();
  }
}
