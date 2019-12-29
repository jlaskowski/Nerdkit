
void UdpSvrInit(uint16_t port);
int UpdSvrBegin();
int UdpSvrDataAvailable();
int UdpSvrReadData(char *buffer, int bufferSize);


extern uint16_t udpsvrport;
extern int udpsvrsocket;