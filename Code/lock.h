#ifndef lock_init_h
#define lock_h


#include <Arduino.h>
#include <Servo.h>

class lock
{
public:
    lock();
    void lock_init();
    void update(int newState);
    void open();
    void close();

private:
    const int servOnPin = 4;
    const int servoPin = 7;
    int pos = 0;
    int servoStartingPos = 0;
    int servoEndPos = 145;
    int state;
    bool isOpen;
};

#endif
