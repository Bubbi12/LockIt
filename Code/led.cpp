
#include "led.h"
#include "Arduino.h"


led::led(){
}


void led::led_init(){
    //ledPinGreen = 8;
    //ledPinRed   = 9;
    pinMode(ledPinGreen, OUTPUT);
    pinMode(ledPinRed,   OUTPUT);
    lightOnTime = 500;
    lastTurnedOn = millis();
    open = false;

    // state 0 is unlocked 
    // state 1 is locked
    // state 2 is ready for authorization
    state = 0;
}   

void led::greenOn(){
    lastTurnedOn = millis();
    digitalWrite(ledPinGreen,HIGH);
}


void led::redOn(){
    lastTurnedOn = millis();
    digitalWrite(ledPinRed,HIGH);
}

void led::greenOff(){
  digitalWrite(ledPinGreen,LOW);
}

void led::redOff(){
  digitalWrite(ledPinRed,LOW);
}

void led::update(){


    if(state == 0 || state == 1){
      if(millis()-lightOnTime > lastTurnedOn){
        greenOff();
        redOff();
      }
    }
    else if(state == 2){
      if(millis()-lightOnTime*0.4 > lastTurnedOn){
        greenOn();
        redOn();
      }
      else if(millis()-lightOnTime*0.2 > lastTurnedOn){
        greenOff();
        redOff();
      }
    }
    else if(state == 3){
      if(millis()-lightOnTime*0.2 > lastTurnedOn){
        greenOn();
      }
      else if(millis()-lightOnTime*0.1 > lastTurnedOn){
        greenOff();
      }
    }
    
}

void led::activateAlarm(){
    redOn();
}

void led::update(int newState){
    if(newState == 0){
        greenOn();
    }
    else if(newState == 1){
        greenOn();
    }
    
    state = newState;


}
