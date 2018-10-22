/* This is a TCP server which runs concurrently.  The server sends some data
and the client shows that data by console.The client can answer with data too*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BACKLOG 5
#define MAXDATASIZE 1000
#define SHMSZ     1000  // Size in bytes of shared memory
/*

  To run the next server: ./server SERVER_PORT
  -->example:
  ./server 3000

*/
int main(int argc, char const *argv[]) {
  int sockfd,new_sockfd,numBytes=0;
  socklen_t sin_size;
  struct sockaddr_in server,client;
  if( argc!=2 ){
    printf("Ups. Something went wrong! Please check initial parameters.\n" );
    return (1);
  }
  /*Create Socket*/
  if((sockfd = socket(AF_INET,SOCK_STREAM,0))==-1){
    printf("There was an error on socket creation\n" );
    return(1);
  }
  printf("Socket was created\n" );
  /*Server struct sockaddr_in*/
  server.sin_family = AF_INET;
  server.sin_port = htons(atoi(argv[1]));
  server.sin_addr.s_addr = INADDR_ANY;
  /*The binding links the sockfd with the socket address*/
  if(bind(sockfd,(struct sockaddr *) &server,sizeof(server))==-1){
    printf("Something went wrong while binding\n" );
    return(1);
  }
  printf("Binding Complete\n");
  /*Now just start listen for connections. The BACKLOG queue was set to 5 request*/
  if(listen(sockfd,BACKLOG)){
    printf("Server can't listen\n");
    return(1);
  }
  printf("Server listening on Port:%d\n",atoi(argv[1]) );

  while(1){
    if ((new_sockfd = accept(sockfd,(struct sockaddr*)&client,&sin_size)) != -1){
      printf("Something went wrong while connecting\n");
    }
    printf("server: got connection from %s on port: %d\n", inet_ntoa(client.sin_addr),ntohs(client.sin_port));
    if (!fork()){
      /*Here we fork for a concurrent server. Basically here is where you change the code*/
      char msg[MAXDATASIZE],msgPort[MAXDATASIZE],fullMsg[MAXDATASIZE],exitMsg[]="bye";
      int exitFlag = 0;

      int myShmid,shmid;
      key_t myKey,key;
      char *myShm = "",*shm = "";
      // key creation
      myKey = (int)ntohs(client.sin_port);

      // segment location
      if ((myShmid = shmget(myKey, SHMSZ, IPC_CREAT | 0666)) < 0) {
         perror("shmget");
         exit(1);
     }
      // segment attaching
      if ((myShm = shmat(myShmid, NULL, 0)) == (char *) -1) {
          perror("shmat");
          exit(1);
      }
      // fill shared memory with empty string so it's "clean"
      *myShm = "";

      while(!exitFlag){
        // Receives port msg from client
        if ((numBytes=recv(new_sockfd, msgPort, MAXDATASIZE, 0)) == -1){
          perror("received");
          exit(1);
        }
        msgPort[numBytes] = ':';
        //  Receives msg from client
        if ((numBytes=recv(new_sockfd, msg, MAXDATASIZE, 0)) == -1){
          perror("received");
          exit(1);
        }
        msg[numBytes] = '\0';
        key = atoi(msgPort);

        strcpy(fullMsg,msgPort);
        strcat(fullMsg,msg);
        if ((shmid = shmget(key, SHMSZ,0666)) < 0) {
           perror("shmget");
           exit(1);
        }
        // segment attaching
        if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
            perror("shmat");
            exit(1);
        }
        // fill shared memory with the message
        strcpy(shm,fullMsg);


        if(strcmp(msg,exitMsg)==0){
          exitFlag = 1;
        }
        else{
          if (send(new_sockfd, myShm,strlen(myShm), 0) == -1)
            perror("send");
          }
          printf("Bytes enviados:%ld\n",strlen(msg));
        }
        printf("Ended connection!\n" );
        close(new_sockfd);
    }

  }
  /*Close the socket*/
  if ( close(sockfd) == -1 ){
    printf("Error on socket closing");
    return(1);
  }
  printf("Socket closed\n");
  return 0;
}
