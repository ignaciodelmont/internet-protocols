/*
    Ejemplo de servidor
*/

#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<stdlib.h>

int main(int argc , char *argv[])
{
  int socket_desc , socket_cliente , c , longitud_mensaje;
  struct sockaddr_in server , client;
  char mensaje_cliente[2000];

  //Crear socket
  socket_desc = socket(AF_INET , SOCK_STREAM , 0);
  if (socket_desc == -1) {
    printf("No se pudo crear el socket");
  }
  puts("Socket creado");

  //Preparo la estructura sockaddr_in
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons( 8888 );

  //Bind
  if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
    perror("ERROR: Falló el bind.");
    return 1;
  }
  puts("bind realizado");

  //Escucho en el puerto
  listen(socket_desc , 3);

  //Acepto las conexiones que llegan
  puts("Esperando conexiones desde clientes...");
  c = sizeof(struct sockaddr_in);

  //Recibo mensaje desde cliente
  while(socket_cliente = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) {
    int pid;
    puts("Conexión aceptada");
    if((pid = fork()) == 0) {
      while( (longitud_mensaje = recv(socket_cliente , mensaje_cliente , 2000 , 0)) > 0 ) {
	       //Envió la respuesta al cliente
	       write(socket_cliente , mensaje_cliente , strlen(mensaje_cliente));
      }

      if(longitud_mensaje == 0) {
	       puts("El cliente se desconectó");
	       fflush(stdout);
      }
      else if(longitud_mensaje == -1) {
	       perror("ERROR: falló la recepción del mensaje");
      }
      exit(0);
    }
  }
  return 0;
}
