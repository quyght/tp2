#include <stdio.h>

#include "client_thread.h"

#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include "client_thread.h"
#include <sys/types.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "client_thread.h"

extern unsigned int num_clients;
extern const unsigned int max_wait_time;
extern const unsigned int num_resources;
extern const int port_number;

int
main (int argc, char *argv[argc + 1])
{
  client_thread client_threads[num_clients];

  //Faire le begin
  //Initialisation pour client
  int socket_fd, serv_fd;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char *name;
  size_t len;
  int start = time(NULL);

  //Création du socket cllient
  socket_fd = socket(AF_INET, SOCK_STREAM,0);
  if(socket_fd ==-1){
	  perror("socket");
	  close(socket_fd);
	  exit(1);
  }
  
  //Information du serveur
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons (port_number);
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  //Connection avec le serveur
  serv_fd = -1;
    while (serv_fd < 0)
    {
      serv_fd = connect(socket_fd, (struct sockaddr *)&
	serv_addr,sizeof(struct sockaddr));
      if ((time (NULL) - start) >= max_wait_time)
	{
	  break;
	}
    }
  
	printf("%d \n", socket_fd);
  if(serv_fd == -1){
		perror("connection");
		close(socket_fd);
		exit(1);
	}
	//Envoi du BEGIN au serveur
	unsigned int buffer[3];
	buffer[0] = 1;
	buffer[1] = num_resources;
	buffer[2] = num_clients;
	//printf("%ul", sizeof(msg[0]));
int lenbuffer, bytes_sent;
lenbuffer = 12;
bytes_sent = send(socket_fd, buffer, lenbuffer, 0);
if (bytes_sent < 0){
	perror("send");
	close(socket_fd);
	close(serv_fd);
	exit(1);
}/*
bytes_sent = recv(socket_fd, msg, lentest, 0);
if (msg[0] =! 3 || msg =! 5 || msg =! 7){
	printf("Not following the protocol");
	msg[0] = 9;
	bytes_sent = send(client_socket, msg, lentest, 0);
	exit(1);
}
if (msg[1] =! num_resources){
	printf("BEGIN : pas le meme nombre de resource entre server et client");
	msg[0] = 7;
	bytes_sent = send(client_socket, msg, lentest, 0);
	exit(1);
}*/
//printf("%c \n", msg[0]);
//printf("%d \n", bytes_sent);
	/*char buffer[256];
	buffer[0] = 'a';
	buffer[1] = '\0';
	int n;
    n = write(socket_fd,buffer,strlen(buffer));*/
  for (unsigned int i = 0; i < num_clients; i++)
    {
      ct_init (&(client_threads[i]));
    }

  for (unsigned int i = 0; i < num_clients; i++)
    {
      ct_create_and_start (&(client_threads[i]));
    }

  ct_wait_server ();

  // Affiche le journal.
  st_print_results (stdout, true);
  FILE *fp=fopen("client_log", "w");
  if (fp == NULL)
    {
      fprintf(stderr, "Could not print log");
      return EXIT_FAILURE;
    }
  st_print_results (fp, false);
  fclose(fp);

  return EXIT_SUCCESS;
}
