#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socekt() and bind() */
#include <arpa/inet.h> /* sockaddr_in and inet_ntoa() */
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */

#define ECHOMAX 255

void DieWithError(char*); /*External Error handling function*/

int main(int argc, char **argv){
  int sock;
  struct sockaddr_in echoServAddr;
  struct sockaddr_in echoClntAddr; // client address
  unsigned int cliAddrLen; // length of incoming message
  char echoBuffer[ECHOMAX]; // buffer for echo string
  unsigned short echoServPort; // server port
  int recvMsgSize; //size of received message

  if(argc != 2){
    fprintf(stderr, "Usage: %s <UDP SERVER PORT>\n", argv[0]);
    exit(1);
  }

  echoServPort = atoi(argv[1]);

  /* Create socket for sending/receiving datagrams */
  if((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    DieWithError("socket() failed");

  /* construct local address structure */
  memset(&echoServAddr, 0, sizeof(echoServAddr));
  echoServAddr.sin_family = AF_INET;
  echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  echoServAddr.sin_port = htons(echoServPort);

  /* Bind the local address */
  if(bind(sock, (struct sockaddr*) &echoServAddr, sizeof(echoServAddr)) < 0)
    DieWithError("bind() failed");

  for(;;){
    cliAddrLen = sizeof(echoClntAddr);  //set the size of in-out parameter
    /* block until receive message from a client */
    if((recvMsgSize = recvfrom(sock, echoBuffer, ECHOMAX, 0, (struct sockaddr*) &echoClntAddr, &cliAddrLen)) < 0)
      DieWithError("recvfrom() failed");
    printf("Handling client %s\n", inet_ntoa(echoClntAddr.sin_addr));

    /* send receving datagram back to the client */
    if(sendto(sock, echoBuffer, recvMsgSize, 0, (struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) != recvMsgSize)
      DieWithError("sendto() sent a different number of bytes than expected");
  }
  return 0;
}

void DieWithError(char *ErrorMessage){
  printf("%s\n", ErrorMessage);
  exit(1);
}
