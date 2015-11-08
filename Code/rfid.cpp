#include "rfid.h"
#include "Arduino.h"
#include <SPI.h>

#include <MFRC522.h>
MFRC522 nfc(10,5);

const int authCount = 4 ;


byte data[MAX_LEN];
byte Authorized[authCount][6] =  {
  {0x29, 0x6A, 0x5E, 0x4A, 0xFF, 0xFF, },
  {0x79, 0xED, 0x89, 0xDC, 0xFF, 0xFF, },
  {0x29, 0x6A, 0x5E, 0x4A, 0xFF, 0xFF, },
  {0x29, 0x6A, 0x5E, 0x4A, 0xFF, 0xFF, }                             
};

bool active[authCount] = {true,true,false,false};

rfid::rfid()
{
}


void rfid::rfid_init()
{
  Serial.println("Looking for MFRC522.");
  SPI.begin();
  Serial.println("Looking for MFRC522.");
  nfc.begin();
  byte version = nfc.getFirmwareVersion();
  Serial.println(version, HEX);
  if(! version)
  {
    Serial.print("Didn't find MFRC522 board.");
    while(1)
    {
      Serial.println("Board not found");
    }
  }
  Serial.print("Found chip MFRC522 ");
  Serial.print("Firmware ver. 0x");
  Serial.print(version, HEX);
  Serial.println(".");

  AUTHORIZED_COUNT= authCount;

  afterAuthTime = 1500;
  timeOfAuth = millis();
  open = false;

  Opening = false;

  // state 0 is unlocked 
  // state 1 is locked
  // state 2 is ready for authorization
  state = 0;
  isOpen = true;
  badKey = false;

  isMaster = false;
  authNext = false;
  nextUnused();
  printAuthArray();
  
}

void rfid::printAuthArray()
{
  for(int i=0;i<AUTHORIZED_COUNT;i++)
  {
    for(int b=0;b<4;b++)
    {
      Serial.print(Authorized[i][b],HEX);
      Serial.print(" ");
    }
    Serial.println("");
  }
}

bool rfid::authenticate(byte *serial)
{
  int row = nextUnused();
  if(row==0)
  {
    return false;
  }
  else
  {
    for(int i=0;i<4;i++)
    {
      Authorized[row][i] = serial[i];
    }
    active[row] = true;
    //printAuthArray();
    return true;
  }
}

int rfid::nextUnused()
{
  for (int i = 1; i < AUTHORIZED_COUNT;i++)
  {
    if(!active[i])
    {
      Serial.print("Next unused is:");
      Serial.println(i);
      
      return i;
    }
  }
  
  Serial.println("No row is unused");
  return 0;
}

boolean rfid::isSame(byte *key, byte *serial)
{
  for(int i = 0; i < 4; i++)
  {
    if(key[i] != serial[i])
    {
      return false;
    }
  }
  return true;
}

boolean  rfid::isAuthorized(byte *serial)
{
  for(int i = 0; i < AUTHORIZED_COUNT; i++)
  {
    if(active[i] && isSame(serial, Authorized[i]))
    {
      if(i==0){isMaster=true;}
      else{isMaster=false;}
      return true;
    }
  }
  
  return false;
}

void  rfid::printSerial(byte *serial)
{
  Serial.print("Serial:");
  for(int i = 0; i < 4; i++)
  {
    Serial.print(serial[i], HEX);
    Serial.print(" ");
  }
}

bool rfid::isBadKey(){
  if(badKey == true){
    badKey = !badKey;
    return true;
  }
  return false;
}




int rfid::statusRFID()
{
	status = nfc.requestTag(MF1_REQIDL, data);
  if(status == MI_OK)
  {
    status = nfc.antiCollision(data);
    memcpy(serial, data, 5);

    bool isAuth = isAuthorized(serial);
        

    // Ef key er authorized og lock er opinn, setja í state 0 (open)
    if(!authNext && !isOpen && isAuth && (millis() - afterAuthTime) > timeOfAuth){
      Serial.println("key detected: opening");
      timeOfAuth = millis();
      isOpen = !isOpen; 
      state = 0;
    }  
    // Ef key er authorized og lock er lokaður, setja í state 1 (closed)
    else if(!authNext && isOpen && isAuth && (millis() - afterAuthTime) > timeOfAuth){
      Serial.println("key detected: closing");
      timeOfAuth = millis();
      isOpen = !isOpen;
      state = 1; 
    }  
    else if(!isAuth && (millis() - afterAuthTime*0.5  ) > timeOfAuth)
    {
      //printSerial(serial);
      //timeOfAuth = millis();
      //Serial.println("is NOT authenticated");
    }
    
    

    //Ef key er authorized sem master lykill og er haldið inni lengur inni: enter authNext state
    if(!authNext && isMaster && isAuth && (millis() - afterAuthTime*0.8) > timeOfAuth && (millis() - afterAuthTime) < timeOfAuth){
      Serial.println("Ready for authenticating new chip");
      authNext = true;
      isMaster = false;
      timeOfAuth = millis();
      state = 2;
    }
    else if(authNext && !isAuth &&  (millis() - afterAuthTime*0.5) > timeOfAuth && (millis() - afterAuthTime*2) < timeOfAuth){
      printSerial(serial);
      Serial.println("is now authenticated");
      authenticate(serial);
      authNext = false;
      timeOfAuth = millis();
      state = 3;
    }



    // Ef key er ekki authorized og state er ekki í authentication mode
    if(!isAuth && state != 2 && state != 3){
      badKey = true;
      Serial.println("not a valid key");
    }
    else{
      badKey = false;
      Serial.println("valid key");
    }

    nfc.haltTag();
    
  }

  // Ef ekkert merki nýtt tag: abort stage 2
  if((state == 2 || state == 3) && (millis() - afterAuthTime*2) > timeOfAuth){
      authNext = false;
      if(state == 2){
        Serial.println("No new key entered, aborting authentication");
      }
      else if(state == 3){
        Serial.println("Returning to normal operation");
      }
      
      if(isOpen){
        state = 0;
      }
      else{
        state = 1;
      }
      //nullstilli timer + smá delay
      timeOfAuth = millis()+1000;
    }
  
  return state;
  //return open;
}
