# Controller

The main controller of quadrofly

![Controller](https://raw.github.com/ni-c/quadrofly/gh-pages/images/controller.jpg)

## Schematics

![Controller Schematics](https://raw.github.com/ni-c/quadrofly/gh-pages/images/controller_schematics.png)

## Board

![Controller Schematics](https://raw.github.com/ni-c/quadrofly/gh-pages/images/controller_board.png)

## Flash

```bash
avrdude -c stk500 -p m644p -P /dev/ttyACM0 -U flash:w:controller.hex
```