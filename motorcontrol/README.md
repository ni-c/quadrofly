# Motorcontrol

Reads the RX values and generates the PWM for the motor controllers. Communicates via I2C with the Controller.

![Controller](https://raw.github.com/ni-c/quadrofly/gh-pages/images/motorcontrol.jpg)

## Schematics

![Controller Schematics](https://raw.github.com/ni-c/quadrofly/gh-pages/images/motorcontrol_schematics.jpg)

## Board

![Controller Schematics](https://raw.github.com/ni-c/quadrofly/gh-pages/images/motorcontrol_board.jpg)

## Flash

```bash
avrdude -c stk500 -p m88p -P /dev/ttyACM0 -U flash:w:motorcontrol.hex
```