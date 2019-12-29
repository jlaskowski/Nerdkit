#define DISABLE										(0)
#define ENABLE										(1)


int initializeWifi();
int initializeWifiSmartConfig();
void StartSmartConfig(void);
void CC3000_UsynchCallback(long lEventType, char *data, unsigned char length);
char *sendWLFWPatch(unsigned long *Length);
char *sendDriverPatch(unsigned long *Length);
char *sendBootLoaderPatch(unsigned long *Length);
long ReadWlanInterruptPin(void);
void WlanInterruptEnable();
void WlanInterruptDisable();
void WriteWlanPin(unsigned char val);
