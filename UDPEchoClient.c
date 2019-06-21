#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socekt(), connect(), sendto() nad recvfrom() */
#include <arpa/inet.h> /* sockaddr_in and inet_addr() */
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */

#define ECHOMAX 255

void DieWithError(char*); /*External Error handling function*/

int main(int argc, char **argv){
  int sock;
  struct sockaddr_in echoServAddr; // echo server address
  struct sockaddr_in fromAddr; //source address of echo
  unsigned short echoServPort; //echo server port
  unsigned int fromSize; //In-out of address size for recvfrom()
  char *servIP; //ip address of the server
  char *echoString; //string to send to the echo server
  char *echoBuffer[ECHOMAX+1]; //buffer for receiving echoed string
  int echoStringLen; //Length of the string to echo
  int respStringLen; //Length of the received response

  if(argc < 3 || argc > 4){  // Test for correct number of input arguments
    fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
    exit(1);
  }

  servIP = argv[1];
  echoString = argv[2];

  if((echoStringLen = strlen(echoString)) > ECHOMAX){
    DieWithError("Echo word is too long");
  }

  if(argc == 4)
    echoServPort = atoi(argv[3]);
  else
    echoServPort = 7; // 7 is a well-known port for the echo service

  /* Create a datagram/UDP socket*/
  memset(&echoServAddr, 0, sizeof(echoServAddr)); //zero out the structure
  echoServAddr.sin_family = AF_INET;                // Internet address family
  echoServAddr.sin_addr.s_addr = inet_addr(servIP); // server ip address
  echoServAddr.sin_port = htons(echoServPort);      // server port

  /* Send the string to the server */
  if(sendto(sock, echoString, echoStringLen, 0, (struct sockaddr_in*) &echoServAddr, sizeof(echoServAddr)) != echoStringLen){
    DieWithError("Sendto() sent a different number of bytes than expected");
  }

  /* Receive a message */
  fromSize = sizeof(fromAddr);
  if((respStringLen = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr_in*) &fromAddr, &fromSize)) != echoStringLen)
    DieWithError("recvfrom() failed");

  if(echoServAddr.sin_addr.s_addr != fromAddr.sin_addr.s_addr){
    fprintf(stderr, "Error: received a packet from unknown source.\n");
    exit(1);
  }

  /* null terminate the received data */
  echoBuffer[respStringLen] = '\0';
  printf("Received: %s\n", echoBuffer);

  close(sock);
  return 0;
}

void DieWithError(char *ErrorMessage){
  printf("%s\n", ErrorMessage);
  exit(1);
}
