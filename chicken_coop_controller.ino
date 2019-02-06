#include "MotorWithStops.h"
/*
    MotorWithStops library located here:
    https://github.com/patrick3making/Motor-with-Stops
*/

//#define DEBUG

/*
    Pin Assignments
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
#define LIGHT_SENSE_PIN A3
#define TEMP_SENSE_PIN A2

// Mode selector switch
#define SUSPEND_PIN 2
#define MANUAL_OPEN_PIN 8

// Enable coop actuators
#define DOOR     // enable coop door funtionality
//#define WINDOW   // enable coop window funcitonality

// Setup motor control objects

#ifdef DOOR
MotorWithStops door(DIR_DOOR_PIN, PWM_DOOR_PIN, true, DOOR_CLOSED_LIMIT, DOOR_OPEN_LIMIT, false, true);
#endif

#ifdef WINDOW
MotorWithStops window(DIR_WINDOW_PIN, PWM_WINDOW_PIN, false, WINDOW_CLOSED_LIMIT, WINDOW_OPEN_LIMIT);
#endif


void setup()
{

#ifdef DEBUG
  // setup serial
  Serial.begin(9600);
  Serial.println("Chicken Coop Controller");
#endif

  // setup pins
  pinMode(LIGHT_SET_PIN, INPUT);
  pinMode(LIGHT_SENSE_PIN, INPUT); //  Light sensor used for status light brightness

  pinMode(TEMP_SET_PIN, INPUT);
  pinMode(TEMP_SENSE_PIN, INPUT);


  pinMode(MANUAL_OPEN_PIN, INPUT_PULLUP);
  pinMode(SUSPEND_PIN, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(SUSPEND_PIN), overrideInterrupt, CHANGE);
}


void loop()
{
  if (digitalRead(SUSPEND_PIN) == LOW) {
    suspendMotors();
  }
  else if (digitalRead(MANUAL_OPEN_PIN) == LOW) {
    manualOpen();
  }
  adjustCoop();
  wasteSomeTime();
}


void wasteSomeTime() {

#ifdef DEBUG
  Serial.println(".");
  delay(1000);    // Check twice a second for serial output sake.
#else
  delay(5000);  // Check every 5 seconds because temperature and sunlight change slowly.
#endif
}


// Check sensors and actuate motors if needed.
void adjustCoop() {

#ifdef DOOR
  adjustDoor();
#endif

#ifdef WINDOW
  adjustWindow();
#endif
}

#ifdef DOOR
void adjustDoor() {
  // Door and light
#ifdef DEBUG
  Serial.println("Adjusting Door");
  if (door.isOpen()) {
    Serial.println("  Door: open");
  }
  else {
    Serial.println("  Door: not open");
  }
  if (door.isClosed()) {
    Serial.println("  Door: closed");
  }
  else {
    Serial.println("  Door: not closed");
  }
  Serial.println(" Light:");
#endif
  if (sensorAboveThreshold(LIGHT_SET_PIN, LIGHT_SENSE_PIN)) {
    door.open();
  }
  else {
    door.close();
  }
}
#endif

#ifdef WINDOW
void adjustWindow() {
  // Window and temperature
#ifdef DEBUG
  Serial.println("Adjusting Window");
  if (door.isOpen()) {
    Serial.println("  Window: open");
  }
  else {
    Serial.println("  Window: not open");
  }
  if (door.isClosed()) {
    Serial.println("  Window: closed");
  }
  else {
    Serial.println("  Window: not closed");
  }
  Serial.println(" Temperature:");
#endif

  if (sensorAboveThreshold(TEMP_SET_PIN, TEMP_SENSE_PIN)) {
    window.open();
  }
  else {
    window.close();
  }
}
#endif

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


void overrideInterrupt() {
  delay(20);  // debounce
  if ( digitalRead(SUSPEND_PIN) == LOW) {
#ifdef DEBUG
    Serial.println("*** Suspending all motors ***");
#endif
    suspendMotors();
  }
  else {
#ifdef DEBUG
    Serial.println("*** Resuming all motors ***");
#endif
    resumeMotors();
  }

  if (digitalRead(MANUAL_OPEN_PIN) == LOW) {
    manualOpen();
  }
}

void suspendMotors() {
#ifdef DOOR
  door.suspend(true);
#endif

#ifdef WINDOW
  window.suspend(true);
#endif
  while (digitalRead(SUSPEND_PIN) == LOW) {
    delay(100);
  }
}

void resumeMotors() {
#ifdef DOOR
  door.suspend(false);
#endif

#ifdef WINDOW
  window.suspend(false);
#endif
}

void manualOpen() {
#ifdef DEBUG
  Serial.println("*** Manual open all motors ***");
#endif

#ifdef DOOR
  door.open();
#endif

#ifdef WINDOW
  window.open();
#endif
  while (digitalRead(MANUAL_OPEN_PIN) == LOW) {
    delay(100);
  }
}

