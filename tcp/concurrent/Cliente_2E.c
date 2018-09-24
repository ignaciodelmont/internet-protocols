/*
    Cliente que retorna lo escrito
*/
#include<stdio.h> //printf
#include<string.h>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

#define MAXDATASIZE 280

int main(int argc , char *argv[])
{
  int sock;
  struct sockaddr_in server;
  char mensaje[1000] , respuesta_servidor[2000];

  //Crear socket
  sock = socket(AF_INET , SOCK_STREAM , 0);
  if (sock == -1) {
    printf("No se pudo crear el socket");
  }
  printf("Socket creado en puerto: %s", argv[1]);

  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons( 8888 );

  //Conexión al servidor remoto
  if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0) {
    perror("ERROR: Falló la conexión al servidor.");
    return 1;
  }

  puts("Conectado al Servidor\n");

  //Mantengo la comunicación con el servidor
  while(1) {
    printf("Ingresar texto: ");
    fgets(mensaje, MAXDATASIZE, stdin);

    //Enviar texto
    if( send(sock , mensaje , strlen(mensaje) , 0) < 0) {
      puts("ERROR: Falló envío de mensaje");
      return 1;
    }

    //Recibir respuesta del servidor
    if( recv(sock , respuesta_servidor , MAXDATASIZE , 0) < 0) {
      puts("ERROR: Falló respuesta desde el servidor");
      break;
    }

    puts("Respuesta del servidor:");
    puts(respuesta_servidor);
  }

  close(sock);
  return 0;
}
