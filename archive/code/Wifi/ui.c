#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <inttypes.h>
#include <string.h>

#include "../libnerdkits/delay.h"
#include "../libnerdkits/lcd.h"
#include "../libnerdkits/uart.h"
#include "../libnerdkits/io_328p.h"

void turn_on_red() {
	PORTD |= (1<<PD3);
}
void turn_on_red2() {
	PORTD |= (1<<PD4);
}
void turn_on_green() {
	PORTD |= (1<<PD5);
}
void turn_on_green2() {
	PORTD |= (1<<PD6);
}
void turn_on_yellow() {
	PORTD |= (1<<PD7);
}

void turn_off_red() {
	PORTD &= ~(1<<PD3);
}
void turn_off_red2() {
	PORTD &= ~(1<<PD4);
}
void turn_off_green() {
	PORTD &= ~(1<<PD5);
}
void turn_off_green2() {
	PORTC &= ~(1<<PD6);
}
void turn_off_yellow() {
	PORTC &= ~(1<<PD7);
}
void blink_red() {
  turn_on_red();
  delay_ms(500);
  turn_off_red();
  delay_ms(300);
}
void blink_red2() {
  turn_on_red2();
  delay_ms(500);
  turn_off_red2();
  //delay_ms(300);
}
void blink_green() {
  turn_on_green();
  delay_ms(500);
  turn_off_green();
  delay_ms(500);
}
void blink_green2() {
  turn_on_green2();
  delay_ms(500);
  turn_off_green2();
  delay_ms(500);
}
void blink_yellow() {
  turn_on_yellow();
  delay_ms(500);
  turn_off_yellow();
  //delay_ms(300);
}


void prepare_visual_outputs() {

	// init the LCD
	//lcd_init();
    //lcd_home();

	// configure LED ports as output
	DDRD |= (1<<DDD3);
	DDRD |= (1<<DDD4);
	DDRD |= (1<<DDD5);
	DDRD |= (1<<DDD6);
	DDRD |= (1<<DDD7);

}
