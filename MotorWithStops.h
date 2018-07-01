/*
Tracks motor and limit switches for movement between end points
*/
#ifndef MotorWithStops_h
#define MotorWithStops_h


#include "Arduino.h"

class MotorWithStops {
  public:
    MotorWithStops(int _dirPin, int _pwmPin, int _closeStop, int _openStop);
    void open();
    void close();
    void invertDirection();
    bool getMotorStatus();
    bool getDirectionStatus();
    bool getOpenStopStatus();
    bool getCloseStopStatus();
  private:
    int currentStatus, dirPin, pwmPin, closeStop, openStop;
    bool inverted;
    void driveMotorToStop(int selectedStop);
    bool checkOpenStop();
    bool checkCloseStop();
    void run(bool forward);
};

#endif
