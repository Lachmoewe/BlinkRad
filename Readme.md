# BlinkRad

Using old or broken bicycle rims as an ilumination project.

36 boards, 36 rgb-LEDs, 36 attiny13, 108 FETs, 108 resistors 
(at least if youve got 36 spoke rims).
1 controller of your choice.

## Current Status

Got the current version (as of this commit) of boards at home.
Already got one working with 24bit RGB PWM. Now Im trying
to build a useful and robust data protocol. I was thinking
about sending packages with 4bit per color, a used bit and a 
sync bit, adding up to 14bit packages.


## Further plans
- network control with teensy3.1
- develop a protocol to send to attinys
- maybe 433MHz control
- you name it!

## Getting the code to work
You will need the following packages:
```
sudo apt-get install avr-libc binutils-avr gcc-avr avrdude libusb-dev
```

If you want to edit the board, go with either Cadsoft Eagle or
edit the .brd files manually, its only xml.

Thanks to apmorton for creating the teensy template:
https://github.com/apmorton/teensy-template
