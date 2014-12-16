#include <stdio.h>
#include <stdint.h>


const uint8_t lookup[16] = {
    0,3,7,12,19,26,35,45,58,73,91,113,140,171,209,254
};

void setColor(uint16_t cdata, uint8_t *color) {
    *(color)   = lookup[(cdata & 0x0F00)>>8];
    *(color+1) = lookup[(cdata & 0x00F0)>>4];
    *(color+2) = lookup[(cdata & 0x000F)];
}


int main(void) {
    uint8_t color[3];
    uint8_t *cptr;
    cptr = &color[0];
    uint16_t i = 0;
    while (i<=0x0FFF) {
        setColor(i, cptr);
        printf("0x%x: %d %d %d\n", i,color[0],color[1],color[2]);
        i++;
    }
    return 0;
}
