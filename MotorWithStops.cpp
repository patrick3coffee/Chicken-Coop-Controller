#include "MotorWithStops.h"

#ifndef FORWARD
#define FORWARD  0
#define REVERSE 1
#endif

MotorWithStops::MotorWithStops(int _dirPin, int _pwmPin, int _closeStop, int _openStop) {
  inverted = false;

  dirPin = _dirPin;
  pwmPin = _pwmPin;
  closeStop = _closeStop;
  openStop = _openStop;

  pinMode(dirPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  pinMode(closeStop, INPUT_PULLUP);
  pinMode(openStop, INPUT_PULLUP);

  digitalWrite(dirPin, LOW);
  digitalWrite(pwmPin, LOW);

  updateStatus();
}

void MotorWithStops::invertDirection() {
  inverted = !inverted;
}

void MotorWithStops::open() {
  if (inverted) {
    digitalWrite(dirPin, REVERSE);
  }
  else {
    digitalWrite(dirPin, FORWARD);
  }
  driveMotorToStop(openStop);
}

void MotorWithStops::close() {
  if (inverted) {
    digitalWrite(dirPin, FORWARD);
  }
  else {
    digitalWrite(dirPin, REVERSE);
  }
  driveMotorToStop(closeStop);
}

void MotorWithStops::driveMotorToStop(int selectedStop) {
  digitalWrite(pwmPin, HIGH);
  updateStatus();

  while (digitalRead(selectedStop) == HIGH ) {
    delay(200);
  }

  digitalWrite(pwmPin, LOW);
  updateStatus();
}

int MotorWithStops::getStatus() {
  return currentStatus;
}

void MotorWithStops::updateStatus() {
  /*
     Status codes:
  */
  int sum = 0;
  if (digitalRead(closeStop) == LOW) {
    sum += 1;
  }
  if ( digitalRead(openStop) == LOW) {
    sum += 2;
  }
  if ( digitalRead(pwmPin) == HIGH) {
    sum += 4;
  }
  if ( digitalRead(dirPin) == HIGH) {
    sum += 8;
  }
  

  currentStatus = sum;
}

