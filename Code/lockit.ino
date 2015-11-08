#include "rfid.h"
#include "led.h"
#include "lock.h"
#include <MFRC522.h>
#include <SPI.h>
#include <Servo.h>

led LED;
rfid RFID;
lock LOCK;

//bool openStatus = false;
//bool openNew = false;

int state = 0;
int newState = 0;


void setup() {
  Serial.begin(9600);
  LED.led_init();
  RFID.rfid_init();
  LOCK.lock_init();
  delay(1000);
}

void loop()
{
  newState = RFID.statusRFID();

  if(state != newState){
    Serial.print("State has changed from ");
    Serial.print(state);
    Serial.print(" to ");
    Serial.println(newState);

    state = newState;

    LED.update(state);
    LOCK.update(state);
    
    Serial.println("---");
  }
  LED.update();
  if(RFID.isBadKey()){
      LED.activateAlarm();
      Serial.println("entering activate alarm");
    }
  //delay(50);
  
}

