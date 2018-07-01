/*
Tracks motor and limit switches for movement between end points
*/
#ifndef MotorWithStops_h
#define MotorWithStops_h


#include "Arduino.h"

class MotorWithStops {
  public:
    MotorWithStops(int _dirPin, int _pwmPin, int _closeStop, int _openStop);
    MotorWithStops(int _dirPin, int _pwmPin, int _closeStop, int _openStop, bool _inverted);
    void open();
    void close();
    void stop();
    void suspend(bool suspend);
    bool getMotorStatus();
    bool getDirectionStatus();
    bool getOpenStopStatus();
    bool getCloseStopStatus();
  private:
    int currentStatus, dirPin, pwmPin, closeStop, openStop;
    bool inverted, suspended;
    void driveMotorToStop(int selectedStop);
    bool checkOpenStop();
    bool checkCloseStop();
    void run(bool forward);
};

#endif
