all:
	cd controller && make all && cp main.hex ../controller.hex
	cd motorcontrol && make all && cp main.hex ../motorcontrol.hex
	cd telemetrics && make all && cp main.hex ../telemetrics.hex
clean:
	rm -f *.hex
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