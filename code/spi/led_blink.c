/*****************************************************************************
//  File Name    : avrspi.c
//  Version      : 1.0
//  Description  : SPI I/O Using 74HC595 8-bit shift registers
//                 with output latch
//  Author       : RWB
//  Target       : AVRJazz Mega168 Board
//  Compiler     : AVR-GCC 4.3.0; avr-libc 1.6.2 (WinAVR 20080610)
//  IDE          : Atmel AVR Studio 4.14
//  Programmer   : AVRJazz Mega168 STK500 v2.0 Bootloader
//               : AVR Visual Studio 4.14, STK500 programmer
//  Last Updated : 28 May 2009
*****************************************************************************/
#include <avr/io.h>
#include "../libnerdkits/delay.h"
#include "../libnerdkits/io_328p.h"


#define SPI_PORT PORTB
#define SPI_DDR  DDRB
#define SPI_CS   PB2

unsigned char SPI_WriteRead(unsigned char dataout);
void blink_led();

int main(void)
{
  // LED as output
  DDRC |= (1<<PC4);

  unsigned char cnt;

  // Set the PORTD as Output:
  DDRD=0xFF;
  PORTD=0x00;

  // Initial the AVR ATMega168 SPI Peripheral

  // Set SS, MOSI and SCK as output, others as input
  SPI_DDR = (1<<PB2)|(1<<PB3)|(1<<PB5);

  // SS Low
  SPI_PORT &= ~(1<<SPI_CS);

  // Enable SPI, Master, set clock rate fck/2 (maximum)
  SPCR = (1<<SPE)|(1<<MSTR);
  SPSR = (1<<SPI2X);

  delay_ms(2000);

  // send a test byte
  cnt=SPI_WriteRead(0);

  return 0;
}

unsigned char SPI_WriteRead(unsigned char dataout)
{
  unsigned char datain;

  // Start transmission (MOSI)
  SPDR = dataout;
blink_led();
  // Wait for transmission complete
  while(!(SPSR & (1<<SPIF)));
blink_led();
  // Get return Value;
  datain = SPDR;

  // Latch the Output using rising pulse to the RCK Pin
  SPI_PORT |= (1<<SPI_CS);

  delay_us(1);             // Hold pulse for 1 micro second

  // Disable Latch
  SPI_PORT &= ~(1<<SPI_CS);

  // Return Serial In Value (MISO)
  return datain;
}


void blink_led() {
    PORTC |= (1<<PC4);
    PORTC &= ~(1<<PC5);

    //delay for 500 milliseconds to let the light stay on
    delay_ms(2000);

    // turn off LED
    PORTC &= ~(1<<PC4);
    PORTC |= (1<<PC5);

    //delay for 500 milliseconds to let the light stay off
    delay_ms(2000);

}