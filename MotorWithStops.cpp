#include "MotorWithStops.h"

#ifndef FORWARD
#define FORWARD  0
#define REVERSE 1
#endif

MotorWithStops::MotorWithStops(int _dirPin,
                               int _pwmPin,
                               int _closeStop,
                               int _openStop,
                               bool _inverted) {
  inverted = _inverted;
  MotorWithStops(_dirPin, _pwmPin, _closeStop, _openStop);
}

MotorWithStops::MotorWithStops(int _dirPin, int _pwmPin, int _closeStop, int _openStop) {
  inverted = false;
  suspended = false;

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


void MotorWithStops::stop() {
  digitalWrite(pwmPin, LOW);

}


void MotorWithStops::suspend(bool suspend) {
  if (suspend) {
    stop();
    suspended = true;
  }
  else {
    suspended = false;
  }
}


void MotorWithStops::driveMotorToStop(int selectedStop) {
  if (suspended || digitalRead(selectedStop) == LOW ) {
    stop();
  }
  else {
    for (int i = 0; i< 255; i++){
      analogWrite(pwmPin, i);
      delay(3);
    }
    digitalWrite(pwmPin, HIGH);

    bool done = false;
    while (!done) {
      if (suspended || digitalRead(selectedStop) == LOW ) {
        done = true;
      }
      delay(200);
    }
    stop();
  }
}


bool MotorWithStops::getMotorStatus() {
  if ( digitalRead(pwmPin) == HIGH) {
    return true;
  }
  else {
    return false;
  }
}

bool MotorWithStops::getDirectionStatus() {

  if ( digitalRead(dirPin) == HIGH) {
    return true;
  }
  else {
    return false;
  }
}

bool MotorWithStops::getOpenStopStatus() {
  if ( digitalRead(openStop) == LOW) {
    return true;
  }
  else {
    return false;
  }
}

bool MotorWithStops::getCloseStopStatus() {
  if (digitalRead(closeStop) == LOW) {
    return true;
  }
  else {
    return false;
  }
}

