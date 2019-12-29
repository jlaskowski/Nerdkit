#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <inttypes.h>
#include <string.h>

#include "../libnerdkits/delay.h"
#include "../libnerdkits/io_328p.h"

#include "../CC3000HostDriver/cc3000_common.h"
#include "../CC3000HostDriver/socket.h"

#include "udphelper.h"


uint16_t udpsvrport = 0;
int udpsocket = -1;
sockaddr tSocketAddr;
char udpMessageBuffer[100];
sockaddr fromSockAddr;
socklen_t fromSockAddrLen;



int UpdSvrOpenSocket() {

   // Open the socket if it isn't already open.
   if (udpsocket == -1) {
		// Create the UDP socket
		int soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (soc < 0) {
			//TODO: Communicate Error
			return 0;
		}

		long optvalue = 60000;
		setsockopt(udpsocket, SOL_SOCKET, SOCKOPT_RECV_TIMEOUT, &optvalue, sizeof( optvalue ));
        //int recvNonBlock = SOCK_ON;
        //setsockopt(soc, SOL_SOCKET, SOCKOPT_RECV_NONBLOCK, &recvNonBlock, sizeof (recvNonBlock));


		sockaddr tSocketAddr;
		
		// create MCAST address 224.0.1.187 port 5454

		tSocketAddr.sa_family = AF_INET;

		// the destination port
		tSocketAddr.sa_data[0] = 0x15;
		tSocketAddr.sa_data[1] = 0x4e;

		// the destination IP address
		tSocketAddr.sa_data[2] = 0xe0;
		tSocketAddr.sa_data[3] = 0;
		tSocketAddr.sa_data[4] = 1;
		tSocketAddr.sa_data[5] = 0xbb;
		
		if (bind(soc, &tSocketAddr, sizeof(tSocketAddr)) < 0) {
			// TODO: Communicate Error
			return 0;
		}
		udpsocket = soc;
	}
   return 1;
}


int UdpSvrReadData(char *buffer, int bufferSize) {
	if (udpsocket == -1 && !UpdSvrOpenSocket()) {
	    return -1;
	}
	
	return recv(udpsocket, buffer, bufferSize, 0);
	//return retval = recvfrom(udpsocket, buffer, bufferSize, 0, &fromSockAddr,  &fromSockAddrLen);

}
/*
int UdpClientSend(char * message) {

	tSocketAddr.sa_family = AF_INET;


    if (udpsocket == -1) {
		// Create the UDP socket
		udpsocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (udpsocket < 0) {
			//TODO: Communicate Error
			return 0;
		}
			// the destination port
		tSocketAddr.sa_data[0] = 0x0F;
		tSocketAddr.sa_data[1] = 0xB5;

		// the destination IP address
		tSocketAddr.sa_data[2] = 192;
		tSocketAddr.sa_data[3] = 168;
		tSocketAddr.sa_data[4] = 1;
		tSocketAddr.sa_data[5] = 16;
    } else {
		sendto(udpsocket, message, strlen(message)+1,  0, &fromSockAddr, fromSockAddrLen);
	}
    return 1;

}
*/
int UpdCloseSocket() {
	int retVal = closesocket(udpsocket);
    udpsocket = -1;
	return retVal;
}
