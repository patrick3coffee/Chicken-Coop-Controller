#include "MotorWithStops.h"

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

#define DEBUG


void setup()
{

#ifdef DEBUG
  Serial.begin(9600);
  Serial.println("Chicken Coop Controller");
#endif
  setupArdumoto(); // Set all pins as outputs
  pinMode(LIGHT_SET_PIN, INPUT);
  pinMode(LIGHT_SENSE_PIN, INPUT);
  pinMode(TEMP_SET_PIN, INPUT);
  pinMode(TEMP_SENSE_PIN, INPUT);
  
  pinMode(DOOR_CLOSED_LIMIT, INPUT_PULLUP);
  pinMode(DOOR_OPEN_LIMIT, INPUT_PULLUP);
  pinMode(WINDOW_CLOSED_LIMIT, INPUT_PULLUP);
  pinMode(WINDOW_OPEN_LIMIT, INPUT_PULLUP);
}


void loop()
{
  adjustDoor();
  //  adjustWindow();
#ifdef DEBUG
  delay(500);
#else
  sleepForAMinute();
#endif
}


void sleepForAMinute() {
  delay(60000);
}


void adjustDoor() {
  if (lightAboveThreshold()) {
    openDoor(true);
  }
  else {
    openDoor(false);
  }
}


bool lightAboveThreshold() {
  int sensor_val = analogRead(LIGHT_SENSE_PIN);
  int setting_val = analogRead(LIGHT_SET_PIN);
#ifdef DEBUG
  Serial.print("Light reading: ");
  Serial.println(sensor_val);
  Serial.print("Light setting: ");
  Serial.println(setting_val);
#endif

  if ( sensor_val > setting_val) {

#ifdef DEBUG
    Serial.println("Light above threshold.");
#endif

    return true;
  }
  else {

#ifdef DEBUG
    Serial.println("Light below threshold.");
#endif

    return false;
  }
}

void openDoor(bool openDoor) {
  if (openDoor) {
#ifdef DEBUG
    Serial.println("Open door");
#endif
    digitalWrite(DIR_DOOR_PIN, LOW);
    digitalWrite(PWM_DOOR_PIN, HIGH);
    while (digitalRead(DOOR_OPEN_LIMIT) == HIGH) {
      delay(200);
    }
    digitalWrite(PWM_DOOR_PIN, LOW);
  }
  else {
#ifdef DEBUG
    Serial.println("Close door");
#endif
    digitalWrite(DIR_DOOR_PIN, HIGH);
    digitalWrite(PWM_DOOR_PIN, HIGH);
    while (digitalRead(DOOR_CLOSED_LIMIT) == HIGH) {
      delay(200);
    }
    digitalWrite(PWM_DOOR_PIN, LOW);
  }
}


// setupArdumoto initialize all pins
void setupArdumoto()
{
  // All pins should be setup as outputs:
  pinMode(PWM_DOOR_PIN, OUTPUT);
  pinMode(PWM_WINDOW_PIN, OUTPUT);
  pinMode(DIR_DOOR_PIN, OUTPUT);
  pinMode(DIR_WINDOW_PIN, OUTPUT);

  // Initialize all pins as low:
  digitalWrite(PWM_DOOR_PIN, LOW);
  digitalWrite(PWM_WINDOW_PIN, LOW);
  digitalWrite(DIR_DOOR_PIN, LOW);
  digitalWrite(DIR_WINDOW_PIN, LOW);
}

