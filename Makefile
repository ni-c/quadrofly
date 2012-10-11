all:
	cd controller && make all && cp main.hex ../controller.hex && cp main.eep ../controller.eep
	cd motorcontrol && make all && cp main.hex ../motorcontrol.hex && cp main.eep ../motorcontrol.eep
	cd telemetrics && make all && cp main.hex ../telemetrics.hex && cp main.eep ../telemetrics.eep
clean:
	rm -f *.hex
	rm -f *.eep
	cd controller && make clean
	cd motorcontrol && make clean
	cd telemetrics && make clean
flashcontroller:
	cd controller && make clean && make all && cp main.hex ../controller.hex
	avrdude -c stk500 -p m644p -P /dev/ttyACM0 -U flash:w:controller.hex:i
flashmotorcontrol:
	cd motorcontrol && make clean && make all && cp main.hex ../motorcontrol.hex
	avrdude -c stk500 -p mm164p -P /dev/ttyACM0 -U flash:w:motorcontrol.hex:i
flashtelemetrics:
	cd telemetrics && make clean && make all && cp main.hex ../telemetrics.hex
	avrdude -c stk500 -p m644p -P /dev/ttyACM0 -U flash:w:telemetrics.hex:i
eepromcontroller:
	cd controller && make clean && make all && cp main.eep ../controller.eep
	avrdude -c stk500 -p m644p -P /dev/ttyACM0 -U eeprom:w:controller.eep:i
eeprommotorcontrol:
	cd motorcontrol && make clean && make all && cp main.eep ../motorcontrol.eep
	avrdude -c stk500 -p mm164p -P /dev/ttyACM0 -U eeprom:w:motorcontrol.eep:i
eepromtelemetrics:
	cd telemetrics && make clean && make all && cp main.eep ../telemetrics.eep
	avrdude -c stk500 -p m644p -P /dev/ttyACM0 -U eeprom:w:telemetrics.eep:i