
#define F_CPU 80000000UL
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

uint8_t r=200;
uint8_t g=64;
uint8_t b=64;
uint8_t count = 1;

int PWM(void) {
    if (count == 0) {
        PORTB = 0b00011100;
    }
    if (r == count) {
        PORTB &= 0b11101111; //r
    }
    if (g == count) {
        PORTB &= 0b11110111; //g
    }
    if (b == count) {
        PORTB &= 0b11111011; //b
    }
    count++;
}

int main(void) {

    DDRB = 0b00011100;
    // Set up Port B data to be all low
    PORTB = 0;  

    sei();
    while (1) {
        // Toggle Port B pin 4 output state

        _delay_us(1);
    }

    return 0;
}

ISR (TIMER0_OVF_vect) {
    PWM();
}
