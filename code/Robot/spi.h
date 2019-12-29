

// The magic number that resides at the end of the TX/RX buffer (1 byte after the allocated size)
// for the purpose of detection of the overrun. The location of the memory where the magic number
// resides shall never be written. In case it is written - the overrun occured and either recevie function
// or send function will stuck forever.
#define CC3000_BUFFER_MAGIC_NUMBER (0xDE)

#define SPI_READ                    3
#define SPI_WRITE                   1


void SpiOpen(void (*pfRxHandler)(void *p));

void SpiWriteDataSynchronous(unsigned char *data, unsigned short size);
long SpiFirstWrite(unsigned char* pUserBuffer, unsigned short usLengthOfPayloadMinusPad);
long SpiWrite(unsigned char *pUserBuffer, unsigned short usLength);

unsigned char SpiWriteReadByte(unsigned char outByte);
void SpiReadDataSynchronous(unsigned char *data, unsigned short size);
void SpiReadHeader(void);
long SpiReadDataCont(void);
void SpiTriggerRxProcessing(void);

void log_app_state();
void init_spi();

void SpiClose(void);
void SpiPauseSpi();
void SpiResumeSpi();



typedef void (*gcSpiHandleRx)(void *p);
extern unsigned char wlan_tx_buffer[CC3000_TX_BUFFER_SIZE];
extern unsigned char tSpiReadHeader[];
