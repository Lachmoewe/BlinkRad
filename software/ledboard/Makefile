AVRFLAGS = -D AVRMODE -std=c99 -mmcu=attiny13 -Os 
FUSES = -U lfuse:w:0x7a:m -U hfuse:w:0xff:m 

all: program.elf

program.elf: main.c
	avr-gcc main.c $(AVRFLAGS) -o program.elf
	avr-objcopy -j .text -O ihex program.elf program.hex 

flash:
	avrdude -c usbasp -p attiny13 -U program.hex

fuse:
	avrdude -c usbasp -p attiny13 $(FUSES)

clean:
	rm program.elf program.hex
