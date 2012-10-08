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
	avrdude -c stk500 -p m644p -P /dev/ttyACM0 -U flash:w:controller.hex
flashmotorcontrol:
	cd motorcontrol && make clean && make all && cp main.hex ../motorcontrol.hex
	avrdude -c stk500 -p mm164p -P /dev/ttyACM0 -U flash:w:motorcontrol.hex
flashtelemetrics:
	cd telemetrics && make clean && make all && cp main.hex ../telemetrics.hex
	avrdude -c stk500 -p m644p -P /dev/ttyACM0 -U flash:w:telemetrics.hex
eepromcontroller:
	cd controller && make clean && make all && cp main.hex ../controller.eep
	avrdude -c stk500 -p m644p -P /dev/ttyACM0 -U flash:w:controller.eep
eeprommotorcontrol:
	cd motorcontrol && make clean && make all && cp main.hex ../motorcontrol.eep
	avrdude -c stk500 -p mm164p -P /dev/ttyACM0 -U flash:w:motorcontrol.eep
eepromtelemetrics:
	cd telemetrics && make clean && make all && cp main.hex ../telemetrics.eep
	avrdude -c stk500 -p m644p -P /dev/ttyACM0 -U flash:w:telemetrics.eep
	