#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include <inttypes.h>
#include <string.h>

#include "../libnerdkits/delay.h"
#include "../libnerdkits/lcd.h"
#include "../libnerdkits/io_328p.h"

#include "../CC3000HostDriver/cc3000_common.h"
#include "../CC3000HostDriver/socket.h"

#include "udpserver.h"
#include "ui.h"


uint16_t udpsvrport = 0;
int udpsvrsocket = -1;

void UdpSvrInit(uint16_t port) {
   udpsvrport = port;
   udpsvrsocket = -1;
}

int UpdSvrBegin() {

   // Open the socket if it isn't already open.
   if (udpsvrsocket == -1) {
      // Create the UDP socket
      int soc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if (soc < 0) {
         turn_on_red();
         return 0;
      }

     sockaddr tSocketAddr;
     tSocketAddr.sa_family = AF_INET;
	  
//TODO: convert uint16 initialized port to two 8-bit values	  
	  // the destination port
	  tSocketAddr.sa_data[0] = 0x0F;
	  tSocketAddr.sa_data[1] = 0xB5;
	  
	  tSocketAddr.sa_data[2] = 0;
	  tSocketAddr.sa_data[3] = 0;
	  tSocketAddr.sa_data[4] = 0;
	  tSocketAddr.sa_data[5] = 0;

      if (bind(soc, &tSocketAddr, sizeof(tSocketAddr)) < 0) {
         turn_on_yellow();
         return 0;
      }
	  turn_on_green();
      udpsvrsocket = soc;
   }


   return 1;
}

int UdpSvrDataAvailable() {
   timeval timeout;
   timeout.tv_sec = 0;
   timeout.tv_usec = 5000;
   fd_set reads;
   FD_ZERO(&reads);
   FD_SET(udpsvrsocket, &reads);
   select(udpsvrsocket + 1, &reads, NULL, NULL, &timeout);
   if (!FD_ISSET(udpsvrsocket, &reads)) {
      // No data to read.
      return 0;
   }
   
   return 1;
}

int UdpSvrReadData(char *buffer, int bufferSize) {
	  
	  sockaddr fromSockAddr;
	  socklen_t fromSockAddrLen;
	  
	  int n = recvfrom(udpsvrsocket, buffer, bufferSize, 0, &fromSockAddr,  &fromSockAddrLen);
	  
      //int n = recv(udpsvrsocket, buffer, bufferSize, 0);
      
      if (n < 1) {
         // Error getting data.
         turn_on_red();
        return -1;
      }
      
      return n;
}