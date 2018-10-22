/* This is a client for a TCP server which runs concurrently. The server sends some data
and the client shows that data by console.The client can answer with data too*/

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>

#define MAXDATASIZE 1000
/*

  To run the client: ./client serverIP LOCAL_PORT SERVER_PORT
  -->example:
  ./client 127.0.0.1 3000 3001

*/

int main(int argc, char const *argv[]) {
  /*sockfd is the socket descriptor. server and my_addr are sockaddr_in structs*/
  int sockfd,numBytes = 0;
  struct sockaddr_in server,my_addr;
  if( argc!=4 ){ // Check parameters
    printf("Ups. Something went wrong! Please check initial parameters.\n" );
    return (1);
  }
  /*Create socket*/
  if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1){
    printf("There was an error on socket creation\n" );
    return(1);
  }
  printf("Socket was created\n" );
  /*Client struct sockadd_in */
  my_addr.sin_family = AF_INET;
  my_addr.sin_addr.s_addr = INADDR_ANY;
  my_addr.sin_port = htons(atoi(argv[2]));
  /*Server struct sockadd_in */
  server.sin_port = htons(atoi(argv[3]));
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(argv[1]);
  printf("%s\n",argv[1] );
  /*The binding links the sockfd with the socket address*/
  if(bind(sockfd,(struct sockaddr *) &my_addr,sizeof(server))==-1){
    printf("Something went wrong while binding\n" );
    return(1);
  }
  printf("Binding Complete\n");
  /*Connect to the server*/
  if(connect(sockfd,(struct sockaddr *) &server,sizeof(my_addr))!=0){
    perror("Connection");
    close(sockfd);
    return (1);
  }
  printf("Connection Complete\n");

  /*Here we send and recv the data*/
  char msg[MAXDATASIZE],chatPort[MAXDATASIZE],msgrecv[MAXDATASIZE],exitMsg[]="bye";
  int exitFlag = 0;
  while(!exitFlag){

    /* To send to the server. "bye" stops the chat*/
    printf("Select the client:");
    scanf("%s", chatPort);
    if (send(sockfd,chatPort,strlen(chatPort), 0) == -1){
      perror("send");
    }
    printf("Send:");
    scanf("\n%[^\n]", msg);
    if(strcmp(msg,exitMsg)==0){
      exitFlag = 1;
    }
    if (send(sockfd, msg,strlen(msg), 0) == -1){
      perror("send");
    }
    printf("Bytes enviados:%ld\n",strlen(msg));
    /* To receive text from server if !exitFlag==1*/
    if(!exitFlag){
        if ((numBytes=recv(sockfd,msgrecv, MAXDATASIZE, 0)) == -1){
        perror("recv");
        exit(1);
        }
         msgrecv[numBytes] = '\0';
         printf("\nReceived: %s\n\n", msgrecv);
    }

  }
  printf("Ending connection\n");
  /*Close the socket*/
  if ( close(sockfd) == -1 ){
    printf("Error on socket closing");
    return(1);
  }
  printf("Socket closed\n");
  return 0;
}
