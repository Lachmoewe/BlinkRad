#include "WProgram.h"

uint16_t display[37] = {
    //xxsurrrgggbbb
    0b0011111111111,
    0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000,
    0x0000,0x0000,0x0000,0x0000    
};

void setLed(uint8_t nr, uint8_t r, uint8_t g, uint8_t b) {
    if (nr<36) {
        display[nr+1] = (r<<8) + (g<<2) + b;
    }
}

uint8_t nextBit(void) {
    static uint16_t bitpos;
    
}

extern "C" int main(void)
{

	pinMode(13, OUTPUT);
    setLed(0,15,8,0);
	while (1) {
		digitalWriteFast(13, HIGH);
		delay(500);
		digitalWriteFast(13, LOW);
		delay(500);
	}
}

