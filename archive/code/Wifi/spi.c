#define F_CPU 14745600

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <inttypes.h>
#include <string.h>

#include "../libnerdkits/delay.h"
#include "../libnerdkits/lcd.h"
#include "../libnerdkits/uart.h"
#include "../libnerdkits/io_328p.h"


#include "../CC3000HostDriver/cc3000_common.h"
#include "../CC3000HostDriver/hci.h"
#include "../CC3000HostDriver/evnt_handler.h"
#include "../libnerdkits/io_328p.h"

#include "spi.h"
#include "ui.h"

#define HI(value)               (((value) & 0xFF00) >> 8)
#define LO(value)               ((value) & 0x00FF)

#define READ                    3
#define WRITE                   1

#define HEADERS_SIZE_EVNT       (SPI_HEADER_SIZE + 5)


#define ASSERT_CS()    (PORTB &= ~(PB2 << 1))
#define DEASSERT_CS()  (PORTB |=  (PB2 << 1))


#define 	eSPI_STATE_POWERUP 				 (0)
#define 	eSPI_STATE_INITIALIZED  		 (1)
#define 	eSPI_STATE_IDLE					 (2)
#define 	eSPI_STATE_WRITE_IRQ	   		 (3)
#define 	eSPI_STATE_WRITE_FIRST_PORTION   (4)
#define 	eSPI_STATE_WRITE_EOT			 (5)
#define 	eSPI_STATE_READ_IRQ				 (6)
#define 	eSPI_STATE_READ_FIRST_PORTION	 (7)
#define 	eSPI_STATE_READ_EOT				 (8)



unsigned char spi_buffer[CC3000_RX_BUFFER_SIZE];
unsigned char wlan_tx_buffer[CC3000_TX_BUFFER_SIZE];
unsigned short wlan_tx_buffer_len;
unsigned char add_pad_byte;

typedef struct
{
	gcSpiHandleRx  SPIRxHandler;
	unsigned short usTxPacketLength;
	unsigned short usRxPacketLength;
	unsigned long  ulSpiState;
	unsigned char *pTxPacket;
	unsigned char *pRxPacket;

}tSpiInformation;

tSpiInformation sSpiInformation;



void SpiOpen(void (*pfRxHandler)(void *p)) {

    sSpiInformation.ulSpiState = eSPI_STATE_POWERUP;

	sSpiInformation.SPIRxHandler = pfRxHandler;
	sSpiInformation.usTxPacketLength = 0;
	sSpiInformation.pTxPacket = NULL;
	sSpiInformation.pRxPacket = (unsigned char *)spi_buffer;
	sSpiInformation.usRxPacketLength = 0;

    // the write and read buffers have a special character at the end the code; the code will test whether it has
	// been overwritten, which means we've gone off the end of the bufffer (an unrecoverable error)
	spi_buffer[CC3000_RX_BUFFER_SIZE - 1] = CC3000_BUFFER_MAGIC_NUMBER;
	wlan_tx_buffer[CC3000_TX_BUFFER_SIZE - 1] = CC3000_BUFFER_MAGIC_NUMBER;

	// tell the MCU to start handling any interrupts sent by the TiWi-SL
	tSLInformation.WlanInterruptEnable();

}


// SPI_IRQ Handler
ISR(PCINT2_vect)
{

    // if SPI_IRQ was asserted (brought low)
    if (! (PIND & (1<<PD2))) {

		if (sSpiInformation.ulSpiState == eSPI_STATE_POWERUP)
		{

			// just change the state; don't do the write in an interrupt handler; other code that is
			// watching the interrupt pin will handle initiate the first write (see wlan_start in wlan.c)
			/* This means IRQ line was low call a callback of HCI Layer to inform on event */
	 		sSpiInformation.ulSpiState = eSPI_STATE_INITIALIZED;
		}
		else if (sSpiInformation.ulSpiState == eSPI_STATE_WRITE_IRQ)
		{
			SpiWriteDataSynchronous(sSpiInformation.pTxPacket, sSpiInformation.usTxPacketLength);

			sSpiInformation.ulSpiState = eSPI_STATE_IDLE;

			DEASSERT_CS();
		}
		else if (sSpiInformation.ulSpiState == eSPI_STATE_IDLE)
		{
			sSpiInformation.ulSpiState = eSPI_STATE_READ_IRQ;

			ASSERT_CS();

	        SpiReadHeader();

			sSpiInformation.ulSpiState = eSPI_STATE_READ_EOT;

			// The header was read - continue with  the payload read
			if (!SpiReadDataCont())
			{
				// All the data was read - finalize handling by switching to teh task
				//	and calling from task Event Handler
				SpiTriggerRxProcessing();
			}
		}

	// SPI_IRQ was deasserted (brought high)
    } else {
		blink_red2();
	}
}

