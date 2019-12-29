GCCFLAGS=-g -Os -Wall -mmcu=atmega328p
LINKFLAGS=-Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm
AVRDUDEFLAGS=-c avr109 -p m328p -b 115200 -P COM1
LINKOBJECTS=./spi.o ./wifi.o ./udphelper.o ../libnerdkits/delay.o ../CC3000HostDriver/nvmem.o ../CC3000HostDriver/hci.o ../CC3000HostDriver/wlan.o ../CC3000HostDriver/cc3000_common.o ../CC3000HostDriver/evnt_handler.o ../CC3000HostDriver/security.o ../CC3000HostDriver/socket.o ../CC3000HostDriver/netapp.o

all:	robot-upload

robot.hex:	robot.o spi.o udphelper.o wifi.o
	make -C ../libnerdkits
	avr-gcc ${GCCFLAGS} ${LINKFLAGS} -o robot.o robot.c ${LINKOBJECTS}
	avr-objcopy -O ihex robot.o robot.hex
	avr-objdump -S -d robot.o > robot.ass

robot.o: robot.c
	avr-gcc ${GCCFLAGS} -o robot.o -c robot.c

spi.o: spi.c spi.h
	avr-gcc ${GCCFLAGS} -o spi.o -c spi.c

wifi.o: wifi.c wifi.h
	avr-gcc ${GCCFLAGS} -o wifi.o -c wifi.c

udphelper.o: udphelper.c udphelper.h
	avr-gcc ${GCCFLAGS} -o udphelper.o -c udphelper.c

robot-upload:	robot.hex
	avrdude ${AVRDUDEFLAGS} -U flash:w:robot.hex:a
