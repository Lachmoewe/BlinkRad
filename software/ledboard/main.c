
#define F_CPU 8000000UL
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define SCK     PB2
#define DATAIN  PB0
#define DATAOUT PB1

uint8_t r,g,b;

const uint8_t lookup[16] = {
    0, 3, 7, 12, 19, 26, 35, 45, 58, 73, 91, 113, 140, 171, 209, 254
};

int PWM(void) {
    static uint8_t pwm_count = 0;
    if      (pwm_count == 0) { PORTB |= (1<<PB3)|(1<<PB4)|(1<<PB5); }
    else if (pwm_count == r) { PORTB &= ~(1<<PB3); }
    else if (pwm_count == g) { PORTB &= ~(1<<PB4); }
    else if (pwm_count == b) { PORTB &= ~(1<<PB5); }
    
    pwm_count++;
}

void setColor(uint16_t cdata) {
    r = lookup[(cdata & 0x0F00)>>8];
    g = lookup[(cdata & 0x00F0)>>4];
    b = lookup[cdata & 0x000F];
}

int main(void) {

    DDRB = 0b00111010;
    // Set up Port B data to be all low
    PORTB = 0;  
    
    r = 0;
    g = 255;
    b = 255;

    uint8_t bit_count = 0;
    uint8_t sck_oldstate = 0;
    uint8_t gotdata = 0;
    uint8_t databit = 0;
    uint16_t data = 0;

    const uint16_t sync_sequence = 0x3FFF;    // 14 bit set to 1
    const uint16_t pattern       = 0x3FFF;          // 14 bit set to 1
    const uint16_t syncbit       = 0x2000;
    const uint16_t usedbit       = 0x1000;

    while (1) {
        // Toggle Port B pin 4 output state
        if ((PORTB && (1<<SCK)) && !sck_oldstate) { // check if sck rised
            sck_oldstate = 1;

            // read datain pin, shift data one left and write databit to lowest pos
            if (PORTB & (1<<DATAIN)) { databit = 1; } else { databit = 0; }
            data = (data<<1);
            data |= databit;

            // set 4 highest bits 0
            data = data & pattern;
            
            if (bit_count == 13) {          // check if we received a full package yet
                if (!(data && usedbit)) {      // check if this package was already used
                    if (!gotdata) {
                        gotdata = 1;
                        setColor( (data & 0x0FFF) ); // send only rgb info
                    }
                }
            }
            bit_count++;

            if (bit_count == 14) {
                bit_count = 0;
            }
            if (data == sync_sequence) {
                // we just found a sync sequence, reset the bit counter
                bit_count = 0;
                gotdata = 0;
            } 

        } else if (!(PORTB & (1<<SCK)) && sck_oldstate){ // check if sck fell
            sck_oldstate = 0;
            
            // write dataout pin
            if (databit == 1) {
                PORTB |= (1<<DATAOUT);
            } else if ( (bit_count == 1) && !gotdata && !databit ) {
                // this sets the usedbit for the first unusedpackage in the datastream
                PORTB |= (1<<DATAOUT);
            } else {
                PORTB &= ~(1<<DATAOUT);
            }
        }
        PWM();
        _delay_us(1);
    }

    return 0;
}

