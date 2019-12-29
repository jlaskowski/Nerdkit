#define F_CPU 14745600

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <inttypes.h>
#include <string.h>

#include "../libnerdkits/delay.h"
#include "../libnerdkits/io_328p.h"

#include "../CC3000HostDriver/nvmem.h"
#include "../CC3000HostDriver/evnt_handler.h"
#include "../CC3000HostDriver/wlan.h"
#include "../CC3000HostDriver/hci.h"
#include "../CC3000HostDriver/wlan.h"
#include "../CC3000HostDriver/security.h"

#include "spi.h"
#include "udphelper.h"

#include "wifi.h"

volatile unsigned long ulSmartConfigFinished, ulCC3000Connected,ulCC3000DHCP,OkToDoShutDown;
volatile unsigned long ulEventType = 0;


int initializeWifi() {

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

	//unsigned char* patchVer = (unsigned char*)"          ";
	//nvmem_read_sp_version(patchVer);

	while ((ulCC3000DHCP == 0) || (ulCC3000Connected == 0)) {
		hci_unsolicited_event_handler();
	    delay_ms(1000);
	}

}


int initializeWifiSmartConfig() {
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

	//unsigned char* patchVer = (unsigned char*)"          ";
	//nvmem_read_sp_version(patchVer);

    StartSmartConfig();

	while ((ulCC3000DHCP == 0) || (ulCC3000Connected == 0)) {
		hci_unsolicited_event_handler();
	    delay_ms(1000);
	}
	
}


void StartSmartConfig(void)
{
	ulSmartConfigFinished = 0;
	ulCC3000Connected = 0;
	ulCC3000DHCP = 0;
	OkToDoShutDown=0;
	char aucCC3000_prefix[] = {'T', 'T', 'T','\0'};
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

	// Wait for Smartconfig process complete
	while (ulSmartConfigFinished == 0)
	{
	    // TODO: need a way to communicate not-readiness
		delay_ms(500);
	}
	// TODO: need a way to communicate readiness here
    

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
    PCICR |= (1<<PCIE2);
}

void WlanInterruptDisable() {
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

