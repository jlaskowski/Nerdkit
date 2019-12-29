
#include <ctype.h>
#include <string.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "../libnerdkits/delay.h"
#include "../libnerdkits/uart.h"
#include "../libnerdkits/io_328p.h"

#include "../CC3000HostDriver/cc3000_common.h"
#include "../CC3000HostDriver/data_types.h"
#include "../CC3000HostDriver/socket.h"
#include "../CC3000HostDriver/netapp.h"


#include "wifi.h"
#include "udphelper.h"
#include "spi.h"   // added for testing buffer overrun


void initializeMCUForRobot();

void motor_one_forward();
void motor_one_stop();
void motor_one_backward();
void motor_two_forward ();
void motor_two_stop();
void motor_two_backward();
void motor_three_forward();
void motor_three_stop();
void motor_three_backward();
void motor_four_forward();
void motor_four_stop();
void motor_four_backward();
void motor_five_forward();
void motor_five_stop();
void motor_five_backward();

void test_motor_one();
void test_motor_two();
void test_motor_three();
void test_motor_four();
void test_motor_five();


char readBuffer[5];

int main() {


	int bytesRead;

	initializeMCUForRobot();

	initializeWifi();


	while (1) {

		if (UpdSvrOpenSocket()) {	
			while (1) {

				bytesRead = UdpSvrReadData(readBuffer, sizeof(readBuffer)/sizeof(readBuffer[0]));
	
				if (bytesRead == 5) {
				
					if (readBuffer[0] == '+') {
						motor_one_forward();
					} else if (readBuffer[0] == '0') {
						motor_one_stop();
					} else if (readBuffer[0] == '-') {
						motor_one_backward();
					}

					if (readBuffer[1] == '+') {
						motor_two_forward ();
					} else if (readBuffer[1] == '0') {
						motor_two_stop();
					} else if (readBuffer[1] == '-') {
						motor_two_backward();
					}

					if (readBuffer[2] == '+') {
						motor_three_forward ();
					} else if (readBuffer[2] == '0') {
						motor_three_stop();
					} else if (readBuffer[2] == '-') {
						motor_three_backward();
					}


					if (readBuffer[3] == '+') {
						motor_four_forward ();
					} else if (readBuffer[3] == '0') {
						motor_four_stop();
					} else if (readBuffer[3] == '-') {
						motor_four_backward();
					}

					if (readBuffer[4] == '+') {
						motor_five_forward ();
					} else if (readBuffer[4] == '0') {
						motor_five_stop();
					} else if (readBuffer[4] == '-') {
						motor_five_backward();
					}

				} else {
					if (bytesRead < 0) {
						UpdCloseSocket();
						break;
					}
				}
			}
		} else {
		  //TODO: notifiy of error
		  break;
		}
	}

    while (1) {
		motor_five_forward ();
    }
}


void initializeMCUForRobot()  {
    // need 9 pins to drive robot
  DDRC |= (1<<DDC0);
  DDRC |= (1<<DDC2);

  DDRC |= (1<<DDC4);
  DDRC |= (1<<DDC5);

  DDRD |= (1<<DDD7);
  DDRD |= (1<<DDD6);

  DDRC |= (1<<DDC1);
  DDRD |= (1<<DDD3);

  DDRD |= (1<<DDD4);
  DDRD |= (1<<DDD5);

  DDRB |= (1<<DDB1);

}

void motor_one_forward() {
  PORTD |=  (1<<PD6);  
  PORTD &= ~(1<<PD7);  
}
void motor_one_stop() {
  PORTD &= ~(1<<PD6);
  PORTD &= ~(1<<PD7);  
}
void motor_one_backward() {
  PORTD &= ~(1<<PD6);  
  PORTD |=  (1<<PD7);  
}


void motor_two_forward() {
  PORTC |=  (1<<PC0);  
  PORTD &= ~(1<<PD5);
}
void motor_two_stop() {
  PORTC &= ~(1<<PC0);  
  PORTD &= ~(1<<PD5);
}
void motor_two_backward() {
  PORTC &= ~(1<<PC0);  
  PORTD |=  (1<<PD5);
}


void motor_three_forward() {
  PORTC |=  (1<<PC1);  
  PORTC &= ~(1<<PC2);  
}
void motor_three_stop() {
  PORTC &= ~(1<<PC1);  
  PORTC &= ~(1<<PC2);  
}
void motor_three_backward() {
  PORTC &= ~(1<<PC1);  
  PORTC |=  (1<<PC2);  
}


void motor_four_forward() {
  PORTD |=  (1<<PD4);  
  PORTD &= ~(1<<PD3);  
}
void motor_four_stop() {
  PORTD &= ~(1<<PD4);  
  PORTD &= ~(1<<PD3);  
}
void motor_four_backward() {
  PORTD &= ~(1<<PD4);  
  PORTD |=  (1<<PD3);  
}


void motor_five_forward() {
  PORTC |=  (1<<PC4);  
  PORTC &= ~(1<<PC5);  
}
void motor_five_stop() {
  PORTC &= ~(1<<PC4);  
  PORTC &= ~(1<<PC5);  
}
void motor_five_backward() {
  PORTC &= ~(1<<PC4);  
  PORTC |=  (1<<PC5);  
}


void test_motor_one() {
  motor_one_forward ();
  delay_ms(1200);
  motor_one_backward();
  delay_ms(1200);
  motor_one_stop();
}

void test_motor_two() {
  motor_two_forward ();
  delay_ms(1200);
  motor_two_backward();
  delay_ms(1200);
  motor_two_stop();
}

void test_motor_three() {
  motor_three_forward ();
  delay_ms(2000);
  motor_three_backward();
  delay_ms(3000);
 motor_three_stop();
}

void test_motor_four() {
  motor_four_forward ();
  delay_ms(1200);
  motor_four_backward();
  delay_ms(1200);
  motor_four_stop();
}


void test_motor_five() {
  motor_five_forward ();
  delay_ms(1000);
  motor_five_backward();
  delay_ms(1000);
  motor_five_stop();
}