void SpiWriteDataSynchronous(unsigned char *data, unsigned short size)
{
	while (size)
    {
        SpiWriteReadByte(*data);
		size --;
        data++;
    }
}


long SpiFirstWrite(unsigned char* pUserBuffer, unsigned short usLength) {

    if (pUserBuffer != wlan_tx_buffer) {
        memcpy(wlan_tx_buffer,pUserBuffer, usLength);
	}
    wlan_tx_buffer_len = usLength;


    delay_ms(7);  // time to wait after IRQ goes low before asserting CS (per TiWi_SL Datasheet, though it's contradictory)

	// The master asserts CS.
	ASSERT_CS();

	// The master introduces a delay of at least 50 µs before starting actual transmission of data. (as per "CC3000 Serial Port Interface (SPI)")
	delay_us(50);

	SpiWriteReadByte(wlan_tx_buffer[0]);
	SpiWriteReadByte(wlan_tx_buffer[1]);
	SpiWriteReadByte(wlan_tx_buffer[2]);
	SpiWriteReadByte(wlan_tx_buffer[3]);

	// The master introduces a delay of at least an additional 50 µs.
	delay_us(50);

	// The master transmits the rest of the packet, starting with the last byte of the header...
	SpiWriteReadByte(wlan_tx_buffer[4]);

	// ...followed by the payload, which may include a pad byte so the whole packet has an even number of bytes
	int i;
	for(i=5; i < wlan_tx_buffer_len; ++i) {
	   SpiWriteReadByte((unsigned char)wlan_tx_buffer[i]);
	}

	sSpiInformation.ulSpiState = eSPI_STATE_IDLE;

	// After the last byte of data, the nCS is deasserted by the master.
	DEASSERT_CS();

	return(0);
}


long SpiWrite(unsigned char *pUserBuffer, unsigned short usLength)
{
    unsigned char ucPad = 0;

	// Figure out the total length of the packet in order to figure out if there is padding or not
    if(!(usLength & 0x0001))
    {
        ucPad++;
    }


    pUserBuffer[0] = WRITE;
    pUserBuffer[1] = HI(usLength + ucPad);
    pUserBuffer[2] = LO(usLength + ucPad);
    pUserBuffer[3] = 0;
    pUserBuffer[4] = 0;

    usLength += (SPI_HEADER_SIZE + ucPad);

    // The magic number that resides at the end of the TX/RX buffer (1 byte after the allocated size)
    // for the purpose of detection of the overrun. If the magic number is overriten - buffer overrun
    // occurred - and we will stuck here forever!
	if (wlan_tx_buffer[CC3000_TX_BUFFER_SIZE - 1] != CC3000_BUFFER_MAGIC_NUMBER)
	{
		while (1)
			;
	}

	if (sSpiInformation.ulSpiState == eSPI_STATE_POWERUP)
	{
		while (sSpiInformation.ulSpiState != eSPI_STATE_INITIALIZED)
			;
	}

	if (sSpiInformation.ulSpiState == eSPI_STATE_INITIALIZED)
	{
		// This is time for first TX/RX transactions over SPI: the IRQ is down - so need to send read buffer size command
		SpiFirstWrite(pUserBuffer, usLength);
	}
	else
	{
		// We need to prevent here race that can occur in case 2 back to back packets are sent to the
		// device, so the state will move to IDLE and once again to not IDLE due to IRQ
		tSLInformation.WlanInterruptDisable();

		while (sSpiInformation.ulSpiState != eSPI_STATE_IDLE)
		{
			;
		}


		sSpiInformation.ulSpiState = eSPI_STATE_WRITE_IRQ;
		sSpiInformation.pTxPacket = pUserBuffer;
		sSpiInformation.usTxPacketLength = usLength;

		// Assert the CS line and wait till SSI IRQ line is active and then initialize write operation
		ASSERT_CS();

		// Re-enable IRQ - if it was not disabled - this is not a problem...
		tSLInformation.WlanInterruptEnable();
	}


	// Due to the fact that we are currently implementing a blocking situation
	// here we will wait till end of transaction
	while (eSPI_STATE_IDLE != sSpiInformation.ulSpiState)
		;

    return(0);
}


