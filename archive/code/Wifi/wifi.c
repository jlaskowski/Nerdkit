#define F_CPU 14745600

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <inttypes.h>
#include <string.h>

#include "../libnerdkits/delay.h"
#include "../libnerdkits/lcd.h"
#include "../libnerdkits/io_328p.h"

#include "../CC3000HostDriver/nvmem.h"
#include "../CC3000HostDriver/evnt_handler.h"
#include "../CC3000HostDriver/wlan.h"
#include "../CC3000HostDriver/hci.h"
#include "../CC3000HostDriver/wlan.h"
#include "../CC3000HostDriver/security.h"
#include "../CC3000HostDriver/socket.h"

#include "spi.h"
#include "ui.h"
#include "udpserver.h"

#define DISABLE										(0)
#define ENABLE										(1)


void StartSmartConfig(void);
void CC3000_UsynchCallback(long lEventType, char *data, unsigned char length);
char *sendWLFWPatch(unsigned long *Length);
char *sendDriverPatch(unsigned long *Length);
char *sendBootLoaderPatch(unsigned long *Length);
long ReadWlanInterruptPin(void);
void WlanInterruptEnable();
void WlanInterruptDisable();
void WriteWlanPin(unsigned char val);

volatile unsigned long ulSmartConfigFinished, ulCC3000Connected,ulCC3000DHCP,OkToDoShutDown;
volatile unsigned long ulEventType = 0;
volatile long ulSocket;

char udpMessageBuffer[100];



int main() {

    prepare_visual_outputs();  // configures LCD, LED, and serial Port

    delay_ms(1000); // let the CC3000 stablize after VCC (TiWi_SL Datasheet)

    init_spi();


	wlan_init( CC3000_UsynchCallback,
	           sendWLFWPatch,
			   sendDriverPatch,
			   sendBootLoaderPatch,
			   ReadWlanInterruptPin,
			   WlanInterruptEnable,
			   WlanInterruptDisable,
			   WriteWlanPin);

	// first param zero means use the patch in CC3000 EPROM (assumes you downloaded patch)
	wlan_start(0,wlan_tx_buffer);


	// Mask out all non-required events from CC3000
	wlan_set_event_mask(HCI_EVNT_WLAN_KEEPALIVE|HCI_EVNT_WLAN_UNSOL_INIT|HCI_EVNT_WLAN_ASYNC_PING_REPORT);

	unsigned char* patchVer = (unsigned char*)"          ";
	nvmem_read_sp_version(patchVer);

	//lcd_line_one();
    //FILE lcd_stream = FDEV_SETUP_STREAM(lcd_putchar, 0, _FDEV_SETUP_WRITE);
	//fprintf_P(&lcd_stream, PSTR("VER(%d, %d) "), (int)patchVer[0],(int)patchVer[1]);

    blink_green();

    StartSmartConfig();


	while ((ulCC3000DHCP == 0) || (ulCC3000Connected == 0)) {
		hci_unsolicited_event_handler();
	    delay_ms(1000);
	}

    blink_green();

/*
UdpSvrInit((uint16_t)6024);
    UpdSvrBegin();
turn_on_green();
	int count = UdpSvrReadData(udpMessageBuffer, sizeof(udpMessageBuffer));

    if (count > 1) {
      udpMessageBuffer[count] = 0;
      turn_on_yellow();
	  //lcd_write_string((char*)udpMessageBuffer);
      FILE lcd_stream = FDEV_SETUP_STREAM(lcd_putchar, 0, _FDEV_SETUP_WRITE);
      fprintf_P(&lcd_stream, PSTR("RECV:[%s] "), udpMessageBuffer);
    }

while (1) {
  delay_ms(1000);
}
*/

	sockaddr tSocketAddr;
	tSocketAddr.sa_family = AF_INET;

	// the destination port: 5454
	tSocketAddr.sa_data[0] = 0x15;
	tSocketAddr.sa_data[1] = 0x4e;

	// the destination IP address 224.0.1.187
	tSocketAddr.sa_data[2] = 0xe0;
	tSocketAddr.sa_data[3] = 0;
	tSocketAddr.sa_data[4] = 1;
	tSocketAddr.sa_data[5] = 0xbb;


	ulSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	sendto(ulSocket, "Eat more Chicken", 16, 0, &tSocketAddr, sizeof(sockaddr));


    while (1) {
	  //hci_unsolicited_event_handler();
	  delay_ms(1000);
	}


  return 0;
}


