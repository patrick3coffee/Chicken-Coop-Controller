#include "MotorWithStops.h"

MotorWithStops::MotorWithStops(int dirPin, int pwmPin, int closeStop, int openStop) {
  pinMode(dirPin, OUTPUT);
  pinMode(PWMPin, OUTPUT);
  pinMode(closeStop, INPUT_PULLUP);
  pinMode(openStop, INPUT_PULLUP);
  
}

void MotorWithStops::open() {

}

void MotorWithStops::close() {

}

int MotorWithStops::getStatus() {

}

