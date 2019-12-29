GCCFLAGS=-g -Os -Wall -mmcu=atmega328p

all: cc3000_common.o evnt_handler.o hci.o netapp.o nvmem.o security.o socket.o wlan.o 

cc3000_common.o: cc3000_common.c cc3000_common.h
	avr-gcc ${GCCFLAGS} -o cc3000_common.o -c cc3000_common.c

evnt_handler.o: evnt_handler.c evnt_handler.h spi.h
	avr-gcc ${GCCFLAGS} -o evnt_handler.o -c evnt_handler.c

hci.o: hci.c hci.h spi.h
	avr-gcc ${GCCFLAGS} -o hci.o -c hci.c

netapp.o: netapp.c netapp.h
	avr-gcc ${GCCFLAGS} -o netapp.o -c netapp.c

nvmem.o: nvmem.c nvmem.h
	avr-gcc ${GCCFLAGS} -o nvmem.o -c nvmem.c

security.o: security.c security.h
	avr-gcc ${GCCFLAGS} -o security.o -c security.c

socket.o: socket.c socket.h
	avr-gcc ${GCCFLAGS} -o socket.o -c socket.c

wlan.o: wlan.c wlan.h spi.h
	avr-gcc ${GCCFLAGS} -o wlan.o -c wlan.c


