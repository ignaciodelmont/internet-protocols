// udp client-server chat, the client should start the conversation
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>

#define MYPORT 3490

#define BACKLOG 5
#define MAXDATASIZE 100

int main() {
 int sockfd, new_fd;
 struct sockaddr_in my_addr;
 struct sockaddr_in their_addr;
 int sin_size;
 int numbytes;
 char buf[MAXDATASIZE];


 if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
  perror("socket");
  exit(1);
 }

 my_addr.sin_family = AF_INET;
 my_addr.sin_port   = htons(MYPORT);
 my_addr.sin_addr.s_addr = INADDR_ANY;
 bzero(&(my_addr.sin_zero), 8);

 if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
  perror("bind");
  exit(1);
 }


 while(1) {
   struct sockaddr_in host_address;
   int host_address_size;
   unsigned char *address_holder;
   char message[]="Mensaje enviado";
   char buffer[256];

   memset((void *)&host_address, 0, sizeof(host_address));
   host_address.sin_family=AF_INET;
   host_address.sin_port=htons(MYPORT);
   address_holder=(unsigned char*)&host_address.sin_addr.s_addr;

   memset((void*)&host_address, 0, sizeof(host_address));
   host_address.sin_family=AF_INET;
   host_address_size=sizeof(host_address);

   // recv from client
   if(recvfrom(sockfd, buffer, 255, 0, (struct sockaddr*)&host_address, &host_address_size)<0) {
     printf("%d", errno);
     perror("recvfrom()");
     return 1;
   }
   printf("Message coming from port %d: %s\n\n", MYPORT, buffer);

   // send message to client
   fflush(stdin);
   fgets(buffer, MAXDATASIZE, stdin);
   if (sendto(sockfd,	buffer, strlen(buffer) + 1, 0, (struct sockaddr*)&host_address, host_address_size)<0) {
      perror("sendto()");
      return 1;
   }
   printf("sent %d bytes\n\n", strlen(buffer));
 }
}
