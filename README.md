# Quadrofly

Building a Quadrocopter from scratch.

![Quadrofly](https://raw.github.com/ni-c/quadrofly/gh-pages/images/quadrofly.jpg)

## Directory Structure

- `/shared` – Libraries that are used by the main controller and the motor control unit
- `/controller` – The code for the main controller of the Quadrocopter, an ATMega644
- `/motorcontrol` – The code for the motor control unit, an ATMega884
- `/telemetrics` – The code for the telemetrics module, an ATMega1284
- `/contrib` – Contributions like datasheets and schematics

## Documentation

Source code documentation is generated with Doxygen and can be viewed under http://ni-c.github.com/quadrofly/

More information about Quadrofly on my blog: http://quadrofly.ni-c.de

## Modules

### Controller

The controller is the main controller of the Quadrocopter. It is responsible for the following tasks:

- Generation of 3.3 Volt power supply
- I2C Master
- I2C communication with the MPU-Gyroscope
- I2C Step-Up converter to 5 Volt
- I2C communication with the *Motorcontrol* module
- RF communication with the *Telemetrics* module
- Calculations (Kalman-Filter, PID-Control)

#### Schematics

![Controller Schematics](https://raw.github.com/ni-c/quadrofly/gh-pages/images/controller_schematics.png)

#### Board

![Controller Board](https://raw.github.com/ni-c/quadrofly/gh-pages/images/controller_board.png)

#### Assembled

![Controller](https://raw.github.com/ni-c/quadrofly/gh-pages/images/controller.jpg)

### Motorcontrol

The motorcontrol is responsible for the following tasks:

- Generation of 5 Volt power supply
- I2C Slave
- Receiving and decoding RC-Signals
- Generating PWN for the brushless controllers
- Measuring battery power

#### Schematics

![Motorcontrol Schematics](https://raw.github.com/ni-c/quadrofly/gh-pages/images/motorcontrol_schematics.png)

#### Board

![Motorcontrol Board](https://raw.github.com/ni-c/quadrofly/gh-pages/images/motorcontrol_board.png)

#### Assembled

![Motorcontrol](https://raw.github.com/ni-c/quadrofly/gh-pages/images/motorcontrol.jpg)

### Telemetrics

The telemetrics is responsible for the following tasks:

- RF communication with the Quadrocopter
- Telemetrics logging

#### Schematics

![Telemetrics Schematics](https://raw.github.com/ni-c/quadrofly/gh-pages/images/telemetrics_schematics.png)

#### Assembled

![Telemetrics](https://raw.github.com/ni-c/quadrofly/gh-pages/images/telemetrics.jpg)

## License

MIT License

Copyright (c) 2011-2013 Willi Thiel (wthiel@quadrofly.ni-c.de)

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
