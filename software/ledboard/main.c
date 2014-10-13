
#define F_CPU 8000000UL
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

uint8_t r,g,b;
uint8_t pwm_count;
uint8_t oldstate;

uint8_t databit;
uint16_t data;

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

    DDRB = 0b00111000;
    // Set up Port B data to be all low
    PORTB = 0;  
    
    r =  11;
    g =  64;
    b = 255;

    pwm_count = 0;
    oldstate = 0;

    while (1) {
        // Toggle Port B pin 4 output state
        if (PORTB && (1<<PB2) && !oldstate) { // check if sck rised
            oldstate = 1;

            // read datain pin
            databit = (PORTB && (1<<PB0));
            data = (data<<1);
            data |= databit;
        
        } else if (!(PORTB && (1<<PB2)) && oldstate){ // check if sck fell
            oldstate = 0;

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