void StartSmartConfig(void)
{
	ulSmartConfigFinished = 0;
	ulCC3000Connected = 0;
	ulCC3000DHCP = 0;
	OkToDoShutDown=0;
	char aucCC3000_prefix[] = {'T', 'T', 'T','\0'};
	
	//smartconfigAES16
	const unsigned char smartconfigkey[] = {0x73,0x6d,0x61,0x72,0x74,0x63,0x6f,0x6e,0x66,0x69,0x67,0x41,0x45,0x53,0x31,0x36};


	// Reset all the previous configuration
	wlan_ioctl_set_connection_policy(DISABLE, DISABLE, DISABLE);
	wlan_ioctl_del_profile(255);

	//Wait until CC3000 is disconnected
	while (ulCC3000Connected == 1)
	{
        delay_ms(1000);
		hci_unsolicited_event_handler();
	}

	wlan_smart_config_set_prefix((char*)aucCC3000_prefix);

	// Start the SmartConfig start process
	wlan_smart_config_start(1);

	turn_on_green2();

	FILE lcd_stream = FDEV_SETUP_STREAM(lcd_putchar, 0, _FDEV_SETUP_WRITE);

	// Wait for Smartconfig process complete
	while (ulSmartConfigFinished == 0)
	{

		delay_ms(500);
	}

    turn_off_green2();

	// create new entry for AES encryption key
	nvmem_create_entry(NVMEM_AES128_KEY_FILEID,16);

	// write AES key to NVMEM
	aes_write_key((unsigned char *)(&smartconfigkey[0]));

	// Decrypt configuration information and add profile
	wlan_smart_config_process();

	// Configure to connect automatically to the AP retrieved in the
	// Smart config process
	wlan_ioctl_set_connection_policy(DISABLE, DISABLE, ENABLE);

	// reset the CC3000
	wlan_stop();

	delay_ms(1000);


	wlan_start(0,wlan_tx_buffer);

	// Mask out all non-required events
	wlan_set_event_mask(HCI_EVNT_WLAN_KEEPALIVE|HCI_EVNT_WLAN_UNSOL_INIT|HCI_EVNT_WLAN_ASYNC_PING_REPORT);
}


void CC3000_UsynchCallback(long lEventType, char *data, unsigned char length)
{
	if (lEventType == HCI_EVNT_WLAN_ASYNC_SIMPLE_CONFIG_DONE)
	{
		ulSmartConfigFinished = 1;
	}

	if (lEventType == HCI_EVNT_WLAN_UNSOL_CONNECT)
	{
		ulCC3000Connected = 1;
	}

	if (lEventType == HCI_EVNT_WLAN_UNSOL_DISCONNECT)
	{
		ulCC3000DHCP = 0;
		ulCC3000Connected = 0;
	}

	if (lEventType == HCI_EVNT_WLAN_UNSOL_DHCP)
	{
		ulCC3000DHCP = 1;
	}

	if (lEventType == HCI_EVENT_CC3000_CAN_SHUT_DOWN)
	{
		OkToDoShutDown = 1;
	}

}

char *sendWLFWPatch(unsigned long *Length)
{
	*Length = 0;
	return NULL;
}


char *sendDriverPatch(unsigned long *Length)
{
	*Length = 0;
	return NULL;
}

char *sendBootLoaderPatch(unsigned long *Length)
{
	*Length = 0;
	return NULL;
}


long ReadWlanInterruptPin(void)
{
    return (PIND & (1 << PD2));
}


void WlanInterruptEnable() {
//lcd_write_string(PSTR("EN "));
    PCICR |= (1<<PCIE2);
}

void WlanInterruptDisable() {
//lcd_write_string(PSTR("DIS "));
    PCICR &= ~(1<<PCIE2);
}

void WriteWlanPin( unsigned char val )
{
	if(val)
	{
		PORTC |= (1<<PC3);  // power-enable the CC3000
	}
	else
	{
		PORTC &= ~(1<<PC3);  // power-disable the CC3000
	}

}