unsigned char SpiWriteReadByte(unsigned char outByte)
{
	unsigned char inByte;


	// The magic number that resides at the end of the TX/RX buffer (1 byte after the allocated size)
	// for the purpose of detection of the overrun. If the magic number is overriten - buffer overrun
	// occurred - and we will stuck here forever!
	if (wlan_tx_buffer[CC3000_TX_BUFFER_SIZE - 1] != CC3000_BUFFER_MAGIC_NUMBER) {
        lcd_write_string(PSTR("ERROR: Write buffer overrun"));
		while (1)
			;
	}
	// The magic number that resides at the end of the TX/RX buffer (1 byte after the allocated size)
	// for the purpose of detection of the overrun. If the magic number is overriten - buffer overrun
	// occurred - and we will stuck here forever!
	if (spi_buffer[CC3000_RX_BUFFER_SIZE - 1] != CC3000_BUFFER_MAGIC_NUMBER) {
        lcd_write_string(PSTR("ERROR: Read buffer overrun"));
		while (1)
			;
	}


	SPDR = outByte;
	// wait for SPIF (SPI end-of-transmission flag) to be set to know we can write another byte
	while(!(SPSR & (1<<SPIF)));

	inByte = SPDR;
	return inByte;
}


void SpiReadDataSynchronous(unsigned char *data, unsigned short size)
{
	long i = 0;

	//FILE lcd_stream = FDEV_SETUP_STREAM(lcd_putchar, 0, _FDEV_SETUP_WRITE);
	//fprintf_P(&lcd_stream, PSTR("%u "), (unsigned int)data_to_send[0]);
	//delay_ms(500);

    if (size > 0)
	    data[0]  = SpiWriteReadByte((unsigned char)READ);
    if (size > 1)
	    data[1]  = SpiWriteReadByte(0);
    if (size > 2)
	    data[2]  = SpiWriteReadByte(0);
    if (size > 3)
	    data[3]  = SpiWriteReadByte(0);
    if (size > 4)
        data[4]  = SpiWriteReadByte(0);


	for (i = 5; i < size; i ++) {
		data[i]  = SpiWriteReadByte(0);
    }
}


void SpiReadHeader(void)
{
    // the reason for 10 is 5 byte SPI header sent (requesting READ)
	// followed by min 5 byte data received
	SpiReadDataSynchronous(sSpiInformation.pRxPacket, 10);
}

long SpiReadDataCont(void)
{
    long data_to_recv;
	unsigned char *evnt_buff, type;


    //determine what type of packet we have
    evnt_buff =  sSpiInformation.pRxPacket;
    data_to_recv = 0;
	STREAM_TO_UINT8((char *)(evnt_buff + SPI_HEADER_SIZE), HCI_PACKET_TYPE_OFFSET, type);

    switch(type)
    {
        case HCI_TYPE_DATA:
        {
			// Get length from 4th and 5th byte after SPI header (must reverse bytes)
			STREAM_TO_UINT16((char *)(evnt_buff + SPI_HEADER_SIZE), HCI_DATA_LENGTH_OFFSET, data_to_recv);
			// add a padding byte if necessary
			if (!((HEADERS_SIZE_EVNT + data_to_recv) & 1)) {
    	        data_to_recv++;
			}

			if (data_to_recv) {
            	SpiReadDataSynchronous(evnt_buff + 10, data_to_recv);
			}
            break;
        }
        case HCI_TYPE_EVNT:
        {
			//
			// Calculate the rest length of the data
			//
            STREAM_TO_UINT8((char *)(evnt_buff + SPI_HEADER_SIZE), HCI_EVENT_LENGTH_OFFSET, data_to_recv);
			data_to_recv -= 1;

			// Add padding byte if needed
			if ((HEADERS_SIZE_EVNT + data_to_recv) & 1) {
	            data_to_recv++;
			}

			if (data_to_recv) {
            	SpiReadDataSynchronous(evnt_buff + 10, data_to_recv);
			}

			sSpiInformation.ulSpiState = eSPI_STATE_READ_EOT;
            break;
        }
    }

    return (0);
}


void SpiTriggerRxProcessing(void)
{

	SpiPauseSpi();

	DEASSERT_CS();

	// The magic number that resides at the end of the TX/RX buffer (1 byte after the allocated size)
	// for the purpose of detection of the overrun. If the magic number is overriten - buffer overrun
	// occurred - and we will stuck here forever!

	if (spi_buffer[CC3000_RX_BUFFER_SIZE - 1] != CC3000_BUFFER_MAGIC_NUMBER)
	{
//TODO: NEED BETTER ERROR REPORTING
		while (1)
			turn_on_red();
	}
	sSpiInformation.ulSpiState = eSPI_STATE_IDLE;
	sSpiInformation.SPIRxHandler(sSpiInformation.pRxPacket + SPI_HEADER_SIZE);
}



