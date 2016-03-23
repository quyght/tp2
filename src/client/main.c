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
  int client_socket_fd, serv_fd;
  struct sockaddr_in serv_addr;
  struct hostent *server;
  char *name;
  int start = time(NULL);

  //Création du socket cllient
 client_socket_fd = socket(AF_INET, SOCK_STREAM,0);
  if(client_socket_fd ==-1){
	  perror("socket");
	  close(client_socket_fd);
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
      serv_fd = connect(client_socket_fd, (struct sockaddr *)&
	serv_addr,sizeof(struct sockaddr));
      if ((time (NULL) - start) >= max_wait_time)
	{
	  break;
	}
    }
  if(serv_fd == -1){
		perror("connection");
		close(client_socket_fd);
		exit(1);
	}
	//Envoi du BEGIN au serveur
	unsigned int buffer[3];
	buffer[0] = 1;
	buffer[1] = num_resources;
	buffer[2] = num_clients;
	//printf("%ul", sizeof(buffer[0]));
int bufferlen, comm;
bufferlen = 12;
comm = send(client_socket_fd, buffer, bufferlen, 0);
if (comm < 0){
	perror("send");
	close(client_socket_fd);
	close(serv_fd);
	exit(1);
}
comm = recv(client_socket_fd, buffer, bufferlen, 0);
printf("%d \n", buffer[0]);

if (buffer[0] != 3 && buffer[0] != 5 && buffer[0] != 7){
	printf("Not following the protocol (BEGIN : client) \n");
	buffer[0] = 9;
	comm = send(client_socket_fd, buffer, bufferlen, 0);
	exit(1);
}
if (buffer[1] != num_resources){
	printf("BEGIN : pas le meme nombre de resource entre server et client \n");
	buffer[0] = 7;
	comm = send(client_socket_fd, buffer, bufferlen, 0);
	exit(1);
}
//printf("%c \n", buffer[0]);
//printf("%d \n", comm);
	/*char buffer[256];
	buffer[0] = 'a';
	buffer[1] = '\0';
	int n;
    n = write(socket_fd,buffer,strbuffer(buffer));*/
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
