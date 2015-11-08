
#include "lock.h"
#include "Arduino.h"
#include <Servo.h>

Servo myservo;
  
lock::lock(){
}


void lock::lock_init(){
    //const int servOnPin = 4;
    //const int servoPin = 7;
    
    pinMode(servOnPin, OUTPUT);

    // state 0 is unlocked 
    // state 1 is locked
    // state 2 is ready for authorization
    open();
    state = 0;


}   


void lock::update(int newState){
    state = newState;
    //Serial.println(state);
    if(state == 0 && !isOpen){
        open();
    }
    else if(state == 1 && isOpen){
        close();
    }
}

void lock::open(){
    myservo.attach(servoPin);
    digitalWrite(servOnPin, HIGH);
    //Serial.println("Lock Opening");
    for(pos = servoStartingPos; pos <= servoEndPos; pos += 1){
      myservo.write(pos);
      delay(5);
    } 
    myservo.detach();
    digitalWrite(servOnPin, LOW);
    isOpen = true;
}

void lock::close(){
    myservo.attach(servoPin);
    digitalWrite(servOnPin, HIGH);
    //Serial.println("Lock Closing");
    for(pos = servoEndPos; pos>=servoStartingPos; pos-=1){
      myservo.write(pos);
      delay(5);
    }
    myservo.detach();
    digitalWrite(servOnPin, LOW);
    isOpen = false;
}   