void log_app_state() {
  char* appStateStr = "";
  lcd_clear_and_home();
  FILE lcd_stream = FDEV_SETUP_STREAM(lcd_putchar, 0, _FDEV_SETUP_WRITE);

  switch (sSpiInformation.ulSpiState) {

	  case eSPI_STATE_POWERUP:
		appStateStr = "eSPI_STATE_POWERUP";
        lcd_write_string(PSTR("state=eSPI_STATE_POWERUP"));
		break;
      case eSPI_STATE_INITIALIZED:
        lcd_write_string(PSTR("state=eSPI_STATE_INITIALIZED"));
		break;
	  case eSPI_STATE_IDLE:
		appStateStr = "eSPI_STATE_IDLE";
        lcd_write_string(PSTR("state=eSPI_STATE_IDLE"));
		break;
	  case eSPI_STATE_WRITE_IRQ:
		appStateStr = "eSPI_STATE_WRITE_IRQ";
        lcd_write_string(PSTR("state=eSPI_STATE_WRITE_IRQ"));
		break;
	  case eSPI_STATE_WRITE_FIRST_PORTION:
		appStateStr = "eSPI_STATE_WRITE_FIRST_PORTION";
        lcd_write_string(PSTR("state=eSPI_STATE_WRITE_FIRST_PORTION"));
		//lcd_line_three();
		//fprintf_P(&lcd_stream, PSTR("wlan_tx_buffer_len=%u"), wlan_tx_buffer_len);
		break;
	  case eSPI_STATE_WRITE_EOT:
		appStateStr = "eSPI_STATE_WRITE_EOT";
        lcd_write_string(PSTR("state=eSPI_STATE_WRITE_EOT"));
		break;
	  case eSPI_STATE_READ_IRQ:
		appStateStr = "eSPI_STATE_READ_IRQ";
        lcd_write_string(PSTR("state=eSPI_STATE_READ_IRQ"));
		break;
	  case eSPI_STATE_READ_FIRST_PORTION:
		appStateStr = "eSPI_STATE_READ_FIRST_PORTION";
        lcd_write_string(PSTR("state=eSPI_STATE_READ_FIRST_PORTION"));
		break;
	  case eSPI_STATE_READ_EOT:
		appStateStr = "eSPI_STATE_READ_EOT";
        lcd_write_string(PSTR("state=eSPI_STATE_READ_EOT"));
		break;
	  default:
        fprintf_P(&lcd_stream, PSTR("state=%d"), sSpiInformation.ulSpiState);
  }
  delay_ms(1000);
}

void init_spi() {

    // Clear PRSPI (in Power Reduction Register) to enable SPI module power
    PRR &= ~(1<<PRSPI);

    // PB5 (pin 19) = SCK (Master clock output)
    // Set DDB5 on Data Direction Register for Port B (DDRB) to configure PB5 as output
    DDRB |= (1<<DDB5);

    // PB4 (pin 18) = MISO (master input) does not need to be configured to be input when MCU is SPI Master

    // PB3 (pin 17) = MOSI (master output) (DDB3 must be set properly for this to be output)
	//PORTB &= ~(1<<PB3);
    DDRB |= (1<<DDB3);

    // PB2  (pin 16) = SS (slave select) (DDB2 must be set properly for this to be output)
    DDRB |= (1<<DDB2);

	// Enable SPI, set it as Master
	SPCR = (1<<SPE)|(1<<MSTR);

	// Set slave select (SS), which is also known as Chip Select (CS) as high (which means slave not selected)
    DEASSERT_CS();

	// Make SPI Data Transmission Order to be Most Significant Bit First (MSB)
	SPCR &= ~(1<<DORD);

	// Set SPI clock characteristics as per CC3000 requirements
	SPCR &= ~(1<<CPOL);
	SPCR |= (1<<CPHA);

	// set SPI Clock with fastest speed
	SPCR |= (1<<SPR1);
	SPCR |= (1<<SPR0);

    // do NOT double the speed
	SPSR &= ~(1<<SPI2X);

	// configure Port C3 (CC3000 PWR_EN) as output
	DDRC |= (1<<DDC3);

	// configure Port D2 (CC3000 SPI_IRQ) as input
	DDRD &= ~(1<<DDD2);

	// activate the pull-up resistor on Port D2
    PORTD |= (1<<PD2);

    // enable interrupt from CC3000's SPI_IRQ on PD2
    PCICR |= (1<<PCIE2);   //Enable PCINT2
    PCMSK2 |= (1<<PCINT18); //Trigger on change of PCINT18 (PD2)

    sei();  // enable global interrupts
}

void SpiClose(void) {

}

void SpiPauseSpi() {
  PCICR &= ~(1<<PCIE2);
}

void SpiResumeSpi() {
  PCICR |= (1<<PCIE2);
}


