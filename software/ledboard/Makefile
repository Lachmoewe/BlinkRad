AVRFLAGS = -D AVRMODE -std=c99 -mmcu=attiny13 -O1 
FUSES = -U lfuse:w:0x7a:m -U hfuse:w:0xff:m 

all: program.elf

test: test.elf

program.elf: test.c
	avr-gcc test.c $(AVRFLAGS) -o program.elf
	avr-objcopy -j .text -O ihex program.elf program.hex 

flash:
	avrdude -c usbasp -p attiny13 -U program.hex

fuse:
	avrdude -c usbasp -p attiny13 $(FUSES)

clean:
	rm program.elf program.hex



