#ifndef led_h
#define led_h


#include <Arduino.h>

class led
{
public:
    led();
    void led_init();
    void update();
    void update(int newState);
    void activateAlarm();

private:
    const int ledPinGreen = 8;
    const int ledPinRed = 9;
    long lightOnTime;
    long lastTurnedOn;
    int  state;
    bool open;
    void greenOn();
    void redOn();
    void greenOff();
    void redOff();
};

#endif
