int UpdSvrOpenSocket();
int UdpSvrReadData(char *buffer, int bufferSize);
int UdpClientSend(char* message);
int UpdCloseSocket();

extern uint16_t udpsvrport;
extern int udpsvrsocket;