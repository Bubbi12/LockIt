#ifndef rfid_h
#define rfid_h
//#include <MFRC522.h>
#include <SPI.h>
#include <Arduino.h>



#include <SPI.h>



class rfid
{
public:
    rfid();
    void rfid_init();
    void printSerial(byte *serial);
    bool isSame(byte *key, byte *serial);
    bool isAuthorized(byte *serial);
    int statusRFID();
    int  nextUnused();
    void printAuthArray();
    bool authenticate(byte *serial);
    bool isBadKey();

private:
    int  AUTHORIZED_COUNT;
    byte status;
    bool Opening;
    byte serial[5];
    long afterAuthTime;
    long timeOfAuth;
    bool open;
    bool isMaster;
    bool authNext;

    int  state;
    bool isOpen;
    bool badKey;



};

#endif
