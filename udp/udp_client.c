// udp client-server chat, the client should start the conversation
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 3490

#define MAXDATASIZE 100

int main(int argc, char *argv[])
{
  int sockfd;
  struct hostent *he;
  struct sockaddr_in their_addr;
  struct	sockaddr_in	host_address;
  unsigned	char	*address_holder;
  char	message[200]="";


  if (argc != 2 ) {
    fprintf(stderr, "usage: client hostname\n");
    exit(1);
  }

  if ((he=gethostbyname(argv[1])) == NULL) {
    perror("gesthostbyname");
    exit(1);
  }

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
    perror("socket");
    exit(1);
  }

  their_addr.sin_family = AF_INET;
  their_addr.sin_port   = htons(PORT);
  their_addr.sin_addr   = *((struct in_addr *)he->h_addr);
  memset(&(their_addr.sin_zero), '\0',8);


  memset((void*)&host_address,	0,	sizeof(host_address));
  host_address.sin_family=AF_INET;
  host_address.sin_port=htons(PORT);
  address_holder=(unsigned char*)&host_address.sin_addr.s_addr;

  while (1) {
    fflush(stdin);
    fgets(message, MAXDATASIZE, stdin);
    *(message + strlen(message) - 1) = '\0';
    if (sendto(sockfd,	message, strlen(message) + 1, 0, (struct sockaddr*)&their_addr, sizeof(struct sockaddr))<0) {
	     perror("sendto()");
		   return 1;
    }
    printf("sent %d bytes\n\n", strlen(message));
    int host_address_size;
    host_address_size = sizeof(host_address);
    if(recvfrom(sockfd, message, 255, 0, (struct sockaddr*)&host_address, &host_address_size)<0) {// Lee e imprime de donde obtuvo los datos.
      printf("%d", errno);
      perror("recvfrom()");
      return 1;
    }
    printf("From server: %s\n\n", message);
  }




   return 0;
}
