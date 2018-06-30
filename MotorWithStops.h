/*
Tracks motor and limit switches for movement between end points
*/
#ifndef MotorWithStops_h
#define MotorWithStops_h


#include "Arduino.h"

class MotorWithStops {
  public:
    MotorWithStops(int dirPin, int pwmPin, int closeStop, int openStop);
    void open();
    void close();
    int getStatus();
  private:
    int status;
    bool checkOpenStop();
    bool checkCloseStop();
    void run(bool forward);
};

#endif
