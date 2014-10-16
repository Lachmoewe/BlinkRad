
#define F_CPU 8000000UL
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

uint8_t r,g,b;
uint8_t pwm_count, bit_count;
uint8_t sck_oldstate, used_oldstate;
uint8_t changed_data;
uint8_t use_this_package;

uint8_t databit;
uint16_t data;

const uint8_t lookup[16] = {
    0, 3, 7, 12, 19, 26, 35, 45, 58, 73, 91, 113, 140, 171, 209, 254
};

int PWM(void) {
    if (pwm_count == 0) {
        PORTB |= (1<<PB3)|(1<<PB4)|(1<<PB5); //0b00011100;
    }
    if (pwm_count == r) {
        PORTB &= ~(1<<PB3); //0b11101111; //r
    }
    if (pwm_count == g) {
        PORTB &= ~(1<<PB4); //0b11110111; //g
    }
    if (pwm_count == b) {
        PORTB &= ~(1<<PB5); //0b11111011; //b
    }
    pwm_count++;
}

int main(void) {

    DDRB = 0b00111010;
    // Set up Port B data to be all low
    PORTB = 0;  
    
    r = 255;
    g = 255;
    b = 255;

    pwm_count = 0;
    bit_count = 0;
    sck_oldstate = 0;
    used_oldstate = 1;
    use_this_package = 0;

    uint16_t sync_sequence = 0x0FFF;


    while (1) {
        // Toggle Port B pin 4 output state
        if ((PORTB && (1<<PB2)) && !sck_oldstate) { // check if sck rised
            sck_oldstate = 1;

            // read datain pin, shift data one left and write databit to lowest pos
            databit = (PORTB && (1<<PB0));
            data = (data<<1);
            data |= databit;

            if ((data & sync_sequence) == sync_sequence) {
                // we just found a sync sequence, reset the bit counter
                bit_count = 0;
            } else if ((bit_count == 1) && !changed_data) { // check if we got the used bit and didnt update rgb yet
                if (!databit && used_oldstate) { // ckeck if the used bit is not set but the one before was
                    use_this_package = 1;
                    databit = 1; // set the used bit for next ledboard
                    // TODO we still net to set used_oldstate somewhere!!!
                    used_oldstate = 0;
                } else if (databit) {
                    used_oldstate = 1;
                }
                bit_count++;
            } else if ((bit_count == 13) && use_this_package) {
                // we now received a full package and write its data to rgb
                r = lookup[(data & 0x0F00)>>8];
                g = lookup[(data & 0x00F0)>>4];
                b = lookup[data & 0x000F];
                use_this_package = 0; // we wont use this package anymore
                bit_count++;
            } else {
                bit_count++;
            } 
            if (bit_count == 14) {
                bit_count = 0;
            }

        } else if (!(PORTB && (1<<PB2)) && sck_oldstate){ // check if sck fell
            sck_oldstate = 0;

            // write dataout pin
            if (databit) {
                PORTB |= (1<<PB1);
            } else {
                PORTB &= ~(1<<PB1);
            }
        }
        PWM();
        _delay_us(1);
    }

    return 0;
}

