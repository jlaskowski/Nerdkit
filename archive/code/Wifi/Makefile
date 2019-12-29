GCCFLAGS=-g -Os -Wall -mmcu=atmega328p
LINKFLAGS=-Wl,-u,vfprintf -lprintf_flt -Wl,-u,vfscanf -lscanf_flt -lm
AVRDUDEFLAGS=-c avr109 -p m328p -b 115200 -P COM1
LINKOBJECTS=../libnerdkits/delay.o ../libnerdkits/lcd.o ./ui.o ./spi.o ./udpserver.o ../CC3000HostDriver/nvmem.o ../CC3000HostDriver/hci.o ../CC3000HostDriver/wlan.o ../CC3000HostDriver/cc3000_common.o ../CC3000HostDriver/evnt_handler.o ../CC3000HostDriver/security.o ../CC3000HostDriver/socket.o

all:	wifi-upload

wifi.hex:	spi.o ui.o wifi.c udpserver.o
	make -C ../libnerdkits
	avr-gcc ${GCCFLAGS} ${LINKFLAGS} -o wifi.o wifi.c ${LINKOBJECTS}
	avr-objcopy -O ihex wifi.o wifi.hex
	avr-objdump -S -d wifi.o > wifi.ass

wifi.o: wifi.c
	avr-gcc ${GCCFLAGS} -o wifi.o -c wifi.c

spi.o: spi.c spi.h
	avr-gcc ${GCCFLAGS} -o spi.o -c spi.c

ui.o: ui.c ui.h
	avr-gcc ${GCCFLAGS} -o ui.o -c ui.c

udpserver.o: udpserver.c udpserver.h
	avr-gcc ${GCCFLAGS} -o udpserver.o -c udpserver.c

wifi-upload:	wifi.hex
	avrdude ${AVRDUDEFLAGS} -U flash:w:wifi.hex:a
