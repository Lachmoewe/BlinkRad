
#define F_CPU 8000000UL
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#define SCK     PB2
#define DATAIN  PB0
#define DATAOUT PB1
#define RED     PB3
#define GREEN   PB4
#define BLUE    PB5


const uint8_t lookup[16] = {
    0, 3, 7, 12, 19, 26, 35, 45, 58, 73, 91, 113, 140, 171, 209, 254
};

int PWM(uint8_t *color) {
    static uint8_t pwm_count = 0;
    if (pwm_count == 0) { PORTB |= (1<<RED)|(1<<GREEN)|(1<<BLUE); }
    if (pwm_count == *(color)  ) { PORTB &= ~(1<<RED); }
    if (pwm_count == *(color+1)) { PORTB &= ~(1<<GREEN); }
    if (pwm_count == *(color+2)) { PORTB &= ~(1<<BLUE); }
    
    pwm_count++;
}

void setColor(uint16_t cdata, uint8_t *color) {
    *(color)   = lookup[(cdata & 0x0F00)>>8];
    *(color+1) = lookup[(cdata & 0x00F0)>>4];
    *(color+2) = lookup[(cdata & 0x000F)];
}

int main(void) {

    DDRB = 0b00111010;
    // Set up Port B data to be all low
    PORTB = 0;  
    uint8_t color[3];
    uint8_t *colorptr = &color[0];
    uint16_t i=0x0880;
    uint16_t cdata = 0x0AA0;
    //setColor(0x0000, colorptr);
    *(colorptr)   = lookup[(i & 0x0F00)>>8];
    *(colorptr+1) = lookup[(i & 0x00F0)>>4];
    *(colorptr+2) = lookup[(i & 0x000F)];
    while (1) {
        uint16_t j=0;
        while (j<0x0fff) {
            //setColor(i, colorptr);
            
            //if (j = 10) { i++; }
            j++;
            PWM(colorptr);
            _delay_us(100);
        }
        color[0] = lookup[(i & 0x0F00)>>8];
        color[1] = lookup[(i & 0x00F0)>>4];
        color[2] = lookup[(i & 0x000F)];
        i--;
    }
    return 0;
}

