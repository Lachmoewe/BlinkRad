#include "WProgram.h"

#define DISPLAYSIZE 3

#define DATAPIN 11
#define CLKPIN 14

uint16_t display[DISPLAYSIZE];
uint16_t colorbitpos = 0;
uint8_t header = 0;


void setLed(uint8_t nr, uint8_t r, uint8_t g, uint8_t b) {
    if (DISPLAYSIZE<36) {
        display[nr] = (r<<8) + (g<<4) + b;
    }
}

void bangBit(uint8_t b) {
    if (b) {
        digitalWriteFast(DATAPIN, HIGH);          
    } else {
        digitalWriteFast(DATAPIN, LOW);           
    }
    digitalWriteFast(CLKPIN, HIGH);         // rising edge
    //delayMicroseconds(10);
    delay(10);
    digitalWriteFast(CLKPIN, LOW);          // falling edge
    delay(10);
    //delayMicroseconds(10);
}

void sendSyncPackage() {
    uint8_t i = 0;
    while (i < 14) {
        bangBit(1);
        i++;
    }
}

void sendPackage(uint8_t n) {
    uint8_t i = 0;
    bangBit(0);
    bangBit(0);
    while (i < 12) {
        uint8_t b = 11 - i;
        uint8_t c = (uint8_t)(display[n]>>b) & 0x01;                  // get the bit
        bangBit(c);
        i++;
    }
}

void sendFrame(void) {
    sendSyncPackage();
    uint8_t p = 0;
    while (p < DISPLAYSIZE) {
        sendPackage(p);
        p++;
    }
}




extern "C" int main(void)
{
    setLed(0,0,7,7);
    setLed(1,7,0,7);
    setLed(2,7,7,7);
    pinMode(DATAPIN, OUTPUT);                   // data out
    pinMode(CLKPIN, OUTPUT);                    // data clock
    
    pinMode(13, OUTPUT);                        // onboard led
    digitalWriteFast(13, HIGH);
    
    while (1) {
        sendFrame();
        digitalWriteFast(13, LOW);
        sendFrame();
        digitalWriteFast(13, HIGH);
    }
}

